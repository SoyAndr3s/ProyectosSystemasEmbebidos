const int PIN_LED = 13;        // Constante para el pin del LED 
int contador      = 0;         // Cuenta los parpadeos 
int i;
// millis() devuelve el tiempo en ms desde que arrancó el Arduino
unsigned long tiempo_actual = millis();
  
void setup() { 

  // Configurar el pin como SALIDA digital 
  pinMode(PIN_LED, OUTPUT); 

  // Iniciar comunicacion serial a 9600 bps 
  Serial.begin(9600); 
  Serial.println('=== Lab 01: Blink Iniciado ==='); 
  Serial.println('Formato: [#N] LED -> ESTADO'); 
} 
  
void loop() { 
  contador++;
  //  EJERCICIO  1 
  // Encender el LED 
  digitalWrite(PIN_LED, HIGH); 
  Serial.print("[#"); 
  Serial.print(contador); 
  Serial.println("] LED -> ENCENDIDO'"); 
  delay(200);                // Esperar 500 ms 

  // Apagar el LED 
  digitalWrite(PIN_LED, LOW); 
  Serial.print("[#"); 
  Serial.print(contador); 
  Serial.println("] LED -> APAGADO"); 
  delay(800);                // Esperar 500 ms 
  

}
