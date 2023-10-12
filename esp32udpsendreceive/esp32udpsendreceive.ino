#include <WiFi.h>
#include <string.h>
#include <ESP32Servo.h>

#define RXp2 16
#define TXp2 17

/* WiFi network name and password */
// Your wirelless router ssid and password
//firebase                                        
#include <IOXhop_FirebaseESP32.h>  
#define FIREBASE_Host "https://parkovisko-14fcb-default-rtdb.europe-west1.firebasedatabase.app"                   // replace with your Firebase Host
#define FIREBASE_authorization_key "w8hjMwOb3yaWnaCd7IxD4iyYfdXXbfiwyiBbXIw2" // replace with your secret key
#define WIFI_SSID "cloud"       // replace with your SSID
#define WIFI_PASSWORD "connected123"          //replace with your Password


Servo myservo;  

//pins
//communication
bool parkoviska[] = {0,0,0,0,0,0};
bool cakanieNaAuto = false;
bool autoPodRampou = false;

//brana
int gateOpenAngle = 90; 
int gateCloseAngle = 0;


void setup(){
  Serial.begin(115200); // to monitor activity
  //Connect to the WiFi network
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   myservo.attach(13);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  pinMode(14, OUTPUT);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);
  myservo.write(gateCloseAngle);
  Serial.print("Connected to arduino");
  Firebase.begin(FIREBASE_Host, FIREBASE_authorization_key);
}

void loop(){
  //technical
  if(cakanieNaAuto){
    bool  boolTemp = Firebase.getString("parkovisko/zavora") == "n";
      if(autoPodRampou ||  boolTemp){
        if(digitalRead(12) || boolTemp){
          delay(300);  
          cakanieNaAuto = false;
          autoPodRampou = false;
          myservo.write(gateCloseAngle);
          Firebase.setString("parkovisko/zavora", "n"); 
        }
      }
      else{
        if(digitalRead(12) == 0){
        autoPodRampou = 1;
        delay(100);
        }
      }
    }
    else{
  String temp = Serial2.readStringUntil('#'); 
  while (Serial2.available() > 0) {
    Serial2.read(); // Read and discard any remaining characters
  }
  Serial.println(temp);
  Firebase.setString("parkovisko/data", temp); 
  
  if(true){
    String tempGate = Firebase.getString("parkovisko/zavora");
    
      if(tempGate == "y"){
        myservo.write(gateOpenAngle);
        cakanieNaAuto = true;
      }
      else if (tempGate == "w"){
        digitalWrite(14, HIGH);
        delay(500);
        digitalWrite(14, LOW);
        Firebase.setString("parkovisko/zavora", "n"); 
      }
      else if (tempGate == "n"){
        myservo.write(gateCloseAngle);
        cakanieNaAuto = false;
        autoPodRampou = false;
      }
    
  }
  
  delay(10);

    }
}
