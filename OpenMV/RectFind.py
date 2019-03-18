# Find Rects Example
#
# This example shows off how to find rectangles in the image using the quad threshold
# detection code from our April Tags code. The quad threshold detection algorithm
# detects rectangles in an extremely robust way and is much better than Hough
# Transform based methods. For example, it can still detect rectangles even when lens
# distortion causes those rectangles to look bent. Rounded rectangles are no problem!
# (But, given this the code will also detect small radius circles too)...

import sensor, image, time # Time is not needed for implementation, just testing
import pyb

#Import LED setup, for testing no real use, yet
r_led = pyb.LED(1)

# Set up sensor and fps calculation
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # grayscale is faster (160x120 max on OpenMV-M7)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()

    # `threshold` below should be set to a high enough value to filter out noise
    # rectangles detected in the image which have low edge magnitudes. Rectangles
    # have larger edge magnitudes the larger and more contrasty they are...

    # For rectangles in img
    for r in img.find_rects(threshold = 10000): # Unknown threshold value, will need testing

        # Outline each rectangle found for ease of sight
        # This is a little messy as it encompasses the entire box
        img.draw_rectangle(r.rect(), color = (255, 0, 0))

        # Turn on LED if r.corners not empty
        #   Turns on but not off for some reason
        if r.corners():
            r_led.on()
        elif not r.corners():
            r_led.off()

        # For each corner
        # This give more accurate understanding of where box lies in image
        for p in r.corners():

            '''
            Can use this function to get an array of cordinates to send to a function
            that finds distance, or center point to know if robot is too far left or
            right of box
            '''
            # Print circle around corners for ease of sight
            img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))

            # Print corner cordinates.
            print(p)
        print(r)

    print("FPS %f" % clock.fps())
