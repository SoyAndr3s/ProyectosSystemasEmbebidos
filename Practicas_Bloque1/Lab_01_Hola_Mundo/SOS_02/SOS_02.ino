// Lab01_Blink.ino 
  
const int PIN_LED = 13;         
int contador      = 0;          
int i;
// millis() devuelve el tiempo en ms desde que arrancó el Arduino
unsigned long tiempo_actual;
  
void setup() { 
  pinMode(PIN_LED, OUTPUT); 

  
  Serial.begin(9600); 
  Serial.println("=== Lab 01: SOS ==="); 
  Serial.println("Formato: [#N] t= millis ms LED -> ESTADO"); 
} 
  
void loop() { 
  contador++;

  // EJERCICIO 2
  
  i=0;
  
  //  S
  while (i<3){
    digitalWrite(PIN_LED, HIGH); 
    Serial.print("[#"); 
    Serial.print(contador); 
    Serial.println("] LED -> ENCENDIDO'"); 
    delay(200);
      

    digitalWrite(PIN_LED, LOW); 
    Serial.print("[#"); 
    Serial.print(contador); 
    Serial.println("] LED -> APAGADO"); 
    delay(300);
      

    i++;
  }

  //  O
  i=0;
  while (i<3){
    digitalWrite(PIN_LED, HIGH); 
    Serial.print("[#"); 
    Serial.print(contador); 
    Serial.println("] LED -> ENCENDIDO'"); 
    delay(600);
      

    digitalWrite(PIN_LED, LOW); 
    Serial.print("[#"); 
    Serial.print(contador); 
    Serial.println("] LED -> APAGADO"); 
    delay(300);
      

    i++;
  }

  //   S
  i=0;
  while (i<3){
    digitalWrite(PIN_LED, HIGH); 
    Serial.print("[#"); 
    Serial.print(contador); 
    Serial.println("] LED -> ENCENDIDO'"); 
    delay(200);
    

    digitalWrite(PIN_LED, LOW); 
    Serial.print("[#"); 
    Serial.print(contador); 
    Serial.println("] LED -> APAGADO"); 
    delay(300);
    

    i++;
  }

  // PAUSA
  delay(2000);
    
}