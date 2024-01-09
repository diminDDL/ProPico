# ProPico
An RP2040 dev board improving on aspects of the pi pico.

### ⚠️ This project is still under development, reproduce at your own risk ⚠️
While I have assembled several of these boards and they seem to work, there is still some verification to be done, so don't expect this to be a fully finished project just yet.
## Features
- An LDO for power making the 3V3 supply much cleaner.
- Larger flash sizes up to 16MB (the higher capacities will likely require a different board revision due to the larger IC package) with binary indicators on the bottom so you remember which board has what.
- USB-C for power and programming, includes CC resistors meaning it'll work with modern USB-C chargers/cables.
- A reset button alongside the bootsel, allowing you to reboot into UF2 mode without having to unplug the board.
- Extra GPIO broken out and used:
    * GPIO 23 - connected to an extra LED.
    * GPIO 24 - connected to the EEPROM I2C line.
    * GPIO 25 - connected to the EEPROM I2C line and an LED (for compatibility with the pico led pinout).
    * GPIO 29 (ADC3) - replaces the 3V3_EN pin on the original pico, giving you an extra ADC pin.
- LM4040 voltage reference for the ADC.
- I2C EEPROM/FRAM onboard so you have easy to use non volatile storage.
- RUN pin exposed next to the debug port.

## Pinout
![ProPico pinout](https://github.com/diminDDL/ProPico/blob/main/outputs/pinout_diagram/pico-pinout_export.svg?sanitize=true)
