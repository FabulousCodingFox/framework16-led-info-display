#include "./inputmodule.hpp"
#include "spdlog/spdlog.h"
#include <iostream>

namespace lib::inputmodule::inputmodule
{
  static const std::array<unsigned char, 2> FWK_MAGIG = {0x32, 0xAC};
  static const unsigned char RESPONSE_SIZE = 32;
  static const unsigned char ENDPOINT_OUT = 0x01;
  static const unsigned char ENDPOINT_IN = 0x82;
  static const int TRANSFER_TIMEOUT_MS = 1000;

  void send_command(libusb_device_handle* device, unsigned char command, const std::vector<unsigned char>& parameters)
  {
    // Build the outgoing data packet
    std::vector<unsigned char> outData;
    outData.insert(outData.end(), std::begin(FWK_MAGIG), std::end(FWK_MAGIG));
    outData.push_back(command);
    outData.insert(outData.end(), parameters.begin(), parameters.end());

    spdlog::trace("Sending command {} with {} parameters", static_cast<int>(command), parameters.size());

    // Send the data via bulk OUT transfer
    int actual_length = 0;
    int ret = LIBUSB_ERROR_OTHER;
    ret = libusb_bulk_transfer(device, ENDPOINT_OUT, const_cast<unsigned char*>(outData.data()), static_cast<int>(outData.size()), &actual_length, TRANSFER_TIMEOUT_MS);
    if (ret != LIBUSB_SUCCESS)
    {
      spdlog::error("Bulk OUT transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
    }
  }

  auto send_command_with_response(libusb_device_handle* device, unsigned char command, const std::vector<unsigned char>& parameters) -> std::vector<unsigned char>
  {
    // Build the outgoing data packet
    std::vector<unsigned char> outData;
    outData.insert(outData.end(), std::begin(FWK_MAGIG), std::end(FWK_MAGIG));
    outData.push_back(command);
    outData.insert(outData.end(), parameters.begin(), parameters.end());

    spdlog::trace("Sending command {} with {} parameters", static_cast<int>(command), parameters.size());

    // Send the data via bulk OUT transfer
    int actual_length = 0;
    int ret = LIBUSB_ERROR_OTHER;
    ret = libusb_bulk_transfer(device, ENDPOINT_OUT, const_cast<unsigned char*>(outData.data()), static_cast<int>(outData.size()), &actual_length, TRANSFER_TIMEOUT_MS);
    if (ret != LIBUSB_SUCCESS)
    {
      spdlog::error("Bulk OUT transfer failed: {}", libusb_strerror(static_cast<libusb_error>(ret)));
      return {};
    }

    std::vector<unsigned char> inData(RESPONSE_SIZE, 0);

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
} // namespace lib::inputmodule::inputmodule