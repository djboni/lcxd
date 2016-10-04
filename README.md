# [LCxD](https://github.com/djboni/lcxd)

by [Djones A. Boni](https://twitter.com/djonesboni)


Flexible 4-bit LCD library.

LCxD is flexible because it is not aware of the LCD interface, which could
be IO pins, SPI shift register, I2C IO expander... The user defines the pin
write function and the delay microsseconds function used by the library.

The pin write function is called with a pin number and the value to be
written. The pin number corresponds to one of the LCD pins instead of an
Arduino pin. The function should convert to the correct Arduino pin or other
interface.

You can use LCxD both for closed- and open-source projects. You are also
free to keep changes to this library to yourself. However we'll enjoy your
improvements and suggestions. `:-)`

You are free to copy, modify, and distribute LCxD with attribution under
the terms of the
[Apache License Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).
See the doc/LICENSE file for details.


## How to use LCxD

To use LCxD you need a pin write functions and a delay function.

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


## Contributing to LCxD

If you have suggestions for improving LCxD, please
[open an issue or pull request on GitHub](https://github.com/djboni/lcxd).



## Important files

[README.md](https://github.com/djboni/lcxd/blob/master/README.md)
Fast introduction (this file).

[doc/LICENCE](https://github.com/djboni/lcxd/blob/master/doc/LICENSE)
Complete license text.
