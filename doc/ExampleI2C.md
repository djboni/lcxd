# LCxD - ExampleI2C


Example using PCF8574 I2C expander.

The example shows an implementation using
[Arduinutil](https://github.com/djboni/arduinutil) as development platform.


```c
#include "Arduinutil.h"
#include "lcd.h"

void Lcd_8574write(const uint8_t *buff, uint8_t length)
{
    const uint8_t addr = 0x27;
    uint8_t num;

    I2c_write(addr, buff, length, &num);
    while(I2c_getStatus() != 0) {}
    if(num != length)
    {
        Serial_print("I2C> W num=%u (!=%u)\n", num, length);
    }
}

/* LCD pin data. */
uint8_t lcd_pins = 0xFD; /* D7-4=1, LED=1, E=1, RW=0, RS=1 */

/* Converts LCxD pins to the I2C expander pins and write the values to LCD pins
 when E is changed.

  Param  LCD  Convert  PCF8574
    pin  pin    to     pin
      0  DB4    ->     P4
      1  DB5    ->     P5
      2  DB6    ->     P6
      3  DB7    ->     P7
      4  E      ->     P2
      5  RS     ->     P0
      6  RW     ->     P1  (not used in LCxD)
      7  LED    ->     P3  (not used in LCxD)
*/
void Lcd_pinsWrite(uint8_t data_00ER7654)
{
    uint8_t data;
    CRITICAL_VAL();

    /* Reorder bits: DB7-DB4, E, RS */
    data = (data_00ER7654 << 4) | ((data_00ER7654 >> 2) & 0x04)
        | ((data_00ER7654 >> 4) & 0x01);

    CRITICAL_ENTER();
    data = (lcd_pins & 0x0A) | (data & 0xF5);
    lcd_pins = data;
    CRITICAL_EXIT();
    
    Lcd_8574write(&data, 1);
}

void Lcd_delayUs(uint32_t delay_us)
{
    static uint32_t block_until = 0;

    uint32_t now = timerCounts();
    int32_t diff = block_until - now;
    uint32_t delay = TIMER_US_TO_COUNT(delay_us);

    if(diff > 0)
    {
        delayCounts(diff);
        now = timerCounts();
    }

    block_until = now + delay;
}

int main(void)
{
    init();
    timerBegin();

    I2c_begin(100000);

    Lcd_init(2, 16); /* LCD 1602 */

    Lcd_blink();
    Lcd_write("LCxD");

    for(;;) {}
}
```
