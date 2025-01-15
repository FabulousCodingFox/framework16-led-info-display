#include "./ledmatrix.hpp"

#include <codecvt>

namespace lib::inputmodule::ledmatrix
{
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

  void pattern_text(libusb_device_handle* device, const std::string& text)
  {
    spdlog::trace("Setting text to {}", text);
    std::vector<std::string> parts = splitUTF8(text);
    pattern_symbols(device, parts);
  }

  void pattern_symbols(libusb_device_handle* device, std::vector<std::string>& parts)
  {
    std::vector<std::reference_wrapper<const std::array<bool, 30>>> font_items;

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

    inputmodule::send_command(device, inputmodule::CommandVals::Draw, vals);
  };
} // namespace lib::inputmodule::ledmatrix