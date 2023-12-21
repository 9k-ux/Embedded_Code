#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <FirebaseESP8266.h>
//#include <WiFiUdp.h>
//#include <NTPClient.h>
const char *ssid     = "pk";
const char *password = "sitaram12345";
#define FIREBASE_HOST "whatsapp-clone-52167-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "xV5Ie0M6zZYlQkFf0KO1LXm84gnn4j6k1UWSkjT1"
FirebaseData firebaseData;
String val = "example";
#define MOTOR_ON 5
#define MOTOR_OFF 13
#define phaseA 12
#define phaseB 14
#define status1 3
#define Auto_Relay 15
#define Auto_Button 0
#define onboard_led 2
int xx;

bool phaseRead, phaserReadA, statusRead;
bool phaseReadState, phaserReadAState, statusReadState, Auto_state;
bool Autoinstant;
bool Auto_Runner;
int Writer;
bool counter;
bool counter1;
bool counter2;
bool counter3;
unsigned long previousMillis = 0;
const long interval = 1000;
unsigned long previousMillis2 = 0;
const long interval2 = 3000;
unsigned long previousMillis3 = 0;
const long interval3 = 1000;

void setup() {
  pinMode(phaseA, INPUT_PULLUP);
  pinMode(phaseB, INPUT_PULLUP);
  pinMode(status1, INPUT_PULLUP);
  pinMode(MOTOR_ON, OUTPUT);
  pinMode(MOTOR_OFF, OUTPUT);
  pinMode(Auto_Relay, OUTPUT);
  pinMode(Auto_Button, INPUT_PULLUP);
  Firebase.setBool(firebaseData, "/B-Phase", false);
  Auto_state = true;
  Auto_Runner = false;

  Serial.begin(115200);
  EEPROM.begin(512);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {

    phaseRead = digitalRead(12);
    phaserReadA = digitalRead(14);
    statusRead = digitalRead(3);
    counter3  =  digitalRead(0);
    if ((counter3 == false) && counter2 == true)
    {Serial.println("Button pushed");
      EEPROM.write(1, !EEPROM.read(1));
      EEPROM.commit();
      if (EEPROM.read(1) == true)
      {
        if ((phaseRead == false) && phaserReadA == false)
        {
          digitalWrite(Auto_Relay, EEPROM.read(1));
        }
      }

      if (EEPROM.read(1) == false)
      {
        digitalWrite(Auto_Relay, EEPROM.read(1));
      }
      counter2 = false;
    }
    if (counter3 == true)
    {
      counter2 = true;

    }
    delay(10);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop() {
  phaseRead = digitalRead(12);
  phaserReadA = digitalRead(14);
  statusRead = digitalRead(3);
  counter3  =  digitalRead(0);

  if ((counter3 == false) && counter2 == true)
  {
    EEPROM.write(1, !EEPROM.read(1));
    EEPROM.commit();
    if (EEPROM.read(1) == true)
    {
      moter_Opration(13);
    }

    if (EEPROM.read(1) == false)
    {
      moter_Opration(14);
    }
    counter2 = false;
  }
  if (counter3 == true)
  {
    counter2 = true;

  }

  if (WiFi.status() == WL_CONNECTED) {
    if ((EEPROM.read(1) == true) && Auto_Runner == false)
    {
      moter_Opration(13);
    }

    if (digitalRead(Auto_Button) == HIGH)
    {
      counter == false;
    }

    if ((digitalRead(Auto_Button) == LOW) && counter == true)
    {
      moter_Opration(13);
      counter = false;
    }
    Serial.println(WiFi.status());
    if (Firebase.ready()) {

      Serial.println(EEPROM.read(1));
      if (Firebase.getInt(firebaseData, "/CONTROLL")) {
        int val = firebaseData.intData();
        switch (val) {
          case 11:
            if (Autoinstant == false)
            {
              moter_Opration(val);
              Firebase.setInt(firebaseData, "/CONTROLL", 0);
            }
            break;

          case 12:
            moter_Opration(val);
            Firebase.setInt(firebaseData, "/CONTROLL", 0);
            break;

          case 13:
            moter_Opration(val);
          case 14:
            moter_Opration(val);
        }
      }

      if ((counter3 == false) && counter2 == true)
      {
        Serial.println("Condition Getting");

        EEPROM.write(1, !EEPROM.read(1));
        EEPROM.commit();
        if (EEPROM.read(1) == true)
        {
          moter_Opration(13);
        }

        if (EEPROM.read(1) == false)
        {
          moter_Opration(14);
        }
        counter2 = false;
      }
      if (counter3 == true)
      {
        counter2 = true;

      }

    }
    switch (Writer) {
      case 1:
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          digitalWrite(MOTOR_ON, LOW);
          digitalWrite(MOTOR_OFF, LOW);
          Writer = 0;
          Serial.println("Without_ delay runing ");
        }
    }

    unsigned long currentMillis2 = millis();

    if (currentMillis2 - previousMillis2 >= interval2) {

      Firebase.setBool(firebaseData, "/R-Phase", phaseRead);
      Firebase.setBool(firebaseData, "/Y-Phase", phaserReadA);
      Firebase.setBool(firebaseData,  "/STATUS", statusRead);

      previousMillis2 = currentMillis2;

    }
  }
  else {
    ESP.restart();
  }

  if ((phaseRead == true) || phaserReadA == true)
  {
    digitalWrite(Auto_Relay, LOW);
    digitalWrite(MOTOR_ON, LOW);
    digitalWrite(MOTOR_OFF, HIGH);
  }

}
int moter_Opration(int value0)
{
  if ((phaseRead == false) && phaserReadA == false)
  {
    switch (value0)
    {
      case 11:
        digitalWrite(MOTOR_ON, HIGH);
        digitalWrite(MOTOR_OFF, LOW);
        Serial.println("Function runing ");
        Writer = 1;
        break;

      case 13:
        digitalWrite(Auto_Relay, HIGH);
        Autoinstant = true;
        EEPROM.write(1, true);
        EEPROM.commit();
        Auto_Runner = true;
        counter1 = false;
        break;

      case 14:
        digitalWrite(Auto_Relay, LOW);
        Autoinstant = false;
        EEPROM.write(1, false);
        EEPROM.commit();
        Auto_Runner = false;
        counter1 = true;
        break;
    }
    if (value0 == 12)
    {
      digitalWrite(MOTOR_ON, LOW);
      digitalWrite(MOTOR_OFF, HIGH);
      Writer = 1;
    }
    return 0;
  }
  else {
    Serial.println("Condition false");
  }
}
