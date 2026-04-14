
// Parte 3

//     Ejercicio 3
const int PIN_BOTON_ROJO = 2;
const int PIN_BOTON_VERDE = 10; 
const int PIN_LED_VERDE = 13;
const int PIN_LED_ROJO = 12; 
const unsigned long DEBOUNCE_MS = 20;

// ROJO
int estado_rojo = HIGH;
int ultimo_rojo = HIGH;
unsigned long t_rojo = 0;

// VERDE
int estado_verde = HIGH;
int ultimo_verde = HIGH;
unsigned long t_verde = 0;

void setup() { 
    pinMode(PIN_BOTON_ROJO, INPUT_PULLUP);
    pinMode(PIN_BOTON_VERDE, INPUT_PULLUP);
    pinMode(PIN_LED_VERDE, OUTPUT);
    pinMode(PIN_LED_ROJO, OUTPUT);
    Serial.println("Lab 02 — Ejercicio 3");
}

void loop() { 
    int lectura_rojo = digitalRead(PIN_BOTON_ROJO);
    int lectura_verde = digitalRead(PIN_BOTON_VERDE);

    // -------- ROJO --------
    if (lectura_rojo != ultimo_rojo) {
        t_rojo = millis();
    }

    if ((millis() - t_rojo) > DEBOUNCE_MS) {
        estado_rojo = lectura_rojo;
    }

    ultimo_rojo = lectura_rojo;

    // -------- VERDE --------
    if (lectura_verde != ultimo_verde) {
        t_verde = millis();
    }

    if ((millis() - t_verde) > DEBOUNCE_MS) {
        estado_verde = lectura_verde;
    }

    ultimo_verde = lectura_verde;

    // -------- CONTROL DE LEDs --------
    // LOW = presionado (por pull-up)

    if (estado_rojo == LOW) {
        digitalWrite(PIN_LED_ROJO, HIGH);
    } else {
        digitalWrite(PIN_LED_ROJO, LOW);
    }

    if (estado_verde == LOW) {
        digitalWrite(PIN_LED_VERDE, HIGH);
    } else {
        digitalWrite(PIN_LED_VERDE, LOW);
    }
}