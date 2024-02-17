#include "Memory.hpp"

/**
 * @brief Construct a new Memory:: Memory object
 * @param device_address the address of the eeprom
 * @param i2c the i2c bus the eeprom is connected to
*/
Memory::Memory(unsigned int device_address, i2c_inst_t* i2c)
{
    // set the private variables
    this->write_cycle_time = DEFAULT_EEPROM_WRITE_CYCLE;
    this->i2c = i2c;
    this->eeprom_addr = device_address;
}

/**
 * @brief Verify that the eeprom is connected to the i2c bus
*/
bool Memory::verifyConnection()
{
    // check if we get a response from the eeprom by dummy writing to it
    unsigned char data;
    // if the dummy write fails, it should return a negative number
    int ret = i2c_read_blocking(this->i2c, this->eeprom_addr, &data, 1, false);
    return !(ret < 0);
}

/**
 * @brief Run a self test on the eeprom
 * @return The number of errors found, 0 if no errors
 * @note This works by reading the contents and saving it to a local variable, then overwriting the EEPROM and verifying that it can be read back. It ends with rewriting back the original data.
*/
int Memory::selfTest()
{
    // store the current address
    unsigned int address = 0;
    // store the error counter
    int error_counter = 0;
    // since the EEPROM has 512 bytes, and each word is 4 bytes, we need to check 128 words
    for(int i = 0; i < (DEFAULT_EEPROM_SIZE / EEPROM_WORD_SIZE); i++)
    {
        // read the current word and store it for rewriting later
        unsigned int original_word = this->readWord(address);
        // calculate a "random" word using the us timer and the current address (lmao this is not random at all)
        unsigned int random_word = (unsigned int)time_us_64() + address;
        // write the random word to the eeprom
        this->writeWord(address, random_word);
        // read the word back from the eeprom
        unsigned int read_word = this->readWord(address);
        // compare the word written to the word read
        if(read_word != random_word)
        {
            // if the words don't match, increment the error counter
            error_counter++;
        }
        // write the original word back to the eeprom
        this->writeWord(address, original_word);
        // increment the address
        address += EEPROM_WORD_SIZE;
    }

    // return the error counter
    return error_counter;
}


/**
 * @brief read a word from the eeprom
 * @param target_address the address to read from
 * @return the word read from the eeprom
 * @note as the RP2040 is a 32 bit microcontroller, a word is 4 bytes
*/
unsigned int Memory::readWord(unsigned int target_address)
{
    // read 4 bytes from the eeprom
    unsigned char data[4];
    unsigned char _device_address = ((this->eeprom_addr) | ((target_address >> 8) & 0x07)) & 0xFF;
    unsigned char _target_address = target_address & 0xFF;
    // set the target address to read from
    i2c_write_blocking(this->i2c, _device_address, &_target_address, 1, true);
    // read the data
    i2c_read_blocking(this->i2c, _device_address, data, 4, false);
    // convert the data to a word
    unsigned int word = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    return word;
}

/**
 * @brief write a word to the eeprom
 * @param target_address the address to write to
 * @param data the word to write to the eeprom
 * @note as the RP2040 is a 32 bit microcontroller, a word is 4 bytes
*/
void Memory::writeWord(unsigned int target_address, unsigned int data)
{
    // write the bytes to the eeprom
    unsigned char _device_address = ((this->eeprom_addr) | ((target_address >> 8) & 0x07)) & 0xFF;
    unsigned char _target_address = target_address & 0xFF;

    // create a new buffer storing the data in
    unsigned char buffer[5] = 
    { 
        _target_address, 
        (unsigned char)((data >> 24) & 0xFF), 
        (unsigned char)((data >> 16) & 0xFF), 
        (unsigned char)((data >> 8) & 0xFF), 
        (unsigned char)(data & 0xFF)
    };

    // write the data to the eeprom
    i2c_write_blocking(this->i2c, _device_address, buffer, 5, false);

    // wait for the write cycle to finish
    sleep_ms(this->write_cycle_time);
}