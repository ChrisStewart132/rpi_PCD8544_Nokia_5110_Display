import time
import picamera
from io import BytesIO

# Set up the camera
with picamera.PiCamera() as camera:
    # Set the resolution to 48x84
    camera.resolution = (96, 48)
    camera.framerate = 24
    camera.rotation = 180
    camera.color_effects = (128,128)
    # Allow the camera to warm up
    time.sleep(2)
    camera.start_preview()
    time.sleep(1)
    camera.stop_preview()

    # Capture an image
    #camera.capture('custom_res_image.jpg')
    #camera.capture('raw_image.bin', format='rgb')
    
    rgb_stream = BytesIO()
    camera.capture(rgb_stream, format='rgb')
    buffer_size = rgb_stream.tell()
    print("rgb len:", buffer_size)
    
    
    
    # todo convert from 96x48 to 84x48
    rgb_stream.seek(0,0)
    threshold = 127
    bitmap_arr = []
    alpha_arr = []
    while rgb_stream.tell() != buffer_size:
        rgb = rgb_stream.read(3)
        alpha = sum(rgb)//3
        alpha_arr.append(alpha)
        bitmap_arr.append(alpha > threshold)
        
    alpha_stream = BytesIO()
    alpha_stream.write(bytes(alpha_arr))
    print("alpha len:", alpha_stream.tell())
    
    bitmap_stream = BytesIO()
    bitmap_stream.write(bytes(bitmap_arr))
    print("bitmap len:", bitmap_stream.tell())
    

    with open('rgb.bin', 'wb') as file:
        rgb_stream.seek(0,0)
        file.write(rgb_stream.read())
    with open('alpha.bin', 'wb') as file:
        alpha_stream.seek(0,0)
        file.write(alpha_stream.read())
    with open('bitmap.bin', 'wb') as file:
        bitmap_stream.seek(0,0)
        file.write(bitmap_stream.read())
    
    rgb_stream.close()
    alpha_stream.close()
    bitmap_stream.close()
    print("Image captured", camera.resolution )

