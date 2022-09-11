#include <stdio.h>
#include <iostream>
#include <math.h>
#include "pico/stdlib.h"
#include "mcp4821.h"

using namespace std;

int main(void)
{
    stdio_init_all();
    const int FACTOR = 64;
    int sin_array[FACTOR];
    for (int i = 0; i < FACTOR; i++)
    {
        sin_array[i] = (int(0.8*2047 * (1 + sin(2 * 3.14 / FACTOR * i))));
    }
    MCP4821 adc;
    adc.begin();
    while (1)
    {
        for (int i = 0; i < FACTOR; i++)
        {
            adc.writeDAC_value(sin_array[i]);
        }
    }
}
