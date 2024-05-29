import time
import picamera
from io import BytesIO
from sys import stdout
'''
    program using the rpi 3b+ and picamera v1.3
    to capture a raw rgb image in grayscale, 
    convert it to an 84x48 bitmap via binary thresholding,
    and stdout the bitmap buffer (84x48) = 4032 bytes each with a value of 0 or 1

    python3 picamera_capture_to_stdout.py | ./Nokia_5110_bitmap_from_stdin
'''
THRESHOLD = 127

with picamera.PiCamera() as camera:     # Set up the camera
    camera.resolution = (96, 48)        # Set to minimum resolution 96,48
    camera.framerate = 24
    camera.rotation = 180               # flip camera view
    camera.color_effects = (128,128)    # grayscale
    time.sleep(2)                       # Allow the camera to warm up
    camera.start_preview()
    time.sleep(1)
    camera.stop_preview()

    rgb_stream = BytesIO()
    camera.capture(rgb_stream, format='rgb')
    buffer_size = rgb_stream.tell()
    rgb_stream.seek(0,0)
    
    bitmap_bytes = bytearray()
    x = 0
    while rgb_stream.tell() != buffer_size:
        rgb = rgb_stream.read(3)            # rgb = bytes([r,g,b])
        if x % camera.resolution[0] < 84:   # only save 0->83 pixels in each row
            alpha = sum(rgb)//3
            bitmap_bytes.append(alpha > THRESHOLD)
        x += 1

    stdout.write(bitmap_bytes.decode())
    rgb_stream.close()