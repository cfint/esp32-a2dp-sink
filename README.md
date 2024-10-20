# ESP32 A2DP sink with extra codecs

Supported codecs:

 * AAC
 * aptX[-LL/-HD]
 * LDAC
 * Opus
 * LC3plus HR

## Requirements

The original ESP32-WROOM is the only ESP32 chip that supports Bluetooth A2DP.

The AAC codec __requires__ PSRAM. It will only work on ESP32 boards such as WROVER or ESP32-CAM. The decoder requires a lot of memory. The serial flash and PSRAM must be run at 80Mhz in order to decode fast enough. Set the CPU frequency to 240Mhz. See `sdkconfig.defaults` for the required configurations.


## Usage

Use `v5.1.4-a2dp-codecs` branch from https://github.com/cfint/esp-idf

Add additional codecs to to `components/bt/host/bluedroid/external/` in esp-idf.
 *  `master` branch @ https://github.com/cfint/libfreeaptx-esp
 *  `idf_component` branch @ https://github.com/cfint/arduino-fdk-aac

Build this project as you would an esp-idf project.


## Note about bluetooth on Linux

The bluetooth stack on Linux keeps a cache of the bluetooth codec configurations per device. If an A2DP sink changes its list of codecs then the cache needs to be cleared. "Forget" the ESP32 bluetooth device and pair it again to re-enumerate the list of codecs. The cache is located at `/var/lib/bluetooth/<FF:FF:FF:FF:FF:FF>/cache/<AA:AA:AA:AA:AA:AA>`. Where `<FF:FF:FF:FF:FF:FF>` is the mac address of the bluetooth device on your computer. `<AA:AA:AA:AA:AA:AA>` is the mac address of the ESP32 bluetooth device.

