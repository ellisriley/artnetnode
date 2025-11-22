# ArtNet Node for ESP32

Lightweight Art-Net (ArtDMX) node implementation for ESP32 devices. Receives Art-Net over UDP and outputs NeoPixel signalling data. Designed for simple LED and fixture control on Wi‑Fi networks.

## Features
- Art-Net (ArtDMX) receiver
- Wi‑Fi client mode (connects to existing network)
- FastLED output to LEDs

## Hardware
- ESP32 development board (e.g., WROOM)
- 5V power supply for transceiver and fixtures (as required)


## Configuration
- Set SSID/PASSWORD or implement captive portal/config file.
- Set TARGET_UNIVERSE to the desired Art-Net universe.
- Choose a GPIO for FastLED accordingly.

## Troubleshooting
- Ensure Wi‑Fi and router allow UDP on port 6454.
- Verify DMX polarity and termination resistor.
- Use a serial monitor to inspect incoming Art-Net packets for debugging.

## License & Copyright
Copyright (c) 2025 Riley Ellis — ellisnet.me