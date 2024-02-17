#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "blink.pio.h"
#include "Memory.hpp"

// ProPico pinout definitions
#define LED0 25
#define LED1 23

// Redefine the default LED to one that won't interfere with the I2C EEPROM
#define PICO_DEFAULT_LED_PIN 23

// I2C EEPROM pinout definitions
#define EEPROM_SDA 24
#define EEPROM_SCL 25


int main() {
    // Overclocking if needed
    // 400Mhz should be stable for most RP2040 chips, adjust the voltage as needed
    // Also don't forget to enable the XIP divider in CMakeLists.txt
    // vreg_set_voltage(VREG_VOLTAGE_MAX);
    // set_sys_clock_khz(400000, true);

    stdio_init_all();

    // setup PIO to blink the LED
    static const float pio_freq = 1000;
    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &blink_program);
    float div = (float)clock_get_hz(clk_sys) / pio_freq;
    blink_program_init(pio, sm, offset, LED1, div);
    pio_sm_set_enabled(pio, sm, true);

    // Initialize the I2C EEPROM
    gpio_set_function(EEPROM_SDA, GPIO_FUNC_I2C);
    gpio_set_function(EEPROM_SCL, GPIO_FUNC_I2C);
    i2c_init(i2c0, DEFAULT_EEPROM_CLOCK);

    // Initialize the Memory class
    Memory memory(0x50, i2c0);

    sleep_ms(1000);
    // Check if the EEPROM is connected
    if (memory.verifyConnection()) {
        printf("EEPROM connected\n");
    } else {
        printf("EEPROM not connected\n");
        return 0;
    }

    if(memory.readWord(0) != 0xDEADBEEF)
    {
        printf("EEPROM not initialized\n");
        // Perform a self test on the EEPROM
        int errors = memory.selfTest();
        printf("Self test found %d errors\n", errors);

        // Write some of our own data to the EEPROM
        memory.writeWord(0, 0xDEADBEEF);
        memory.writeWord(4, 0xCAFEBABE);
        memory.writeWord(8, 0x12345678);
    }else{
        printf("EEPROM initialized, we will just read from it\n");
    }

    while (true) {
        // Read the data back
        printf("Read back: 0x%08X\n", memory.readWord(0));
        printf("Read back: 0x%08X\n", memory.readWord(4));
        printf("Read back: 0x%08X\n", memory.readWord(8));
        sleep_ms(1000);
    }
}
