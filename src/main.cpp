#include <Arduino.h>
#include <SIM900.h>
#include <SoftwareSerial.h>
//If not used, is better to exclude the HTTP library,
//for RAM saving.
//If your sketch reboots itself proprably you have finished,
//your memory available.
//#include "inetGSM.h"

//If you want to use the Arduino functions to manage SMS, uncomment the lines below.
#include <sms.h>
SMSGSM sms;

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to send and receive SMS.

int numdata;
boolean started=false;
char smsbuffer[160];
char n[20];

void setup()
{
    pinMode(12, OUTPUT);

    //Serial connection.
    Serial.begin(9600);
    Serial.println("GSM Shield testing.");
    //Start configuration of shield with baudrate.
    //For http uses is raccomanded to use 4800 or slower.
    if (gsm.begin(2400)) {
        Serial.println("\nstatus=READY");
        started=true;
    } else Serial.println("\nstatus=IDLE");

    if(started) {
        //Enable this two lines if you want to send an SMS.
        //if (sms.SendSMS("639XXXXXXXXX", "Arduino SMS"))
        //Serial.println("\nSMS sent OK");

        // Turn on LED when GSM is ready.
        digitalWrite(12, HIGH);
    }
};

void loop()
{
    char position;
    char phone_num[20]; // array for the phone number string
    char sms_text[100]; // array for the SMS text string

    if(started) {
      //Enable this two lines if you want to send an SMS.
      if (sms.SendSMS("639000000000", "Arduino SMS"))
      Serial.println("\nSMS sent OK");

      //Read if there are messages on SIM card and print them.
      // if(gsm.readSMS(smsbuffer, 160, n, 20)) {
          // Serial.println(n);
          // Serial.println(smsbuffer);
      // }

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

      delay(1000);
    }
};