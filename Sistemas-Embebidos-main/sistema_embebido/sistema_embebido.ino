#define PIR1 12
#define PIR2 11
#define BUZZER 10
#define LED 13
#define SWITCH_ON 5
#define PUL_OFF 7

bool alarmaActiva = false;   // MEMORIA del estado de la alarma
bool estadoLed = false;
unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 100; // velocidad del parpadeo (ms)

void setup()
{
  Serial.begin(9600);
  pinMode(PIR1, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(SWITCH_ON, INPUT_PULLUP);
  pinMode(PUL_OFF, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  // LEER MIS ENTRADAS  
  bool estadoPir1 = digitalRead(PIR1);
  bool estadoPir2 = digitalRead(PIR2);
  bool estadoSwitch = !digitalRead(SWITCH_ON);
  bool estadoPul = !digitalRead(PUL_OFF);
  
  // SECCION MONITOR SERIE
  Serial.print("PIR1:");
  Serial.print(estadoPir1);
  Serial.print(" | PIR2:");
  Serial.print(estadoPir2);
  Serial.print(" | SWITCH:");
  Serial.println(estadoSwitch);

  // ---- LOGICA ORIGINAL: SE DISPARA Y QUEDA ENCENDIDA ----
  if ((estadoPir1 == true || estadoPir2 == true) && estadoSwitch == true) {
    alarmaActiva = true;   // Se dispara la alarma
  }

  if (estadoPul == true || estadoSwitch == false) {
    alarmaActiva = false;  // Se apaga solo con botón o switch
  }

  // ---- SALIDAS ----
  if (alarmaActiva) {
    digitalWrite(BUZZER, true);

    // LED tipo sirena
    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoAnterior >= intervalo) {
      tiempoAnterior = tiempoActual;
      estadoLed = !estadoLed;
      digitalWrite(LED, estadoLed);
    }

  } else {
    digitalWrite(BUZZER, false);
    digitalWrite(LED, false);
    estadoLed = false;
  }

  delay(10);
}