#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
//#include <Firebase_ESP_Client.h>
//#include <ArduinoJson.h>
#include <NTPClient.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define DATABASE_URL ""
#define API_KEY ""
const char* ssid = "";
const char* password = "";
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
int r1= 16; // S1 Red
int r2=4; // S2 Red
int r3=2; // S3 Red
int r4=12; // S4 Red
int g1=5; // S1 Green
int g2=0; // S2 Green
int g3=14; // S3 Green
int g4=13; // S4 Green
int s[4];
unsigned long sendDataPrevMillis=0;
bool signupOK=false; 
const char* ntpServer = "pool.ntp.org";
const long utcOffsetInSeconds = 0; // Adjust this to your timezone, 0 for UTC
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);
void setup() {
  Serial.begin(9600); // Start serial communication for debugging
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(g3, OUTPUT);
  pinMode(g4, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); 
    delay(300);
  }
  Serial.println("Connected to");
  Serial.println(ssid);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());   
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config,&auth,"","")){
    Serial.println("signUp OK");
    signupOK=true;
  }
  else{
    Serial.printf("%s\n",config.signer.signupError.message.c_str());
  }
  config.token_status_callback=tokenStatusCallback;
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
  timeClient.begin();
}

void loop() {
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime(); 
  
  if(Firebase.ready() && signupOK){
    char buffer[30]="";
    if(Serial.available()>0){
      String data=Serial.readStringUntil('\n');
      int index1 = data.indexOf(',');
      int index2 = data.indexOf(',', index1 + 1);
      int index3 = data.indexOf(',', index2 + 1);

      s[0] = data.substring(0, index1).toInt();
      s[1] = data.substring(index1 + 1, index2).toInt();
      s[2] = data.substring(index2 + 1, index3).toInt();
      s[3] = data.substring(index3 + 1).toInt();
      int m=s[0],n=0;
      for(int i=1;i<4;i++)
      {
        if(s[i]>m)
        {
          m=s[i];
          n=i;
        }
      }
      if(n==0) 
      { 
        digitalWrite(g1,HIGH);
        digitalWrite(g2,LOW);
        digitalWrite(g3,LOW);
        digitalWrite(g4,LOW);
        digitalWrite(r1,LOW);
        digitalWrite(r2,HIGH);
        digitalWrite(r3,HIGH);
        digitalWrite(r4,HIGH);
        
      }
      else if(n==1) 
      { 
        digitalWrite(g1,LOW);
        digitalWrite(g2,HIGH);
        digitalWrite(g3,LOW);
        digitalWrite(g4,LOW);
        digitalWrite(r1,HIGH);
        digitalWrite(r2,LOW);
        digitalWrite(r3,HIGH);
        digitalWrite(r4,HIGH);
        
      }
      else if(n==2) 
      { 
        digitalWrite(g1,LOW);
        digitalWrite(g2,LOW);
        digitalWrite(g3,HIGH);
        digitalWrite(g4,LOW);
        digitalWrite(r1,HIGH);
        digitalWrite(r2,HIGH);
        digitalWrite(r3,LOW);
        digitalWrite(r4,HIGH);
        
      }
      else 
      { 
        digitalWrite(g1,LOW);
        digitalWrite(g2,LOW);
        digitalWrite(g3,LOW);
        digitalWrite(g4,HIGH);
        digitalWrite(r1,HIGH);
        digitalWrite(r2,HIGH);
        digitalWrite(r3,HIGH);
        digitalWrite(r4,LOW);
        
      }
//      Serial.print(m);
//      Serial.println(':');
//      Serial.println(n);
      unsigned long time = timeClient.getEpochTime();
      
      
//      if(Firebase.RTDB.setInt(&fbdo, "/sensorData/s1/"+String(time), s[0]))
//        Serial.println("s1 sent to Firebase");
//      else
//        Serial.printf("Failed to send s1. %s\n", fbdo.errorReason().c_str());
//      if(Firebase.RTDB.setInt(&fbdo, "/sensorData/s2/"+String(time), s[1]))
//        Serial.println("s2 sent to Firebase");
//      else
//        Serial.printf("Failed to send s2. %s\n", fbdo.errorReason().c_str());
//      if(Firebase.RTDB.setInt(&fbdo, "/sensorData/s3/"+String(time), s[2]))
//        Serial.println("s3 sent to Firebase");
//      else
//        Serial.printf("Failed to send s3. %s\n", fbdo.errorReason().c_str());
//      if(Firebase.RTDB.setInt(&fbdo, "/sensorData/s4/"+String(time), s[3]))
//        Serial.println("s4 sent to Firebase");
//      else
//        Serial.printf("Failed to send s4. %s\n", fbdo.errorReason().c_str());
    }
  } 
}
