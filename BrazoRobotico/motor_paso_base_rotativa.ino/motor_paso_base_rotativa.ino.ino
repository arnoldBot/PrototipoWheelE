// La secuencia de pulsos que hay que dar a cada bobina del paso a paso para que gire el eje.
int Paso[8][4] =
   {  {1, 0, 0, 0}, 
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 1},
      {0, 0, 0, 1},
      {1, 0, 0, 1}
   };

// Definimos los pines del paso a paso. Hay que cambiarlo en funcion de los disponibles del Ard.Mega
#define IN1  2 
#define IN2  3
#define IN3  4
#define IN4  5

int steps_left=4095; // Variable que contiene el numero de pasos quedan por dar.
boolean Direction = true; // Variable que indica la direccion, es decir, si la secuencia de pulsos va en un sentido o el otro.
int Steps = 0; // Define el paso actual de la secuencia.
int currentAngle = 0; // Indica el angulo actual del motor.
int angulo = 0; // Variable que contiene el angulo al que se quiere llegar.

void setup()
{ 
  // Definimos el modo de los pines.
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600); // Configuramos el puerto serie a 9600 baudios.
  Serial.println("Arduino Ready."); //Indicamos por el puerto serie que el arduino esta listo para operar. (Util para controlar con python por puerto serie)
}

void loop()
{ 
  if(Serial.available()){ // Si hay datos a leer en el puerto serie.
    angulo=Serial.parseInt(); // Leemos el numero 
  }
  if(angulo>=0 && angulo < 360){ // Si ese numero esta contenido en el rango permitido .
    rotaBase(angulo); // Rotamos la base tantos grados como indica la variable angulo.
    angulo=-1; // Borramos el contendio de la variable.
  }
}

void rotaBase(int requestedAngle){
  // Comparamos el angulo actual con el pedido para averiguar en que direccion girar.
  if((currentAngle-requestedAngle)>0){ 
    Direction = 0;
  }
  if((currentAngle-requestedAngle)<0){
    Direction = 1;
  }
  
  steps_left=abs(currentAngle-requestedAngle)/0.08789; // Calculamos el numero de pasos que tiene que dar para llegar a ese angulo.
  while(steps_left>0) // Si ya no quedan mas pasos para dar salimos del bucle.
  { 
    stepper() ;     // Avanza un paso
    steps_left-- ;  // Un paso menos que tenemos que dar
    delay (1) ; // Esperamos 1 ms para que le de tiempo al motor avanzar un paso.
  }
  currentAngle=requestedAngle; // Una vez llegado al angulo que se nos pidio actualizamos la variable de angulo actual.
  Serial.print("Angulo actual: "); // Indicamos por puerto serie que ya hemos llegado a ese angulo.
  Serial.println(currentAngle);
}

void stepper() // Secuencia para avanzar un paso
{
  digitalWrite( IN1, Paso[Steps][0] );
  digitalWrite( IN2, Paso[Steps][1] );
  digitalWrite( IN3, Paso[Steps][2] );
  digitalWrite( IN4, Paso[Steps][3] );
  SetDirection(); // Segun la direccion el siguente paso tendra un conjunto de pulsos u otros.
}

void SetDirection()
{  
  if(Direction){ // Si va en direccion horaria.
    Steps++;  // La seguencia avanza de forma incremental. 
  }
  else{
    Steps--;  // En otro caso la secuencia avanza de forma decremental.
  }
  Steps = ( Steps + 8 ) % 8 ; // Si llega a 8 se resetea la variable a 0, es decir:
  /*  
   *  Si steps = 0 -> (0 + 8) % 8 = 0.
   *  Si steps = 1 -> (1 + 8) % 8 = 1.
   *  Si steps = 2 -> (2 + 8) % 8 = 2.
   *              .
   *              .
   *  Si steps = 7 -> (7 + 8) % 8 = 7.
   *  Si steps = 8 -> (8 + 8) % 8 = 0.  <- Como la division es exacta el resto vale 0 (16/8 = 2 con resto = 0)
    */
}
