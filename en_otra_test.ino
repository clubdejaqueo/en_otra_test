int tiron = 250;
int afloje = 2600;
int frena = 4000;
int velMin = 60;
int velMax = 255;

#define array_size(foo) (sizeof(foo)/sizeof(foo[0]))

class Motor {
    byte pin_pwm, pin_A, pin_B;
  public:
    Motor(byte pin_pwm, byte pin_A, byte pin_B) :
      pin_pwm(pin_pwm), pin_A(pin_A), pin_B(pin_B) {
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
};

Motor motores[] = {
  {5, 6, 7},
  {8, 9, 10},
};

const byte cantidad_motores = array_size(motores);

void setup() {
  for (int n = 0; n < cantidad_motores; n++) {
    motores[n].setup();
  }
}


void loop() {
  Motor& m = motores[0];
  
  m.avanzar(velMax);
  delay (tiron);

  m.retroceder(velMin);
  delay (afloje);

  m.frenar();
  delay (random(500, 4000));

  m.avanzar(velMax);
  delay (tiron);

  m.retroceder(velMin);
  delay (afloje);

  m.frenar();
  delay (random(500, 4000));

  m.avanzar(velMax);
  delay (tiron);

  m.retroceder(velMin);
  delay (afloje);

  m.frenar();
  delay (random(500, 4000));
}




