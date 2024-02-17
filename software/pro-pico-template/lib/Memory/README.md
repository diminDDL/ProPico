### Source and License
This library was originally developed by [Cuprum](https://github.com/Cuprum77) and was part of the [PD2Breadboard](https://github.com/Cuprum77/PD2Breadboard) project. It is licensed under the MIT license. 

# EEPROM Memory Library
This library provides a simple interface to the EEPROM chip onboard the USB-PD board which uses the 24C04 EEPROM chip.


It is designed around the idea that all variables are stored as words and should be accessed as such. This means that if you want to store a byte, you must store it as a word. This is done to simplify the interface and to make it easier to store and retrieve data. The library also provides a simple interface to store and retrieve data from the EEPROM chip.

## Usage
To use the library, simply include the header file in your code:
```c
#include "Memory.hpp"
```

### Initialization
To initialize the Memory library, simply create a new Memory object where you provide the address of the EEPROM chip and the I2C bus that it is connected to.
```cpp
// Create a new Memory object that is connected to the EEPROM chip at address 0x50 on I2C bus 0
Memory memory(0x50, i2c0);
``` 

### Writing data
To write data to the EEPROM, simply call the `writeWord` function and provide the address to write to and the data to write.
```cpp
// Create a variable to store our data
unsigned int data = 0x12345678;
unsigned char address = 0x00;
// Write the data to the EEPROM
memory.writeWord(address, data);
```

### Reading data
To read data from the EEPROM, simply call the `readWord` function and provide the address to read from.
```cpp
// Create a variable to store our data
unsigned int data;
unsigned char address = 0x00;
// Read the data from the EEPROM
data = memory.readWord(address);
```

## Advanced capabilities
The library also features a few quality of life features that makes it easier to make sure the chip is working correctly.
### Verify Connection
To verify that the EEPROM chip is connected to the I2C bus, simply call the `verifyConnection` function. This function will return `true` if the EEPROM chip is connected to the I2C bus and `false` if it is not.
```cpp
// Verify that the EEPROM chip is connected to the I2C bus
if (memory.verifyConnection()) 
{
    // EEPROM chip is connected
} 
else 
{
    // EEPROM chip is not connected
}
```
### Self test
To verify that all the data is stored correctly, the library provides a self test function. This function will first try to read off the data that was written to avoid deleting important data. Then it will write a random number that is stored in the EEPROM chip and then read back to verify that it was stored correctly. The function will return the number of words that were not stored correctly. If the function returns 0, then the EEPROM chip is working correctly.
```cpp
// Run the self test
unsigned int errors = memory.selfTest();
if (errors == 0) 
{
    // EEPROM chip is working correctly
} 
else 
{
    // EEPROM chip is not working correctly
}
```

### Notes
* The EEPROM this was designed for is only 512 bytes, that means you only have 128 words to store data in.