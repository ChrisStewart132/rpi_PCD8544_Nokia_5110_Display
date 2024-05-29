# NOKIA_EXAMPLES
Example C programs run on a rpi 3b+ connected to a Nokia 5110 through SPI0 with 2 GPIO pins (d/c and RST)

# Nokia 5110 PCD8544 Controller
The Nokia 5110 is a popular monochrome LCD display module originally used in the Nokia 5110 mobile phone. It is controlled by the PCD8544 controller, a low-power CMOS LCD controller/driver designed to drive a graphic display of 48 rows and 84 columns. Here's a detailed description of the Nokia 5110 LCD with the PCD8544 controller:
Physical Characteristics

    Display Size: 48 rows x 84 columns (48x84 pixels).
    Dimensions: Approximately 45mm x 45mm x 3mm.
    Interface: SPI (Serial Peripheral Interface).

PCD8544 Controller Features

    Low Power Consumption:
        Ideal for battery-operated devices.
        Sleep mode reduces power consumption.

    Display Control:
        Can control 48 rows and 84 columns.
        Supports both normal and inverse video modes (pixels can be turned on or off).

    Operating Voltage:
        Typically operates at 3.3V, but can tolerate up to 5V.

    Command Set:
        A comprehensive set of commands to control various display parameters, including setting the display RAM, addressing modes, and contrast control.

    Display RAM:
        Internal 504-byte (48 rows x 84 columns / 8 bits per byte) RAM used to store pixel data.
        Data is written to RAM and the controller automatically refreshes the display.

    Communication Protocol:
        Uses SPI for communication, which includes pins for data input (DIN), clock (CLK), chip select (CS), data/command mode (D/C), and reset (RST).

Pin Configuration

    VCC: Power supply pin (typically 3.3V).
    GND: Ground pin.
    SCE (CS): Chip select, active low.
    RST: Reset, active low.
    D/C: Data/Command selection pin.
    DIN (MOSI): Data input.
    CLK (SCK): Clock input.
    LED: Backlight control (optional, depending on the module).

Typical Usage

    Initialization:
        Power up the module.
        Reset the controller.
        Configure the display (contrast, addressing mode, etc.).

    Writing Data:
        Set D/C pin to select data mode.
        Send data bytes to update the display RAM.

    Sending Commands:
        Set D/C pin to select command mode.
        Send command bytes to control display settings.