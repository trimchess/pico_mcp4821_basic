#include <iostream>
#include "mcp4821.h"


MCP4821::MCP4821()
{
    _ss = 5;
}

MCP4821::~MCP4821()
{
}

void MCP4821::begin()
{
    spi_init(spi0, 100 * 1000);          // Baudrate 100k
    gpio_set_function(2, GPIO_FUNC_SPI); // SCK
    gpio_set_function(3, GPIO_FUNC_SPI); //
    gpio_set_function(4, GPIO_FUNC_SPI);
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    gpio_init(_ss);
    gpio_set_dir(_ss, GPIO_OUT);
    gpio_put(_ss, HIGH);
    set_gain(0);
};

void MCP4821::begin(const uint8_t SPIID, const uint8_t clk, const uint8_t mosi, const uint8_t cs, const uint8_t gain, const ulong BAUDRATE)
{
    gpio_set_function(clk, GPIO_FUNC_SPI);  // SCK
    gpio_set_function(mosi, GPIO_FUNC_SPI); //
    if (SPIID == 0)
    {
        spi_init(spi0, BAUDRATE);                                       // Baudrate 100k
        spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    }
    else
    {
        spi_init(spi1, BAUDRATE);                                       // Baudrate 100k
        spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    }
    gpio_init(_ss);
    gpio_set_dir(_ss, GPIO_OUT);
    gpio_put(_ss, HIGH);
    set_gain(gain);
}

void MCP4821::writeDAC_value(int value)
{
    if ((value < 0) || (value > 4095))
    {
        return;
    }
    _writeDAC(value);
}

void MCP4821::set_gain(uint8_t gain_value)
{
    if (gain_value == 0)
    {
        //gain_shdown &= 0x10;
        gain = 0x10;
    }
    else
    {
        //gain_shdown |= 0x20;
        gain = 0x30;
    }
    
}

uint8_t MCP4821::get_gain(void)
{
    return gain;
}

/**
 *Write Command Register (WCR) for MCP4821
 *    ___________________________________________________________________
 *    |W-x|W-x|W-x|W-0 |W-x|W-x|W-x|W-x | W-x|W-x|W-x|W-x|W-x|W-x|W-x|W-x|
 *    |___|___|___|____|___|___|___|____|____|___|___|___|___|___|___|___|
 *    | 0 | — |GA |SHDN|D11|D10|D9 |D8  |  D7| D6| D5| D4| D3| D2| D1| D0|
 *    |___|___|___|____|___|___|___|____|____|___|___|___|___|___|___|___|
 *    |b15|   |   |    |   |   |   |    |    |   |   |   |   |   |   | b0|
 *    |___|___|___|____|___|___|___|____|____|___|___|___|___|___|___|___|
 *    W   Write
 *    bit 15 0 = Write to DAC register; 1 = Ignore this command
 *    bit 14 — Don not Care
 *    bit 13 GA: Output Gain Selection bit. 1 = 1x (VOUT = VREF * D/4096)
 *                                          0 = 2x (VOUT = 2 * VREF * D/4096), internal VREF = 2.048V
 *    bit 12 SHDN: Output Shutdown Control bit. 1 = Active mode operation. VOUT is available.
 *                                            0 = Shutdown the device. Analog output is not available.
 *                                            VOUT pin is connected to 500 ktypical)
 *    bit 11-0 D11:D0: DAC Input Data bits. Bit x is ignored.
 *    -x  undefined at power up
 *    -0  0 at power up
 *
 *    wBuff[0]: bit15...bit8 and gain/shut down
 *    wBuff[1]: bit7...bit0
 */

void MCP4821::_writeDAC(int value)
{
    uint8_t wBuff[2];
    wBuff[0] = ((value & 0x0F00) >> 8) | gain;
    wBuff[1] = (value & 0x00FF);
    gpio_put(_ss, LOW);
    sleep_us(1);
    int n = spi_write_blocking(spi0, wBuff, 2);
    sleep_us(1);
    gpio_put(_ss, HIGH);
}