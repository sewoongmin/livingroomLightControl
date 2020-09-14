// Adafruit IO Subscription Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("light");
AdafruitIO_Feed *indicator = io.feed("indicator");
bool light_state1 = false, light_state2 = false;

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // start MQTT connection to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  counter->onMessage(handleMessage);

  // wait for an MQTT connection
  // NOTE: when blending the HTTP and MQTT API, always use the mqttStatus
  // method to check on MQTT connection status specifically
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  digitalWrite(D4, HIGH);
  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  counter->get();

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  if( light_state1 != digitalRead(D1)){
    light_state1 = digitalRead(D1);
    Serial.print("sending -> ");
    Serial.println(light_state1);
    if (digitalRead(D2) && digitalRead(D1))
      indicator->save(3);
    else if(digitalRead(D2) && !digitalRead(D1))
      indicator->save(2);
    else if(!digitalRead(D2) && digitalRead(D1))
      indicator->save(1);
    else
      indicator->save(0);
  }
  if( light_state2 != digitalRead(D2)){
    light_state2 = digitalRead(D2);
    Serial.print("sending -> ");
    Serial.println(light_state2);  
    if (digitalRead(D2) && digitalRead(D1))
      indicator->save(3);
    else if(digitalRead(D2) && !digitalRead(D1))
      indicator->save(2);
    else if(!digitalRead(D2) && digitalRead(D1))
      indicator->save(1);
    else
      indicator->save(0);
  }
}

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  int state = data->toInt();
  if(state == 1){
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
  }
  else if(state == 0){
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
  }
  else if(state == 2){
    digitalWrite(D2, HIGH);
    digitalWrite(D1, LOW);
  }
  else if(state == 3){
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
  }
}
