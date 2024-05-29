# RASPBERRY PI 3B+ PROGRAM TO INTERFACE AND CONTROL A NOKIA 5110 LCD
Program Using the Linux SPIDEV character driver and the libgpiod GPIO library to control a NOKIA 5110 LCD screen

# DEMO
https://github.com/ChrisStewart132/rpi_PCD8544_Nokia_5110_Display/assets/30304173/fb5c2923-b809-49ff-a8f8-25c49b45d450

# Nokia 5110
Nokia 5110 is a Nokia GSM mobile phone model. It was released in 1998. At one point, it was said to be the most popular mobile telephone in the world. 

# REQUIREMENTS TO RUN
Requires a device (rpi 3b+) with an SPI interface and 2 spare GPIO pins, Nokia 5110 LCD, gpiod library, linux os (gpiod, spidev, C programs, gcc compiler), picameraV1 (rpi camera V1.3 hardware).

# HOW TO RUN
Assuming you have wired your SPI interface to the Nokia 5110 with the 2 GPIO (d/c and rst) pins correctly connected.
Compile Nokia_5110_bitmap_from_stdin_V2.c with gpiod installed on your linux device: 
```
gcc -o Nokia_5110_bitmap_stream_from_stdin_V2 Nokia_5110_bitmap_stream_from_stdin_V2.c -lgpiod
```
With python 3 installed:
```
python3 picamera_stream_to_stdout_V2.py | ./Nokia_5110_bitmap_stream_from_stdin_V2
```


# Linux/Rpi commands
```
ssh pi@192.168.1.65

// copy from remote -> local
scp username@from_host:file.txt /local/directory/

// copy from local -> remote
scp file.txt username@to_host:/remote/directory/
```
