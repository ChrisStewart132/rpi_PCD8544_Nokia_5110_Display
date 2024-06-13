# RASPBERRY PI 3B+ PROGRAM TO INTERFACE AND CONTROL A NOKIA 5110 LCD
Program Using the Linux SPIDEV character driver and the libgpiod GPIO library to control a NOKIA 5110 LCD screen

# DEMO
https://github.com/ChrisStewart132/rpi_PCD8544_Nokia_5110_Display/assets/30304173/fb5c2923-b809-49ff-a8f8-25c49b45d450

# Nokia 5110
Nokia 5110 is a Nokia GSM mobile phone model. It was released in 1998. At one point, it was said to be the most popular mobile telephone in the world. 

# REQUIREMENTS TO RUN
Requires a raspberry pi computer running bullseye or bookworm os, Nokia 5110 LCD, gpiod library, and a rpi camera.

# HOW TO RUN
Assuming you have wired your SPI interface to the Nokia 5110 with the 2 GPIO (d/c and rst) pins correctly connected.
With a cmd line open in a directory:
```
git clone https://github.com/ChrisStewart132/rpi_PCD8544_Nokia_5110_Display.git
```
Navigate into the cloned directory:
```
cd rpi_PCD8544_Nokia_5110_Display
```
install libgpiod
```
sudo apt install libgpiod-dev
```
Compile: 
```
gcc -o YUV420_to_binary_threshold YUV420_to_binary_threshold.c
gcc -o Nokia_5110_bitmap_stream_from_stdin_V2 Nokia_5110_bitmap_stream_from_stdin_V2.c -lgpiod
```
Run
```
rpicam-vid -n -t 0 --framerate 30 --width 128 --height 160 --codec yuv420 -o - | ./YUV420_to_binary_threshold | ./Nokia_5110_bitmap_stream_from_stdin_V2
```
