#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "led_matrix.h"

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\r\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("TP-LINK_2.4GHz_F69944", "514AHUCX");
    WiFiMulti.addAP("NIDIAO", "87654321");

}

void loop() {
    StaticJsonBuffer<3000> jsonBuffer;

    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.printf("[HTTP] begin...\r\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://www.weather.com.cn/adat/sk/101220101.html"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\r\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\r\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);

                JsonObject& weather = jsonBuffer.parseObject(payload);
                if (!weather.success()) {
                    USE_SERIAL.println("parseObject() failed");
                    return;
                }
                int rain = weather["weatherinfo"]["rain"];
                const char *wind = weather["weatherinfo"]["WD"];
                led_matrix_clear_buffer();
                led_matrix_display_hefei();
                led_matrix_display();
                led_matrix_display_rain(rain);
                led_matrix_display();
                led_matrix_display_wind((char *)wind);
                led_matrix_display();
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\r\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    delay(3000);delay(3000);delay(3000);delay(3000);delay(3000);delay(3000);delay(3000);delay(3000);
    USE_SERIAL.println("One Cycle");
}
