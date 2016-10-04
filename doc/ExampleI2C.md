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

/* Converts LCxD pins to the I2C expander pins and write the values to LCD pins
 when E is changed.

  Param  LCD  Convert  PCF8574
    pin  pin    to     pin
      0  D4     ->     P4
      1  D5     ->     P5
      2  D6     ->     P6
      3  D7     ->     P7
      4  E      ->     P2
      5  RS     ->     P0
      6  RW     ->     P1  (not used in LCxD)
      7  LED    ->     P3  (not used in LCxD)
*/
void Lcd_pinWrite(uint8_t pin, uint8_t value)
{
    static uint8_t pins = 0xFD; /* D7-4=1, LED=1, E=1, RW=0, RW=1 */
    uint8_t buff[2];

    switch(pin) {
        case 0: /* D4 */
        case 1: /* D5 */
        case 2: /* D6 */
        case 3: /* D7 */
            pin += 4;
            break;
        case 4: /* E */
            pin = 2;
            break;
        case 5: /* RS */
            pin = 0;
            break;
        case 6: /* RW */
            pin = 1;
            break;
        case 7: /* LED */
            pin = 3;
            break;
        default:
            ASSERT(0); /* Invalid pin. */
    }

    if(value == 0)
    {
        /* Possible E falling edge. Data is read by the LCD in this edge. First
         send data with high E and then send data with low E. */
        buff[0] = pins;
        pins &= ~(1 << pin);
        buff[1] = pins;

        if(pin == 2)
            Lcd_8574write(buff, 2);
    }
    else
    {
        /* Possible E rising edge. Data is not read by the LCD in this edge.
         Just send the data with high E. */
        pins |= (1 << pin);
        buff[0] = pins;

        if(pin == 2)
            Lcd_8574write(buff, 1);
    }
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
