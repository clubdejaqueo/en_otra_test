const int tiempo_tiron = 3500;
const int tiempo_afloje = 3900;
const int tiempo_variable = -1;

const int velMin = -200;
const int velMax = 210;
const int velZero = 0;
const int FIN_PROGRAMA = 9999;

const int programa1[] = {
  velMax, tiempo_tiron,
  velMin, tiempo_afloje,
  velZero, tiempo_variable,
  FIN_PROGRAMA
};

const int programa2[] = {
  200, 2100,
  -170, 1200,
  0, tiempo_variable,
  
  200, 2100,
  -200, 1000,
  
  200, 2100,
  -200, 2100,
  
  -170, 1200,
  170, 1200,
  
  FIN_PROGRAMA
};

#define array_size(foo) (sizeof(foo)/sizeof(foo[0]))
const byte len_programa1 = array_size(programa1);


class Motor {
    byte pin_pwm, pin_A, pin_B;
    int delay_min, delay_max;
    unsigned long tiempo_ultimo_paso;
    int espera;
    byte contador_programa;
    const int* programa;
    int len_programa;
  public:
    Motor(byte pin_pwm, byte pin_A, byte pin_B, int delay_min, int delay_max,
          const int* programa) :
      pin_pwm(pin_pwm), pin_A(pin_A), pin_B(pin_B), delay_min(delay_min),
      delay_max(delay_max), programa(programa),
      tiempo_ultimo_paso(0), contador_programa(0) {
    }

    void setup() {
      pinMode(pin_pwm, OUTPUT);
      pinMode(pin_A, OUTPUT);
      pinMode(pin_B, OUTPUT);
    }

    void avanzar(int velocidad) {
      Serial.println(" Avanzando");
      analogWrite(pin_pwm, velocidad);
      digitalWrite(pin_A, HIGH);
      digitalWrite(pin_B, LOW);
    }

    void retroceder(int velocidad) {
      Serial.println(" Marcha Atras");
      analogWrite(pin_pwm, velocidad);
      digitalWrite(pin_A, LOW);
      digitalWrite(pin_B, HIGH);
    }

    void frenar() {
      Serial.println(" Frenar");
      digitalWrite(pin_A, LOW);
      digitalWrite(pin_B, LOW);
    }

    void cambiar_velocidad(int vel) {
      Serial.print(pin_pwm);
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
      if ((millis() - tiempo_ultimo_paso) > espera) {
        int velocidad = programa[contador_programa++];
        cambiar_velocidad(velocidad);

        espera = programa[contador_programa++];
        if (espera == tiempo_variable) {
          espera = calcular_tiempo_variable();
        }
        Serial.print("Esperando...");
        Serial.println(espera);
        tiempo_ultimo_paso = millis();

        if (programa[contador_programa] == FIN_PROGRAMA) {
          contador_programa = 0;
          return;
        }
      }
    }
};


Motor motores[] = {
  // pin_pwm, pin_A, pin_B, delay_min, delay_max
  { 5, 6, 7, 500, 4000, programa1 },
  { 3, 2, 4, 500, 4000, programa2 },
};

const byte cantidad_motores = array_size(motores);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.print("Cantidad de motores: ");
  Serial.println(cantidad_motores);

  for (int n = 0; n < cantidad_motores; n++) {
    motores[n].setup();
  }

}

void loop() {

  for (int n = 0; n < cantidad_motores; n++) {
    motores[n].loop();
  }

}
