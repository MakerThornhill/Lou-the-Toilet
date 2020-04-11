/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "EXAMPLE_USER"
#define IO_KEY       "EXAMPLE_KEY"  

/******************************* WIFI **************************************/

#define WIFI_SSID "EXAMPLE_SSID"
#define WIFI_PASS "EXAMPLE_PASS"

// uncomment the following line if you are using airlift
#define USE_AIRLIFT
#include "AdafruitIO_WiFi.h"

#if defined(USE_AIRLIFT)
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS    10   // Chip select pin
  #define ESP32_RESETN  11   // Reset pin
  #define SPIWIFI_ACK   12   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#endif

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS, SPIWIFI_SS,
                   SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
