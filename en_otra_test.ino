const int tiempo_tiron = 250;
const int tiempo_afloje = 2600;
const int tiempo_variable = -1;

const int velMin = -60;
const int velMax = 255;
const int velZero = 0;

const int programas[] = {
  velMin, tiempo_afloje,
  velZero, tiempo_variable,
  velMax, tiempo_tiron,
};

const byte programa_reset = 0; // afloje
const byte programa_setup = 4; // tiron (dos columnas)

#define array_size(foo) (sizeof(foo)/sizeof(foo[0]))
const byte len_programas = array_size(programas);

class Motor {
    byte pin_pwm, pin_A, pin_B, pin_sensor;
    int delay_min, delay_max;
    int umbral_sensor;
    unsigned long tiempo_proximo_paso;
    byte contador_programa;
  public:
    Motor(byte pin_pwm, byte pin_A, byte pin_B, int delay_min, int delay_max, byte pin_sensor, int umbral_sensor) :
      pin_pwm(pin_pwm), pin_A(pin_A), pin_B(pin_B), delay_min(delay_min),
      delay_max(delay_max), pin_sensor(pin_sensor), umbral_sensor(umbral_sensor),
      tiempo_proximo_paso(millis()), contador_programa(programa_setup) {
    }

    void setup() {
      pinMode(pin_pwm, OUTPUT);
      pinMode(pin_A, OUTPUT);
      pinMode(pin_B, OUTPUT);
    }

    void avanzar(int velocidad) {
      analogWrite(pin_pwm, velocidad);
      digitalWrite(pin_A, HIGH);
      digitalWrite(pin_B, LOW);
    }

    void retroceder(int velocidad) {
      analogWrite(pin_pwm, velocidad);
      digitalWrite(pin_A, LOW);
      digitalWrite(pin_B, HIGH);
    }

    void frenar() {
      digitalWrite(pin_A, LOW);
      digitalWrite(pin_B, LOW);
    }

    void cambiar_velocidad(int vel) {
      if (vel == velZero) {
        frenar();
      }
      if (vel > 0) {
        avanzar(vel);
      }
      if (vel < 0) {
        retroceder(abs(vel));
      }
    }

    int calcular_tiempo_variable() {
      return random(delay_min, delay_max);
    }
    
    bool sensor_activado() {
      int val = analogRead(pin_sensor);
      if (val < umbral_sensor) {
        return true;
      }
      return false;
    }

    void loop() {
      unsigned long now = millis();
      
      if (sensor_activado()) {
        contador_programa = programa_reset;
        tiempo_proximo_paso = now;
      }
      
      if ((tiempo_proximo_paso - now) <= 0) {
        int velocidad = programas[contador_programa++];
        cambiar_velocidad(velocidad);

        int espera = programas[contador_programa++];
        if (espera == tiempo_variable) {
          espera = calcular_tiempo_variable();
        }
        tiempo_proximo_paso += espera;

        if (contador_programa > len_programas) {
          contador_programa = 0;
        }
      }
    }
};

Motor motores[] = {
  // pin_pwm, pin_A, pin_B, delay_min, delay_max, pin_sensor, umbral_sensor
  {  5,       6,     7,     500,       4000,      A3,         1 },
  {  8,       9,     10,    500,       4000,      A2,         1 },
};

const byte cantidad_motores = array_size(motores);

void setup() {
  for (int n = 0; n < cantidad_motores; n++) {
    motores[n].setup();
  }
}

void loop() {
  for (int n = 0; n < cantidad_motores; n++) {
    motores[n].loop();
  }
  delay(10);
}
