#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define DEFAULT_EEPROM_CLOCK            100000
#define DEFAULT_EEPROM_PAGE_SIZE        16
#define DEFAULT_EEPROM_SIZE             512
#define DEFAULT_EEPROM_WRITE_CYCLE      5
#define EEPROM_WORD_SIZE                4


class Memory
{
public:
    Memory(unsigned int device_address, i2c_inst_t* i2c);
    
    bool verifyConnection();
    int selfTest();

    unsigned int readWord(unsigned int target_address);
    void writeWord(unsigned int target_address, unsigned int data);
private:
    unsigned int write_cycle_time;
    unsigned int eeprom_addr; 
    i2c_inst_t*  i2c;
};