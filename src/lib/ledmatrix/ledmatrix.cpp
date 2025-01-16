#include "./ledmatrix.hpp"
#include "./font.hpp"
#include <codecvt>

namespace lib::ledmatrix
{
  static const std::array<uint8_t, 2> FWK_MAGIG = {0x32, 0xAC};
  static const uint8_t RESPONSE_SIZE = 32;
  static const uint8_t ENDPOINT_OUT = 0x01;
  static const uint8_t ENDPOINT_IN = 0x82;
  static const int TRANSFER_TIMEOUT_MS = 1000;
  static int _id_counter = 0;

  ledmatrix::ledmatrix(libusb_device_handle* device)
    : device(device)
    , id(++_id_counter)
  {
  }

  ledmatrix::~ledmatrix()
  {
    // Reset the device
    int r = libusb_reset_device(device);
    if (r != LIBUSB_SUCCESS)
    {
      spdlog::warn("Could not reset device: {}", libusb_strerror((libusb_error) r));
    }

    // Release the interface
    r = libusb_release_interface(device, 1);
    if (r != LIBUSB_SUCCESS)
    {
      spdlog::warn("Could not release interface: {}", libusb_strerror((libusb_error) r));
    }

#ifdef __linux__
    // Re-attach kernel driver
    r = libusb_attach_kernel_driver(device, 1);
    if (r != LIBUSB_SUCCESS)
    {
      spdlog::warn("Could not attach kernel driver: {}", libusb_strerror((libusb_error) r));
    }
#endif

    libusb_close(device);
  }

  void ledmatrix::apply_config(QSettings* settings, lib::preset::PresetManager* presets)
  {
    if (this->preset != nullptr)
    {
      this->preset->exit(this);
      // delete this->preset;
      this->preset = nullptr;
    }

    int brightness = settings->value(QString("%1_brightness").arg(id), 150).toInt();
    this->brightness((uint8_t) brightness);

    std::string preset_id = settings->value(QString("%1_preset").arg(id), "none").toString().toStdString();
    auto preset = presets->get(preset_id);
    if (preset == nullptr)
    {
      spdlog::error("Preset {} not found", preset_id);
      return;
    }

    // this->preset = preset->clone().get();
    this->preset = preset.get();

    this->preset->init(this);
  };

  void ledmatrix::send_command(command command, const std::vector<uint8_t>& parameters)
  {
    // Build the outgoing data packet
    std::vector<uint8_t> outData;
    outData.insert(outData.end(), std::begin(FWK_MAGIG), std::end(FWK_MAGIG));
    outData.push_back(command);
    outData.insert(outData.end(), parameters.begin(), parameters.end());

    spdlog::trace("Sending command {} with {} parameters", static_cast<int>(command), parameters.size());

    // Send the data via bulk OUT transfer
    int actual_length = 0;
    int ret = LIBUSB_ERROR_OTHER;
    ret = libusb_bulk_transfer(device, ENDPOINT_OUT, const_cast<uint8_t*>(outData.data()), static_cast<int>(outData.size()), &actual_length, TRANSFER_TIMEOUT_MS);
    if (ret != LIBUSB_SUCCESS)
    {
      spdlog::error("Bulk OUT transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
    }

    if (actual_length != outData.size())
    {
      spdlog::error("Bulk OUT transfer size does not match. Expected {}; Got {}", outData.size(), actual_length);
    }
  }

  auto ledmatrix::send_command_with_response(command command, const std::vector<uint8_t>& parameters) -> std::vector<uint8_t>
  {
    // Build the outgoing data packet
    std::vector<uint8_t> outData;
    outData.insert(outData.end(), std::begin(FWK_MAGIG), std::end(FWK_MAGIG));
    outData.push_back(command);
    outData.insert(outData.end(), parameters.begin(), parameters.end());

    spdlog::trace("Sending command {} with {} parameters", static_cast<int>(command), parameters.size());

    // Send the data via bulk OUT transfer
    int actual_length = 0;
    int ret = LIBUSB_ERROR_OTHER;
    ret = libusb_bulk_transfer(device, ENDPOINT_OUT, const_cast<uint8_t*>(outData.data()), static_cast<int>(outData.size()), &actual_length, TRANSFER_TIMEOUT_MS);
    if (ret != LIBUSB_SUCCESS)
    {
      spdlog::error("Bulk OUT transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
      return {};
    }

    std::vector<uint8_t> inData(RESPONSE_SIZE, 0);

    ret = libusb_bulk_transfer(device, ENDPOINT_IN, inData.data(), RESPONSE_SIZE, &actual_length, TRANSFER_TIMEOUT_MS);

    if (ret != LIBUSB_SUCCESS)
    {
      spdlog::error("Bulk IN transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
      return {};
    }

    // Shrink the buffer to the actual number of bytes read
    inData.resize(actual_length);
    return inData;

    return {};
  }

  auto splitUTF8(const std::string& input) -> std::vector<std::string>
  {
    std::vector<std::string> result;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    try
    {
      // Convert UTF-8 string to a wide string (char32_t for full Unicode)
      std::u32string wideStr = converter.from_bytes(input);

      // Iterate over each Unicode character and convert back to UTF-8
      for (char32_t ch : wideStr)
      {
        result.push_back(converter.to_bytes(ch));
      }
    }
    catch (const std::range_error& e)
    {
      spdlog::error("Error processing string: {}", e.what());
    }

    return result;
  }

  void ledmatrix::pattern_text(const std::string& text)
  {
    spdlog::trace("Setting text to {}", text);
    std::vector<std::string> parts = splitUTF8(text);
    this->pattern_symbols(parts);
  }

  void ledmatrix::pattern_symbols(std::vector<std::string>& parts)
  {
    std::vector<std::reference_wrapper<const std::array<bool, font::PIXELS>>> font_items;

    for (size_t i = 0; i < std::min(parts.size(), size_t(5)); ++i)
    {
      font_items.push_back(std::ref(font::get_char(parts[i])));
    }

    std::vector<uint8_t> vals(39, 0x00);

    for (size_t digit_i = 0; digit_i < font_items.size(); ++digit_i)
    {
      const auto& digit_pixels = font_items[digit_i].get();
      size_t offset = digit_i * 7;

      for (size_t pixel_x = 0; pixel_x < font::WIDTH; ++pixel_x)
      {
        for (size_t pixel_y = 0; pixel_y < font::HEIGHT; ++pixel_y)
        {
          bool pixel_value = digit_pixels[pixel_x + pixel_y * font::WIDTH];
          size_t i = (2 + pixel_x) + (9 * (pixel_y + offset));

          if (pixel_value)
          {
            vals[i / 8] |= (1 << (i % 8));
          }
        }
      }
    }

    this->send_command(command::Draw, vals);
  };

  void ledmatrix::pattern_count(int value)
  {
    spdlog::trace("Setting pattern to count ", value);
    if (value > PIXELS || value < 0)
    {
      spdlog::error("Value must be between 0 and {}", PIXELS);
      return;
    }

    std::vector<uint8_t> vals(39, 0x00);

    for (int byte = 0; byte < value / CHAR_BIT; ++byte)
    {
      vals[byte] = 0xFF;
    }
    for (int i = 0; i < value % CHAR_BIT; ++i)
    {
      vals[value / 8] += 1 << i;
    }

    this->send_command(command::Draw, vals);
  }

  auto ledmatrix::get_pwm_freq() -> int
  {
    auto res = this->send_command_with_response(command::PwmFreq);
    if (res.size() == 0)
      return -1;
    uint8_t freq = res[0];
    if (freq == 0)
      return 29000;
    if (freq == 1)
      return 3600;
    if (freq == 2)
      return 1800;
    if (freq == 3)
      return 900;
    return -1;
  }

  void ledmatrix::pattern_matrix(std::vector<bool>& matrix)
  {
    spdlog::trace("Setting pattern to matrix with {} values", matrix.size());
    std::vector<uint8_t> vals(39, 0x00);

    for (int x = 0; x < WIDTH; ++x)
    {
      for (int y = 0; y < HEIGHT; ++y)
      {
        int i = x + y * WIDTH;
        if (matrix[i])
          vals[i / 8] |= (1 << (i % 8));
      }
    }

    this->send_command(command::Draw, vals);
  }

  void ledmatrix::pattern_equalizer(std::vector<uint8_t>& values)
  {
    spdlog::trace("Setting pattern to equalizer with {} values", values.size());
    std::vector<bool> matrix(PIXELS, false);
    for (int col = 0; col < std::min(values.size(), (std::size_t) 9); ++col)
    {
      int val = values[col];

      int row = HEIGHT / 2;
      int above = val / 2;
      int below = val - above;

      for (int i = 0; i < above; i++)
      {
        matrix[col + (row + i) * WIDTH] = true;
      }

      for (int i = 0; i < below; i++)
      {
        matrix[col + (row - 1 - i) * WIDTH] = true;
      }
    }
    this->pattern_matrix(matrix);
  }
} // namespace lib::ledmatrix