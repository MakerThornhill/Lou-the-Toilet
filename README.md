# Lou-the-Toilet
Cute as a button, this lil' internet-connected, miniature toilet model sits on my desk and lets me know when the house's only bathroom is free.

Equipped with a servo, a neopixel, and a ESP32 breakout board, Lou the Toilet glows different colours and raises and lowers its lid to indicate if the bathroom is occupied.

Powered by Arduino and using Adafruit's IO service (https://io.adafruit.com/), Lou uses a separate internet-connected microcontroller, a magnetic door switch, and a light sensor to determine whether the bathroom is free and clear!

More project details: https://hackaday.io/project/170829-lou-the-toilet

![Lou the Toilet - full model](https://github.com/MakerThornhill/Lou-the-Toilet/blob/master/Images/Lou_full-model_gif_low.gif "Lou the Toilet - full model")




## The Project
**Lou the Toilet** is a 3D printed miniature toilet that uses an [Adafruit ItsyBitsy 32u4 5V](https://www.adafruit.com/product/3677), a miniature servo, and a neopixel to indicate the status of the bathroom with a lifting and closing of it's lid and a coloured glow. An ESP32 breakout board (Adafruit AirLift – ESP32 WiFi Co-Processor) lets Lou connect to the internet and the Adafruit IO service to get updates about the bathroom's availability.

Just provide Lou with 5v and it's good to go, merrily opening and closing its lid all day!

**The Bathroom Occupancy Detector** covers the bathroom side, and is made up of an [Adafruit ItsyBitsy M0](https://www.adafruit.com/product/3727) with an ESP32 add-on (Airlift Bitsy), a magnetic door switch, and a light sensor. Together they monitor the state of the door (open or closed) and the bathroom light (off or on) to determine whether the bathroom is free. 

The bathroom status can be viewed anywhere from the house, using Lou or a private Adafruit IO dashboard.

## The Code
Lou and the Occupancy Dector use Arduino and the [Arduino Adafruit IO library](https://github.com/adafruit/Adafruit_IO_Arduino). Examples of how to use the Adafruit IO and it's libraries can be found in the [Adafruit IO learn guide](https://learn.adafruit.com/welcome-to-adafruit-io).

**Required Libraries:**
- [Arduino Adafruit IO library](https://github.com/adafruit/Adafruit_IO_Arduino).
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) 
- [Adafruit Dotstar](https://github.com/adafruit/Adafruit_DotStar)
- Servo

## 3D Model & Print files
Lou is based on a [GrabCad toilet model](https://grabcad.com/library/gustavsberg-artic-4300-1) by [Jari Ikonen](https://grabcad.com/jari.ikonen-2). I edited the design in Fusion 360 to fit the electronics and house the stop sign when the lid is closed.

Other parts, including the stop sign and case for the Occupancy Detector were designed by me. Print files and settings coming soon!
