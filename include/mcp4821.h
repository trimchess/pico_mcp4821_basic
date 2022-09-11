#pragma once

#include "pico/stdlib.h"
#include <hardware/spi.h>
#include <stdio.h>

#define GAIN_1 0x01 //
#define GAIN_2 0x00

#define SCK 2  // MCP4821 Pin 4
#define MOSI 3 // MCP4821 Pin 4
#define CS 5   // MCP4821 Pin 2

#ifndef HIGH
#define HIGH (1)
#endif

#ifndef LOW
#define LOW (0)
#endif

#ifndef ON
#define ON (1)
#endif

#ifndef OFF
#define OFF (0)
#endif
class MCP4821
{
public:
    MCP4821();
    ~MCP4821();

    void begin();
    void begin(const uint8_t SPIID, const uint8_t clk, const uint8_t mosi, const uint8_t cs, const uint8_t gain, const ulong BAUDRATE = 100000);
    void writeDAC_value(int value);
    void set_gain(uint8_t gain);
    uint8_t get_gain(void);

private:
    void _writeDAC(int value);
    uint8_t _ss; // Slave-select pin
    uint8_t gain;
};
