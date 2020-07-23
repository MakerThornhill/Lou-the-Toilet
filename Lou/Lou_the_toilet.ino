
/*----------------------------------------
    Lou the Toilet -- by Thornhill!
  ----------------------------------------

  Minature toilet with motorised lid and NeoPixel. Connects to Adafruit IO 
  and the Bathroom status feed to indicate whether the bathroom is occupied
  or not. 

  Board:    Adafruit ItsyBitsy 32u4 5v
  Airlift:  Adafruit AirLift – ESP32 WiFi Co-Processor Breakout Board (https://www.adafruit.com/product/4201)

  Other:    RadioShack Digital Micro servo
            NeoPixel Mini Button PCB


************************* Configuration ***********************************

  // edit the config.h tab and enter your Adafruit IO credentials
  // and any additional configuration needed for WiFi, cellular,
  // or ethernet clients.

****************************************************************************/
#include "config.h"
#include <Adafruit_NeoPixel.h>
#include <Servo.h>

//Pins
#define SERVO_PIN     6
#define PIXEL_PIN     8
#define PIXEL_COUNT   1
#define PIXEL_TYPE    NEO_GRB + NEO_KHZ800

//Initial servo position for toilet lid, almost fully upright
byte servo_pos = 10;
byte prev_pos = 10;
byte pos;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
Servo myservo;

AdafruitIO_Feed *bathroom_status = io.feed("status");

void setup() {
  Serial.begin(115200);

  //Add neopixel
  pixels.begin();
  pixels.clear();
  pixels.show();
  
  //Change to blue to let us know it's ready to connect
  pixels.setPixelColor(0, pixels.Color(0, 0, 150));
  pixels.show();

  //************ Connect to Adafruit IO ************//
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  bathroom_status->onMessage(handleMessage);
  
  // Try to connect
  while (io.status() < AIO_CONNECTED) {
    //A pink-magenta colour lets us know it's having some problems
    pixels.setPixelColor(0, pixels.Color(150, 0, 150));
    pixels.show();
    delay(1000);
    Serial.print("...");
    Serial.print(io.statusText());
    pixels.clear();
    pixels.show();
    delay(1000);
  }

  // We are connected
  Serial.println();
  Serial.println(io.statusText());

  bathroom_status->get();

  //White colour because everything is dandy
  pixels.setPixelColor(0, pixels.Color(150, 150, 150));
  pixels.show();

  //*************************************//

  myservo.attach(SERVO_PIN);
  myservo.write(10);

  pixels.clear();

}

void loop() {
  io.run();
}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("Received: ");
  Serial.println(data->value());

  
  //Convert received data to an Int
  int toilet_reading = data->toInt();

  switch (toilet_reading) {
    case 0: //Not free
      servo_pos = 50; //Open enough to see the stop sign
      pixels.setPixelColor(0, pixels.Color(255, 0, 1));
      break;
    case 1: //Maybe free
      servo_pos = 70; //Midway
      pixels.setPixelColor(0, pixels.Color(255, 75, 0));
      break;
    case 2://Free!
      servo_pos = 97; //Almost shut... enough for that sweet green glow
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      break;
  }

  pixels.show();
  
  //To decrease the servo position (i.e. servo is at 45... but needs to be 0...)
  if (servo_pos < prev_pos) { //  i.e. servo is at 45... but needs to be 0....
    for (pos = prev_pos; pos >= servo_pos; pos -= 1) { 
      myservo.write(pos);
      delay(75);
    }
  }
  //To increase the servo positoon (i.e. servo is at 0... but needs to be 45...)
  else if (servo_pos > prev_pos) { 
    for (pos = prev_pos; pos <= servo_pos; pos += 1) { 
      myservo.write(pos);
      delay(75);
    }
  }
  else {
  }
  prev_pos = servo_pos;

}
