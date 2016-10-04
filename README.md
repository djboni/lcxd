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

Code examples are in the `doc` directory. There is a list of the examples at the
end of this file.


## Contributing to LCxD

If you have suggestions for improving LCxD, please
[open an issue or pull request on GitHub](https://github.com/djboni/lcxd).


## Important files

[README.md](https://github.com/djboni/lcxd/blob/master/README.md)
Fast introduction (this file).

[doc/LICENCE](https://github.com/djboni/lcxd/blob/master/doc/LICENSE)
Complete license text.

[doc/ExampleIO.md](https://github.com/djboni/lcxd/blob/master/doc/ExampleIO.md)
Example using direct IOs.

[doc/ExampleI2C.md](https://github.com/djboni/lcxd/blob/master/doc/ExampleI2C.md)
Example using PCF8574 I2C expander.
