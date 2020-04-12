/*----------------------------------------
    Bathroom Occupancy Detector -- by Thornhill!
  ----------------------------------------

   Bathroom occupancy detector, using a magnetic door switch and a light sensor.
   Pushed to Adafruit IO via an Adafruit ESP32 Airlift board.

   Assumes when door the is open and the light is off, the bathroom is free (which
   is how we roll in this household). When the door is open but the light is on,
   the bathroom might not be free. A closed door always assumes the bathroom is
   occupied.

   Board:     ItsyBitsy M0
   Airlift:   Airlift Bitsy (https://www.adafruit.com/product/4363)

   Sensors:
              Magnetic Door sensor (https://www.adafruit.com/product/375)
              ALS PT19 Light sensor (https://www.adafruit.com/product/2748)


    Based on https://learn.adafruit.com/using-ifttt-with-adafruit-io



  /************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
/****************************************************************************/

#include <Adafruit_DotStar.h>

//Pins
#define DOOR_PIN        7
#define LIGHT_PIN       A1
#define DATAPIN         41 //for the ItsyBitsy's onboard Dotstar
#define CLOCKPIN        40 //for the ItsyBitsy's onboard Dotstar

//Settings
#define LIGHT_THRESHOLD 60 //Trigger threshold for light level
#define IO_LOOP_DELAY   5000
#define NUMPIXELS       1 //number of dotstars

#define OPEN    0
#define CLOSED  1
#define ON      1
#define OFF     0

unsigned long lastUpdate = 0;
unsigned long lastStateChange = 0;
unsigned long durationSecs;

byte door_state;
byte light_state;
byte status_num;
String status_string = "***** Standby! *****";

char *lightStates[] = {"Off", "On"};
char *doorStates[] = {"Open", "Closed"};

byte prev_state;
byte current_state;

byte prev_door;
byte current_door;

byte prev_light;
byte current_light;

Adafruit_DotStar dot(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

//The various Adafruit IO feeds we want to publish.
//We definitely need the Status feed, the others are for completeness. 

AdafruitIO_Feed *bathroom_status = io.feed("status");
AdafruitIO_Feed *door = io.feed("toilet-or-not-toilet.door");
AdafruitIO_Feed *light = io.feed("toilet-or-not-toilet.light");
AdafruitIO_Feed *bathroom_status_string = io.feed("toilet-or-not-toilet.status-string");
AdafruitIO_Feed *duration = io.feed("toilet-or-not-toilet.duration");


void setup() {
  Serial.begin(115200);

  //Initialise the dotstar
  dot.begin(); 
  dot.setBrightness(80);
  dot.show();
  
  //Blue colour to show we'r going to connect
  dot.setPixelColor(0, 0, 0, 255);
  dot.show();

  //********* Connect to Adafruit IO **********

  Serial.print("Connecting to Adafruit IO... ");
  io.connect();

  //Wait for connection... and flash a magenta colour
  //if we need to really wait... 
  while (io.status() < AIO_CONNECTED) {
    dot.setPixelColor(0, 0, 150, 150);
    dot.show();
    delay(1000);
    Serial.print("StatusText:");
    Serial.print(io.statusText());
    Serial.println("..");
    dot.clear();
    dot.show();
    delay(1000);
  }

  //Connected!
  Serial.print(io.statusText());
  Serial.println();
  setColor(0, 255, 0);//Green, all good!

  //***********************************

  //Set up pins

  pinMode(DOOR_PIN, INPUT_PULLUP);
  
}

void loop() {
  io.run();

  if (io.status() < AIO_CONNECTED) {
    setColor(255, 0, 0);
    connect_AIO();
  }

  //Check the door switch and light sensor
  sensors();

  //Conditions to determine occupancy based on
  //the sensor states.
  if ((door_state == 1 && light_state == 1) || (door_state == 1 && light_state == 0)) { //Door is closed, light is on/off
    status_num = 0;
    status_string = "Not free!";
  }
  else if ((door_state == 0 && light_state == 1)) {
    //Door is open, light is on..
    status_num = 1;
    status_string = "Maybe not free";
  }
  else if (door_state == 0 && light_state == 0) { //Door is open, light is off
    status_num = 2;
    status_string = "Free!";
  }

  current_state = status_num;

  //Check if enough time has passed and then check if there's been a change in state...
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    setColor(0, 255, 0);//Green, all good!

    if (prev_state != current_state) {
      setColor(0, 0, 255);
      unsigned long durationMillis = millis() - lastStateChange;
      durationSecs = durationMillis / 1000;

      Serial.print(status_string);
      Serial.print(" Last state change: ");
      Serial.print(durationSecs);
      Serial.print("s, ");
      Serial.print(durationMillis);
      Serial.println(" ms");

      //If the bathroom is currently free, then send the duration of
      //when it wasn't free. Otherwise just make the duration 0.
      if (current_state == 2) {
        push_to_AIO(durationSecs);
      }
      else {
        push_to_AIO(0);
      }

      prev_state = current_state;
      lastStateChange = millis();
    }
    lastUpdate = millis();
  }
}

void sensors() {

  //Reading the door sensor
  if (digitalRead(DOOR_PIN) == LOW) {
    door_state = CLOSED;
  }
  else {
    door_state = OPEN;
  }
  current_door = door_state;

  //Set up and read the light sensor
  int level_raw = analogRead(LIGHT_PIN); //Read light value

  if (level_raw > LIGHT_THRESHOLD) {
    light_state = ON; //light is on
  } else {
    light_state = OFF; //light if off
  }

  current_light = light_state;

  if ((prev_door != door_state) || (prev_light != light_state)) {
    Serial.println();
    Serial.print("Door: ");
    Serial.println(doorStates[door_state]);
    Serial.print("Light: ");
    Serial.print(lightStates[light_state]);
    Serial.print(" (raw) = ");
    Serial.print(level_raw);
    Serial.println();
  }
  prev_light = current_light;
  prev_door = current_door;
}

void push_to_AIO(float duration_) {
  bathroom_status->save(status_num);
  bathroom_status_string->save(status_string);
  door->save(door_state);
  light->save(light_state);
  duration->save(duration_);
  io.run();
}

void connect_AIO() {
  Serial.println();
  Serial.print("Reconnecting to Adafruit IO...");
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.print(io.statusText());
  setColor(255, 255, 255);
  Serial.println();
}

//Function to set the colour on both the dotstar and the airlift's 
//RGB led...
void setColor(int red, int green, int blue)
{
  WiFi.setLEDs(green, red, blue);
  dot.setPixelColor(0, green, red, blue);
  dot.show();
}
