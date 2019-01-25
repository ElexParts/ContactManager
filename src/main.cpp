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

boolean debug = true; // Set flag for debugging.

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
    if (sms.SendSMS("+639XXXXXXXXX", "GSM Module is initialized.")) {
      Serial.println("\nSMS sent OK");
    }
  }
};

void loop() {
  char position;
  char phone_num[14]; // array for the phone number string
  char sms_text[100]; // array for the SMS text string
  
  if(started) {
    position = sms.IsSMSPresent(SMS_UNREAD);
    Serial.println(position);
    if (position > 0) {
      // Get the unread message.
      sms.GetSMS(position, phone_num, sms_text, 100);
      if (debug) {
        // Send an auto-reply message.
        if (sms.SendSMS(phone_num, "Thanks for contacting us. We'll review your message and get in touch with you shortly.")) {
          Serial.println("\nAuto-reply message is sent.");
        }

        // Print information of the sender.
        Serial.println("Phone number: ");
        Serial.println(phone_num);
        Serial.println("SMS text: ");
        Serial.println(sms_text);
      }
    }

    delay(10000);
  }
};