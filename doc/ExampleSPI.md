# LCxD - ExampleSPI


Example using 74HC595 shift register.

The example shows an implementation using
[Arduinutil](https://github.com/djboni/arduinutil) as development platform.


```c
#include "Arduinutil.h"
#include "lcd.h"

void Lcd_595write(const uint8_t *buff, uint8_t length)
{
    const uint8_t ss_pin = 10;
	
    pinMode(ss_pin, OUTPUT);
    digitalWrite(ss_pin, LOW);

    Spi_transmit(buff, length);
    while(Spi_getStatus() != 0) {}

    digitalWrite(ss_pin, HIGH);
}

/* LCD pin data. */
uint8_t lcd_pins = 0xFF; /* LED=1, D4-7=1, E=1, RS=1, LCD_VCC=1 */

/* Converts LCxD pins to the shift register pins and write the values to LCD
pins when E/LCD_VCC is changed.

  Param  LCD  Convert  74HC595
    pin  pin    to     pin
      0  DB4    ->     6 QG
      1  DB5    ->     5 QF
      2  DB6    ->     4 QE
      3  DB7    ->     3 QD
      4  E      ->     2 QC
      5  RS     ->     1 QB
      6  RW     ->     - --  (not used in LCxD)
      7  LED    ->     7 QH  (not used in LCxD)
      8  LCD_VCC ->    0 QA  (not used in LCxD)
*/
void Lcd_pinsWrite(uint8_t data_00ER7654) {
{
    uint8_t data;
    CRITICAL_VAL();

    /* Reorder bits: DB4, DB5, DB6, DB7, E, RS */
    data = ((data_00ER7654 << 6) & 0x40) | ((data_00ER7654 << 4) & 0x20)
        | ((data_00ER7654 << 2) & 0x10) | (data_00ER7654 & 0x08)
        | ((data_00ER7654 >> 2) & 0x04) | ((data_00ER7654 >> 4) & 0x02);

    CRITICAL_ENTER();
    data = (lcd_pins & 0x81) | (data & 0x7E);
    lcd_pins = data;
    CRITICAL_EXIT();
    
    Lcd_595write(&data, 1);
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

    Spi_begin(PRESCALER);

    Lcd_init(2, 16); /* LCD 1602 */

    Lcd_blink();
    Lcd_write("LCxD");

    for(;;) {}
}
```
