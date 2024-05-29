import time
import picamera
from io import BytesIO
import io
from sys import stdout
'''
    program using the rpi 3b+ and picamera v1.3
    to capture a raw rgb video in grayscale, 
    convert it to an 84x48 bitmap via binary thresholding,
    and stdout the bitmap buffer (84x48) = 4032 bytes each with a value of 0 or 1

    python3 picamera_stream_to_stdout.py | ./Nokia_5110_bitmap_stream_from_stdin
'''
THRESHOLD = 122

with picamera.PiCamera() as camera:     # Set up the camera
    camera.resolution = (96, 48)        # Set to minimum resolution 96,48
    FRAME_SIZE = camera.resolution[0]* camera.resolution[1] * 3
    camera.framerate = 24
    camera.rotation = 180               # flip camera view
    camera.color_effects = (128,128)    # grayscale
    time.sleep(2)                       # Allow the camera to warm up
    camera.start_preview()
    time.sleep(1)
    

    rgb_stream = BytesIO()
    camera.start_recording(rgb_stream, format='rgb')
    camera.wait_recording(1)
    frames_sent = 0
    while True:
        rgb_stream.seek(frames_sent*FRAME_SIZE, 0)
        rgb_frame = rgb_stream.read(FRAME_SIZE)
        rgb_stream.seek(0,io.SEEK_END)
        
        bitmap_bytes = bytearray()
        x = 0
        j = 0
        while x < FRAME_SIZE//3:#               convert from rgb to binary_byte
            if x % camera.resolution[0] < 84:   # only save 0->83 pixels in each row
                rgb = rgb_frame[j:j+3]
                alpha = 0.3*rgb[0] + 0.587*rgb[1] + 0.114*rgb[2]#sum(rgb)//3
                bitmap_bytes.append(alpha > THRESHOLD)
            x += 1
            j += 3
        stdout.write(bitmap_bytes.decode())
        frames_sent += 1
    
    camera.stop_preview()
    camera.stop_recording()
    rgb_stream.close()