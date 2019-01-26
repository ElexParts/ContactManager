/**
 * Send and Receive SMS with SIM900.
 * 
 * @author ElexParts<contact@elexparts.com>
 * @url https://www.elexparts.com
 */

#include <Arduino.h>
#include <SIM900.h>
#include <SoftwareSerial.h>
#include "inetGSM.h"
#include <sms.h>

InetGSM inet;
SMSGSM sms;

// InetGSM variables.
char msg[50];
int numdata;
char inSerial[50];
int i=0;

// Simple sketch to send and receive SMS.
// To change pins for Software Serial, use the two lines in GSM.cpp.
// _GSM_TXPIN_ was set to 2
// _GSM_RXPIN_ was set to 3
int gsmStatusLed = 12;

// Flag to check if GSM has started.
boolean started = false;

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

    // GPRS attach, put in order APN, username and password.
    // If no needed auth let them blank.
    // APN is from Sun Cellular Network, change it according
    // cellular network provider.
    if (inet.attachGPRS("minternet", "", "")) {
      Serial.println("status=ATTACHED");
    }
    else {
      Serial.println("status=ERROR");
    }
    delay(1000);

    // Read IP address.
    gsm.SimpleWriteln("AT+CIFSR");
    delay(5000);

    // Read until serial buffer is empty.
    gsm.WhileSimpleRead();

    // TCP Client GET, send a GET request to the server and
    // save the reply.
    numdata=inet.httpGET("www.google.com", 80, "/", msg, 50);

    // Print the results.
    Serial.println("\nNumber of data received:");
    Serial.println(numdata);
    Serial.println("\nData received:");
    Serial.println(msg);
    
    // Test sending an SMS message.
    // if (sms.SendSMS("+639XXXXXXXXX", "GSM Module is initialized.")) {
    //   Serial.println("\nSMS sent OK");
    // }
  }
};

/**
 * Serial hardware read.
 */
void serialhwread() {
  i=0;
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      inSerial[i]=(Serial.read());
      delay(10);
      i++;
    }

    inSerial[i]='\0';
    if(!strcmp(inSerial,"/END")) {
      Serial.println("_");
      inSerial[0]=0x1a;
      inSerial[1]='\0';
      gsm.SimpleWriteln(inSerial);
    }

    // Send a saved AT command using serial port.
    if(!strcmp(inSerial,"TEST")) {
      Serial.println("SIGNAL QUALITY");
      gsm.SimpleWriteln("AT+CSQ");
    }

    // Read last message saved.
    if(!strcmp(inSerial,"MSG")) {
      Serial.println(msg);
    } else {
      Serial.println(inSerial);
      gsm.SimpleWriteln(inSerial);
    }
    inSerial[0]='\0';
  }
}

/**
 * Serial software read.
 */
void serialswread() {
  gsm.SimpleRead();
}

/**
 * Send an auto-reply message.
 */
void autoReply() {
  char position;
  char phone_num[14]; // array for the phone number string
  char sms_text[100]; // array for the SMS text string
  
  if(started) {
    // Get position of latest unread SMS.
    position = sms.IsSMSPresent(SMS_ALL);
    // Serial.println(position);

    if (position) {
      // Get the unread message.
      sms.GetSMS(position, phone_num, sms_text, 100);

      // Send an auto-reply message.
      if (sms.SendSMS(phone_num, "Thanks for contacting us. We'll review your message and get in touch with you shortly.")) {
        Serial.println("\nAuto-reply message is sent.");
      }

      sms.DeleteSMS(position);
    }

    delay(1000);
  }
};

void loop() {
  // Read for new byte on serial hardware,
  // and write them on NewSoftSerial.
  serialhwread();

  // Read for new byte on NewSoftSerial.
  serialswread();

  // Send an auto-reply message.
  autoReply();
};