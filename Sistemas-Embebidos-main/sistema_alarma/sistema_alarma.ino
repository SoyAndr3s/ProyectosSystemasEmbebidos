//#define SWITCH_ON 5
#define BUZZER 12
const int trigPin = 9; 
const int echoPin = 10; 
const int LED = 13;
const int pulsador = 7;
unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 100;
bool estadoLed= false;
int SWITCH_ON = 5;
bool alarmaActiva = false;  
bool estadoBuzzer = false;


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  pinMode(SWITCH_ON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  digitalWrite(LED, LOW);
  delayMicroseconds(2);
  
  bool estadoPul = !digitalRead(pulsador);
  bool estadoSwitch = !digitalRead(SWITCH_ON);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = duration * 0.0343 / 2; 
  
  //Comienza la logica 
  
   
  if(distance < 20 && estadoSwitch == true){
  	Serial.println("alarma activa");
    Serial.println(distance);
    alarmaActiva = true;
  }
  
  if (estadoPul == true || estadoSwitch == false) {
    alarmaActiva = false;
    Serial.println("alarma desactivada");// Se apaga solo con botón o switch
  }
  
  if (alarmaActiva) {
    //digitalWrite(BUZZER, true);

    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoAnterior >= intervalo) {
      tiempoAnterior = tiempoActual;
      estadoBuzzer = !estadoBuzzer;
      estadoLed = !estadoLed;
      digitalWrite(LED, estadoLed);
      digitalWrite(BUZZER, estadoBuzzer);
    }
  } else {
    digitalWrite(BUZZER, false);
    digitalWrite(LED, false);
    estadoLed = false;
  }
  

  delay(500);
}