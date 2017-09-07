#include <NewPing.h>
#include <EEPROM.h>

#define TRIGGER_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 500
#define GOOD_LED_PIN 5
#define WARN_LED_PIN 6
#define GOOD_SET_BTN_PIN 8
#define WARN_SET_BTN_PIN 9
#define GOOD_EEPROM_ADDRESS 0
#define WARN_EEPROM_ADDRESS 4

int goodDistance = 0;
int warnDistance = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup (){
  Serial.begin(9600);
  
  pinMode(GOOD_LED_PIN, OUTPUT);
  pinMode(WARN_LED_PIN, OUTPUT);
  
  pinMode(GOOD_SET_BTN_PIN, INPUT);
  pinMode(WARN_SET_BTN_PIN, INPUT);
  
  // fetch distances from eeprom
  EEPROM.get(GOOD_EEPROM_ADDRESS, goodDistance);
  EEPROM.get(WARN_EEPROM_ADDRESS, warnDistance);
  
  Serial.print("Maximum good distance: ");
  Serial.print(goodDistance);
  Serial.println("ms");
  
  Serial.print("Maximum warn distance: ");
  Serial.print(warnDistance);
  Serial.println("ms");
  
}

void loop () {
  delay(50);
  
  // get distance
  int distance = sonar.ping();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("ms");

  bool distanceSet = false;
  
  // set good distance
  if (digitalRead(GOOD_SET_BTN_PIN) == HIGH){
    goodDistance = distance;
    EEPROM.put(GOOD_EEPROM_ADDRESS, goodDistance);
    Serial.print("Set good distance to ");
    Serial.print(goodDistance);
    Serial.println("ms");

    distanceSet = true;
  }

  // set warn distance
  if (digitalRead(WARN_SET_BTN_PIN) == HIGH){
    warnDistance = distance;
    EEPROM.put(WARN_EEPROM_ADDRESS, warnDistance);
    Serial.print("Set warn distance to ");
    Serial.print(warnDistance);
    Serial.println("ms");

    distanceSet = true;
  }
  
  // dislay sign that distance has been changed
  if (distanceSet){
    digitalWrite(GOOD_LED_PIN, HIGH);
    digitalWrite(WARN_LED_PIN, HIGH);
    delay(1000);
    digitalWrite(GOOD_LED_PIN, LOW);
    digitalWrite(WARN_LED_PIN, LOW);
  }

  // check distance conflicts
  if (warnDistance > goodDistance){
    digitalWrite(GOOD_LED_PIN, HIGH);
    digitalWrite(WARN_LED_PIN, HIGH);
    Serial.println("Invalid distances, good distance is less than warn distance.");
  } else {
    // check distances
    if (distance < warnDistance){
      digitalWrite(WARN_LED_PIN, HIGH);
      digitalWrite(GOOD_LED_PIN, LOW);
      Serial.println("Below warn distance");
    } else if (distance < goodDistance){
      digitalWrite(GOOD_LED_PIN, HIGH);
      digitalWrite(WARN_LED_PIN, LOW);
      Serial.println("Below good distance");
    } else {
      digitalWrite(WARN_LED_PIN, HIGH);
      digitalWrite(GOOD_LED_PIN, LOW);
      Serial.println("Above good distance");
    }
  }
    
}
