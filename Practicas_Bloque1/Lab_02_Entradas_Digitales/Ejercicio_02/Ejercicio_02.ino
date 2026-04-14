//     Ejercicio 2

const int PIN_BOTON = 2; 
const int PIN_LED   = 13; 
const unsigned long DEBOUNCE_MS = 20;   
const int PIN_RESET = 10;

int estado_led = LOW; 
int ultimo_estado = HIGH;       
int estado_estable = HIGH;      
int i=1;

unsigned long t_cambio = 0; 
int conteo = 0; 

void setup() { 
    pinMode(PIN_BOTON, INPUT_PULLUP); 
    pinMode(PIN_RESET, INPUT_PULLUP);
    pinMode(PIN_RESET, INPUT_PULLUP);
    pinMode(PIN_LED, OUTPUT); 
    Serial.begin(9600); 
    Serial.println("Lab 02 — Debounce activado"); 
} 

void loop() { 
    int lectura = digitalRead(PIN_BOTON); 
    int lectura_reset = digitalRead(PIN_RESET);
    digitalWrite(PIN_LED, LOW);
    
    if (lectura != ultimo_estado) { 
        t_cambio = millis(); 
    } 

    if ((millis() - t_cambio) > DEBOUNCE_MS) { 
        
        if (lectura != estado_estable) {
            estado_estable = lectura;

            if (estado_estable == LOW) {
                conteo++; 
                 
                Serial.print("Pulsacion #"); 
                Serial.print(conteo); 
                Serial.print("  LED: "); 
                Serial.print(conteo);
                Serial.println(" parpadeos"); 
                while(i<= conteo){
                    digitalWrite(PIN_LED, HIGH);
                    delay(350);
                    digitalWrite(PIN_LED, LOW);
                    delay(350);
                    i++;
                }
                i=1;
                estado_led = !estado_led;
            }
        }
    } 

    ultimo_estado = lectura; 
    if (lectura_reset == LOW){
        Serial.println("RESET");
        conteo = 0;
    }
} 