# SimpleNeoPixelDemo

A demonstration showing how easy it can be to drive WS2812 NeoPixels.

The code sends bits and bytes directly to the LED strip without allocating memory for them,
which allows to **control hundreds of pixels** with e.g. a small ATtiny85.
The [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library allocates 4 bytes
per LED, and an ATtiny85 can only control a bit more than 50 pixels.

More info at:
[NeoPixels Revealed: How to (not need to) generate precisely timed signals](http://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/)
