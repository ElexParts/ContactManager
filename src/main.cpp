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
    if (inet.attachGPRS("http://globe.com.ph", "", "")) {
      Serial.println("GPRS Status: ATTACHED");
    }
    else {
      Serial.println("GPRS Status: ERROR");
    }
    delay(1000);

    // Read IP address.
    gsm.SimpleWriteln("AT+CIFSR");
    delay(3000);

    // Read until serial buffer is empty.
    gsm.WhileSimpleRead();
    
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
  }
};

/**
 * Save message details through a Contacts Service API.
 */
void saveMessageToContacts() {
  // Account credentials, we are using Basic Authentication for now.
  // USERNAME
  // PASSWORD

  // Base-encode credentials.
  // SECRET_CREDENTIAL

  // Get access token.
  numdata = inet.httpGET("dev-elexparts-api.pantheonsite.io", 80, "/rest/session/token", msg, 1000);

  // Print the results.
  // Serial.println("\nGET - Number of data received:");
  // Serial.println(numdata);
  // Serial.println("\nGET - Data received:");
  // Serial.println(msg);

  /**
  HTTP/1.1 200 OK
  Cache-Control: must-revalidate, no-cache, private
  Content-Language: en
  Expires: Sun, 19 Nov 1978 05:00:00 GMT
  Server: nginx
  X-Content-Type-Options: nosniff
  X-Debug-Token: e29f2c
  X-Debug-Token-Link: /admin/reports/profiler/view/e29f2c
  X-Frame-Options: SAMEORIGIN
  X-Generator: Drupal 8 (https://www.drupal.org)
  X-Pantheon-Styx-Hostname: styx-fe3-a-5794d5fd7-qpvrq
  X-Styx-Req-Id: styx-24f5a9ba846f9378b2cb5a6c4998e4a1
  X-Ua-Compatible: IE=edge
  Accept-Ranges: bytes
  Accept-Ranges: bytes
  Via: 1.1 varnish
  Age: 0
  Accept-Ranges: bytes
  Accept-Ranges: bytes
  Date: Sun, 27 Jan 2019 05:57:57 GMT
  Via: 1.1 varnish
  Connection: close
  X-Served-By: cache-mdw17345-MDW, cache-hkg17926-HKG
  X-Cache: MISS, MISS
  X-Cache-Hits: 0, 0
  X-Timer: S1548568881.732217,VS0,VE268
  Vary: 
  RIC: 
  Content-Type: text/plain;charset=UTF-8
  Content-Length: 43

  D6t_CjaEE0buRV677WFUajX28DjBqyfH8iw_99zPMZY

  CLOSED

  */

  // We're able to get an http response when requesting for access token.
  // Now we need to parse it.
  char access_token[60] = "D6t_CjaEE0buRV677WFUajX28DjBqyfH8iw_99zPMZY";

  // HTTP Headers.
  // Authorization Basic SECRET_CREDENTIAL
  // X-CSRF-Token ACCESS_TOKEN
  // Content-Type application/vnd.api+json
  // Accept application/vnd.api+json

  // Parameters in json format.
  /**
  {
    "data": {
      "type": "node--contact",
      "attributes": {
        "title": "Contact",
        "field_phone_number": "+639000000000",
        "field_message": "SMS message."
      }
    }
  }
  */

  // Do a POST request.
  // const char* parameters;
  // parameters = "{\"data\":{\"type\":\"node--temperature\",\"attributes\":{\"title\":\"Temperature\",\"field_value_temperature\":21}}}";
  // numdata = inet.httpPOST("dev-elexparts-api.pantheonsite.io", 80, "/json/api/node/temperature", parameters, msg, 50);

  // Print the results.
  // Serial.println("\nPOST - Number of data received:");
  // Serial.println(numdata);
  // Serial.println("\nPOST - Data received:");
  // Serial.println(msg);
}

void loop() {
  // Read for new byte on serial hardware,
  // and write them on NewSoftSerial.
  serialhwread();

  // Read for new byte on NewSoftSerial.
  serialswread();

  // Save message details through a Contacts Service API.
  saveMessageToContacts();

  // Send an auto-reply message.
  // autoReply();

  delay(30000);
};