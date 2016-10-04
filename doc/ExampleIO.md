# LCxD - ExampleIO


Example using direct IOs.

The example shows an implementation using
[Arduinutil](https://github.com/djboni/arduinutil) as development platform.


```c
#include "Arduinutil.h"
#include "lcd.h"

/* Converts LCxD pins to the Arduino pins and write value to LCD pins.

  Param  LCD  Convert  Arduino
    pin  pin    to     pin
      0  D4     ->     D4
      1  D5     ->     D5
      2  D6     ->     D6
      3  D7     ->     D7
      4  E      ->     D8
      5  RS     ->     D9
      -  RW     ->     GND
*/
void Lcd_pinWrite(uint8_t pin, uint8_t value)
{
    pin += 4; /* Convert. Just sum 4. */

    /* Write pin. */
    pinMode(pin, OUTPUT);
    digitalWrite(pin, value);
}

/* Delay if needed. */
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

    Lcd_init(2, 16); /* LCD 1602 */

    Lcd_blink();
    Lcd_write("LCxD");

    for(;;) {}
}
```
