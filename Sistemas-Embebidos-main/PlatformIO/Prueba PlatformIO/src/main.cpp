#include <Arduino.h> 

int led = 2; 
void setup() { 
 // put your setup code here, to run once: 
 pinMode(led,OUTPUT); 
} 
void loop() { 
 digitalWrite(led,HIGH); 
 delay(1000); 
 digitalWrite(led,LOW); 
 delay(1000); 
} 