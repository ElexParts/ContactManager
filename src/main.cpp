/**
 * Send and Receive SMS with SIM900.
 * 
 * @author ElexParts<contact@elexparts.com>
 * @url https://www.elexparts.com
 */

#include <Arduino.h>
#include <SIM900.h>
#include <SoftwareSerial.h>
#include <sms.h>

SMSGSM sms;

// Simple sketch to send and receive SMS.
// To change pins for Software Serial, use the two lines in GSM.cpp.
// _GSM_TXPIN_ was set to 2
// _GSM_RXPIN_ was set to 3
int gsmStatusLed = 12;
boolean started = false;
char smsbuffer[160];
char n[20];

void setup() {
  pinMode(gsmStatusLed, OUTPUT);

  // Serial connection.
  Serial.begin(9600);
  Serial.println("Initializing GSM Shield ...");

  // Start configuration of shield with baudrate (4800 or lower).
  if (gsm.begin(2400)) {
    Serial.println("\nGSM Status: READY");
    started = true;
  } else {
    Serial.println("\nGSM Status: IDLE");
  }

  if(started) {
    // Turn on LED when GSM is ready.
    digitalWrite(gsmStatusLed, HIGH);
  }
};

void loop() {
  if(started) {
    // Test sending an SMS message.
    if (sms.SendSMS("639000000000", "A text message to verify if GSM shield is working.")) {
      Serial.println("\nSMS sent OK");
    }

    // Read if there are messages on SIM card and print them.
    // if(gsm.readSMS(smsbuffer, 160, n, 20)) {
    //   Serial.println(n);
    //   Serial.println(smsbuffer);
    // }

    delay(1000);
  }
};