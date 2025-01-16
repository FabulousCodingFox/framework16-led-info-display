#include "./usb.hpp"
#include "spdlog/spdlog.h"

namespace lib::usb
{
  static const auto HANDLES = new std::vector<ledmatrix::ledmatrix*>();
  static libusb_context* ctx = nullptr;

  auto init() -> std::vector<ledmatrix::ledmatrix*>*
  {
    // Initialize the libusb context
    spdlog::debug("[USB] Initializing libusb");
    int r = libusb_init(&ctx);
    if (r < 0)
    {
      spdlog::error("[USB] Failed to initialize libusb: {}", r);
      return HANDLES;
    }

    // Get the list of USB devices
    spdlog::debug("[USB] Listing USB devices");
    libusb_device** dev_list = nullptr;
    ssize_t cnt = libusb_get_device_list(ctx, &dev_list);
    if (cnt < 0)
    {
      spdlog::error("[USB] Failed to get device list: {}", cnt);
      libusb_exit(ctx);
      return HANDLES;
    }

    // Iterate over the device list
    spdlog::debug("[USB] Iterating over devices");

    for (ssize_t i = 0; i < cnt; i++)
    {
      libusb_device* device = dev_list[i];
      libusb_device_descriptor desc;

      // Retrieve the device descriptor
      if (libusb_get_device_descriptor(device, &desc) == 0)
      {
        // Check if it matches VID=0x32AC and PID=0x0020
        if (desc.idVendor == ledmatrix::VID && desc.idProduct == ledmatrix::PID)
        {
          spdlog::debug("[USB] Found Framework LED Matrix device");

          // Attempt to open this device
          libusb_device_handle* handle = nullptr;
          r = libusb_open(device, &handle);
          if (r == 0 && handle != nullptr)
          {
            spdlog::debug("[USB] Successfully opened device.");

            r = libusb_set_configuration(handle, 1);
            if (r != LIBUSB_SUCCESS && r != LIBUSB_ERROR_BUSY)
            {
              spdlog::error("[USB] Failed to set configuration: {}", libusb_strerror((libusb_error) r));
              libusb_close(handle);
              continue;
            }

#ifdef __linux__
            // On Linux, if a kernel driver is attached, detach it.
            int interfaceNum = 1;
            if (libusb_kernel_driver_active(handle, 1) == 1)
            {
              r = libusb_detach_kernel_driver(handle, 1);
              if (r != LIBUSB_SUCCESS)
              {
                spdlog::error("[USB] Could not detach kernel driver: {}", libusb_strerror((libusb_error) r));
                libusb_close(handle);
                continue;
              }
            }
#endif

            // Claim the interface
            r = libusb_claim_interface(handle, 1);
            if (r != LIBUSB_SUCCESS)
            {
              spdlog::error("[USB] Could not claim interface 1: {}", libusb_strerror((libusb_error) r));
              libusb_close(handle);
              continue;
            }

            spdlog::debug("[USB] Successfully claimed interface 1");

            // Store the handle
            HANDLES->push_back(new ledmatrix::ledmatrix(handle));
          }
          else
          {
            spdlog::error("[USB] Failed to open device: {}", r);
          }
        }
      }
      else
      {
        spdlog::error("[USB] Failed to get device descriptor for device index {}", i);
      }
    }

    // Now we can free the device list itself
    libusb_free_device_list(dev_list, 1);

    return HANDLES;
  };

  void exit()
  {
    spdlog::debug("[USB] Freeing resources");
    // Close and free resources at the end
    for (auto handle : *HANDLES)
    {
      delete handle;
    }

    spdlog::debug("[USB] Exiting libusb");
    HANDLES->clear();
    libusb_exit(ctx);
  };
} // namespace lib::usb