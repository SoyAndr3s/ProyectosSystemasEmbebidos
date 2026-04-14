// Lab01_Blink.ino 
  
const int PIN_LED = 13;         
int contador      = 0;          
int i;
// millis() devuelve el tiempo en ms desde que arrancó el Arduino
unsigned long tiempo_actual;
  
void setup() { 
     
  pinMode(PIN_LED, OUTPUT); 

  
  Serial.begin(9600); 
  Serial.println("=== Lab 01: Millis ==="); 
  Serial.println("Formato: [#N] t= millis ms LED -> ESTADO"); 
} 
  
void loop() { 
  contador++;
  
  // Ejercicio 3 — hint: uso de millis()

  // Encender el LED 
  tiempo_actual = millis();
  digitalWrite(PIN_LED, HIGH); 
  Serial.print("[#"); 
  Serial.print(contador); 
  Serial.print("] t=");
  Serial.print(tiempo_actual);
  Serial.println(" ms LED -> ENCENDIDO'"); 
  delay(200);                 


  // Apagar el LED 
  tiempo_actual = millis();
  digitalWrite(PIN_LED, LOW); 
  Serial.print("[#"); 
  Serial.print(contador); 
  Serial.print("] t=");
  Serial.print(tiempo_actual);
  Serial.print(" ms LED -> APAGADO");
  Serial.println();
  delay(800);
}