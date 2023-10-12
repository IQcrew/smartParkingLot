#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
int sensorsPins[] = {3,4,5,6,7,8};

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
String id = "";
String sensors = "nnnnnn";

//brana
int gateOpenAngle = 90; 
int gateCloseAngle = 0;


void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
    for (int i= 0; i<6; i++){
    pinMode(sensorsPins[i], INPUT);
  }

}
void loop() 
{
  
  
  id = card();
  for (int i= 0; i<6; i++){
    sensors[i] = digitalRead(sensorsPins[i]) ? 'y' : 'n';
  }
  Serial.print(id+","+sensors+"#");
  delay(800);

} 

String card(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return "           ";
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return "           ";
  }
  //Show UID on serial monitor
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     
  }
  content.toUpperCase();
  return content.substring(1);

}
