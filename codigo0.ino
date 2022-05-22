// Grupo 3 - Control de posicion de un motor DC

int potPin = A3;          //  Potenciometro de referencia. En este potenciometro vamos a setear un angulo.
int encoder_pot = A2;    // Sensor de posicion angular. Es un potenciometro donde vamos a leer el valor de voltaje del PIN A2

//Variables que guarda valores del sensor y referencia
int val = 0;
int encoder_val =0;

// Constantes de PID 
float kp = .1;
float ki = 0.001;
float kd = 2;

// Valores de que tiene potenciometro como sensor y _d es el deseado
float Theta, Theta_d;

// Paso de tiempo para integracion en euler y pendientes
int dt;
unsigned long t;

// Inicializacion de variables que son valores pasados de una iteracion
unsigned long t_prev = 0;
int val_prev =0;
float e, e_prev = 0, inte, inte_prev = 0;

float Vmax = 12;      // Voltaje maximo que puede entrar al motor tanto positivo como negativo. Esto satura la salida.
float Vmin = -12;     
float V = 0.1;        //  Inicializacion de voltaje que le entra al motor

// Definicion de pines
const byte PWMPin = 6;   // Entra al motor una señal PWM que se usa para controlar su velocidad
const byte DirPin1 = 7;   //El pin 7 y 8 sirven para darle el sentido de giro al motor 
const byte DirPin2 = 8;


//***Motor Driver Functions*****
//En esta funcion se entra con un valor de voltaje que es dado por el controlador, con un voltaje maximo dado por la fuente de voltaje
// y el voltaje maximo del motor

void WriteDriverVoltage(float V, float Vmax) {
  
  int PWMval = int(255 * abs(V) / Vmax);
  
  if (PWMval > 255) {
    PWMval = 255;
  }
 
  if (V > 0) {
    digitalWrite(DirPin1, HIGH);// Da un sentido de giro distinto al segundo if
    digitalWrite(DirPin2, LOW);
  }
  else if (V < 0) {
    digitalWrite(DirPin1, LOW);
    digitalWrite(DirPin2, HIGH);
  }
  else {
    digitalWrite(DirPin1, LOW);// para el caso que V==0 el motor se queda quieto 
    digitalWrite(DirPin2, LOW);
  }
  analogWrite(PWMPin, PWMval); //en este paso le ingreso la señal PWM al motor desde el pin 6 (PWMPin)

}


void setup() {
  Serial.begin(9600);
  pinMode(DirPin1, OUTPUT);//son salidas digitales para darle un sentido de giro al motor
  pinMode(DirPin2, OUTPUT);

}


void loop() {
  
  val = analogRead(potPin);                           // Lee el valor de voltaje del potenciometro de referencia
  encoder_val =analogRead(encoder_pot);               // Lee el valor de voltaje del potenciometro usado como encoder
  
  t = millis();           // En este paso contamos el tiempo de ejecucion del programa para posteriormente calcular el dt 
  dt = (t - t_prev);      // Paso del tiempo para Euler. Donde t es el tiempo actual y t_prev es el tiempo que leyo millis() en la anterior iteracion
  
  Theta = val;                                        // Theta= Posicion angular actual del motor.
  Theta_d = encoder_val;                              // Theta_d= Posicion angular deseada.

  e = Theta_d - Theta;                                // Calculo del error del feedback
  
  inte = inte_prev + (dt * (e + e_prev) / 2);         // Integracion del error
  V = kp * e + ki * inte + (kd * (e - e_prev) / dt) ; // Funcion del control


// Saturacion del valor ingresado al motor y de las señales error
  if (V > Vmax) {
      V = Vmax;
      inte = inte_prev;
    }
    if (V < Vmin) {
      V = Vmin;
      inte = inte_prev;
      val_prev= val;
    }

    
  WriteDriverVoltage(V, Vmax);// Llamo a la funcion para setear el voltaje al driver

  // Escribo en el monitor serie los valores deseados
  Serial.println(Theta_d); Serial.print(" \t");
  Serial.print(Theta); Serial.print(" \t ");
  // De paso grafico la señal error para ver que pasa
  Serial.print(V); Serial.print(" \t ");
  
  t_prev = t;
  inte_prev = inte;
  e_prev = e;
  
    delay(10);

}
