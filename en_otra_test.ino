const int tiron = 250;
const int afloje = 2600;
const int tiempo_variable = -1;

const int velMin = -60;
const int velMax = 255;
const int velZero = 0;

#define array_size(foo) (sizeof(foo)/sizeof(foo[0]))

const int programas[] = {
    velMax, tiron,
    velMin, afloje,
    velZero, tiempo_variable,
};

const byte len_programas = array_size(programas);

class Motor {
    byte pin_pwm, pin_A, pin_B;
    int delay_min, delay_max;
    unsigned long tiempo_proximo_paso;
    byte contador_programa;
  public:
    Motor(byte pin_pwm, byte pin_A, byte pin_B, int delay_min, int delay_max) :
      pin_pwm(pin_pwm), pin_A(pin_A), pin_B(pin_B), delay_min(delay_min),
      delay_max(delay_max), tiempo_proximo_paso(millis()), contador_programa(0) {
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
    
    void loop() {
      if ((tiempo_proximo_paso-millis()) <= 0) {
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
  // pin_pwm, pin_A, pin_B, delay_min, delay_max
  {5, 6, 7, 500, 4000},
  {8, 9, 10, 500, 4000},
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
}
