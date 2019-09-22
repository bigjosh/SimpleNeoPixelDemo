#include "SimpleNeoPixel.h"

/*

  That is the whole API. What follows are some demo functions rewriten from the AdaFruit strandtest code...

  https://github.com/adafruit/Adafruit_NeoPixel/blob/master/examples/strandtest/strandtest.ino

  Note that we always turn off interrupts while we are sending pixels becuase an interupt
  could happen just when we were in the middle of somehting time sensitive.

  If we wanted to minimize the time interrupts were off, we could instead
  could get away with only turning off interrupts just for the very brief moment
  when we are actually sending a 0 bit (~1us), as long as we were sure that the total time
  taken by any interrupts + the time in our pixel generation code never exceeded the reset time (5us).

*/


// Display a single color on the whole string

void showColor( unsigned char r , unsigned char g , unsigned char b ) {

  cli();
  for( int p=0; p<PIXELS; p++ ) {
    sendPixel( r , g , b );
  }
  sei();
  show();

}

// Fill the dots one after the other with a color
// rewrite to lift the compare out of the loop
void colorWipe(unsigned char r , unsigned char g, unsigned char b, unsigned  char wait ) {
  for(unsigned int i=0; i<PIXELS; i+= (PIXELS/60) ) {

    cli();
    unsigned int p=0;

    while (p++<=i) {
        sendPixel(r,g,b);
    }

    while (p++<=PIXELS) {
        sendPixel(0,0,0);

    }

    sei();
    show();
    delay(wait);
  }
}

// Theatre-style crawling lights.
// Changes spacing to be dynmaic based on string size

#define THEATER_SPACING (PIXELS/20)

void theaterChase( unsigned char r , unsigned char g, unsigned char b, unsigned char wait ) {

  for (int j=0; j< 3 ; j++) {

    for (int q=0; q < THEATER_SPACING ; q++) {

      unsigned int step=0;

      cli();

      for (int i=0; i < PIXELS ; i++) {

        if (step==q) {

          sendPixel( r , g , b );

        } else {

          sendPixel( 0 , 0 , 0 );

        }

        step++;

        if (step==THEATER_SPACING) step =0;

      }

      sei();

      show();
      delay(wait);

    }

  }

}



// I rewrite this one from scrtach to use high resolution for the color wheel to look nicer on a *much* bigger string

void rainbowCycle(unsigned char frames , unsigned int frameAdvance, unsigned int pixelAdvance ) {

  // Hue is a number between 0 and 3*256 than defines a mix of r->g->b where
  // hue of 0 = Full red
  // hue of 128 = 1/2 red and 1/2 green
  // hue of 256 = Full Green
  // hue of 384 = 1/2 green and 1/2 blue
  // ...

  unsigned int firstPixelHue = 0;     // Color for the first pixel in the string

  for(unsigned int j=0; j<frames; j++) {

    unsigned int currentPixelHue = firstPixelHue;

    cli();

    for(unsigned int i=0; i< PIXELS; i++) {

      if (currentPixelHue>=(3*256)) {                  // Normalize back down incase we incremented and overflowed
        currentPixelHue -= (3*256);
      }

      unsigned char phase = currentPixelHue >> 8;
      unsigned char step = currentPixelHue & 0xff;

      switch (phase) {

        case 0:
          sendPixel( ~step , step ,  0 );
          break;

        case 1:
          sendPixel( 0 , ~step , step );
          break;

        case 2:
          sendPixel(  step ,0 , ~step );
          break;

      }

      currentPixelHue+=pixelAdvance;


    }

    sei();

    show();

    firstPixelHue += frameAdvance;

  }
}


// I added this one just to demonstrate how quickly you can flash the string.
// Flashes get faster and faster until *boom* and fade to black.

void detonate( unsigned char r , unsigned char g , unsigned char b , unsigned int startdelayms) {
  while (startdelayms) {

    showColor( r , g , b );      // Flash the color
    showColor( 0 , 0 , 0 );

    delay( startdelayms );

    startdelayms =  ( startdelayms * 4 ) / 5 ;           // delay between flashes is halved each time until zero

  }

  // Then we fade to black....

  for( int fade=256; fade>0; fade-- ) {

    showColor( (r * fade) / 256 ,(g*fade) /256 , (b*fade)/256 );

  }

  showColor( 0 , 0 , 0 );


}

void setup() {

  ledsetup();

}


void loop() {

  // Some example procedures showing how to display to the pixels:
  colorWipe(255, 0, 0, 0); // Red
  colorWipe(0, 255, 0, 0); // Green
  colorWipe(0, 0, 255, 0); // Blue

  // Send a theater pixel chase in...
  theaterChase(127, 127, 127, 0); // White
  theaterChase(127,   0,   0, 0); // Red
  theaterChase(  0,   0, 127, 0); // Blue

  rainbowCycle(1000 , 20 , 5 );
  detonate( 255 , 255 , 255 , 1000);

  return;

}
