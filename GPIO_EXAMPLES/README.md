#Install GPIOD
sudo apt install libgpiod-dev

To link against libgpiod (or any other library), you need to ensure that any library 
references come after your source files (symbols are resolved from left to right):
	gcc -o gpiod_test gpiod_test.c -lgpiod
#Key Concepts

    GPIO Chip: A GPIO chip is a hardware component that provides multiple GPIO lines. On a Raspberry Pi, 
	the GPIO chip is typically represented by files in /dev such as /dev/gpiochip0.

    GPIO Line: A GPIO line is a single GPIO pin provided by a GPIO chip. Each line can be configured 
as an input or output, and its state can be read or set.

    Line Offset: This is the index or number that identifies a specific GPIO line within a GPIO chip. 
	For example, if a GPIO chip has 40 GPIO lines, the offsets will range from 0 to 39.