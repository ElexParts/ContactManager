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
    
    // Test sending an SMS message.
    if (sms.SendSMS("639000000000", "A text message to verify if GSM shield is working.")) {
      Serial.println("\nSMS sent OK");
    }
  }
};

void loop() {
  char position;
  char phone_num[20]; // array for the phone number string
  char sms_text[100]; // array for the SMS text string
  
  if(started) {
    position = sms.IsSMSPresent(SMS_UNREAD);
    if (position) {
      // there is new SMS => read it
      sms.GetSMS(position, phone_num, sms_text, 100);
      #ifdef DEBUG_PRINT
        gsm.DebugPrint("DEBUG SMS phone number: ", 0);
        gsm.DebugPrint(phone_num, 0);
        gsm.DebugPrint("\r\n          SMS text: ", 0);
        gsm.DebugPrint(sms_text, 1);
      #endif
    }

    delay(15000);
  }
};