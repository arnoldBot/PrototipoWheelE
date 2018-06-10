
#include <Servo.h>
//es necesario implementar toda la botonera, configurar el estado de subida de escalones y además, el de movimiento normal. Por otro lado hay que nombrar a cada variable
//de tal modo (y generar una tabla de variables) que sea sencillo identificar cada una para los script de Python.
//w - recto
//q - avance hacia la izqda
//a - rotacion sobre si mismo sent antihorario
//z - retroceso haci la izquierda
//x - marcha atras
//c - retroceso hacia la dcha
//d - rotacion sobre si mismo sent horario
//e - avance hacia la dcha
//m - turbo(max.par)
//n - modo normal
//p - stop
//b - brazo

//LISTA DE ESTADOS
//0 - estado inicial con todos los  motores parados
// 1 - el coche se está moviendo hacia delante
// 2 - el coche se mueve hacia atrás
// 3 - el coche gira sobre si mismo en sentido horario
//4 - el coche gira sobre si mismo en sentido antihorario
//5 - el coche avanza hacia la izquierda
//6 - el coche avanza hacia la derecha
//7-el coche retrocede hacia la izquierda
//8 - el coche retrocede hacia la derecha
//9 - paramos el movimiento del coche
// 10 - pasamos al movimiento del brazo y detenemos el movimiento del coche


//DECLARACIÓN DE VARIABLES
#define Enable_pH 26 //Pin 1(Habilitación de cada puente H)

#define PWM_R1_IZQ 3 //PWM Rueda 1 Izquierda
#define PWM_R2_IZQ 5 //PWM Rueda 2 Izquierda
#define PWM_R3_IZQ 6 //PWM Rueda 3 Izquierda
#define PWM_R1_DER 9 //PWM Rueda 1 Derecha
#define PWM_R2_DER 10 //PWM Rueda 2 Derecha
#define PWM_R3_DER 11 //PWM Rueda 3 Derecha

#define DIG_R1_IZQ 53 //senal Digital Rueda 1 Izquierda
#define DIG_R2_IZQ 51 //senal Digital Rueda 2 Izquierda
#define DIG_R3_IZQ 49 //senal Digital Rueda 3 Izquierda
#define DIG_R1_DER 45 //senal Digital Rueda 1 Derecha
#define DIG_R2_DER 43 //senal Digital Rueda 2 Derecha
#define DIG_R3_DER 47 //senal Digital Rueda 3 Derecha

String ch; //en esta variable se almacenará la direccion que le pasemos por puerto serie.

//con estas variables guardamos la direccion en la que se estaba moviendo el coche para no estar enviando constantemente una tecla de direccion
char delante = 0; //tecla w
char atras = 0; //tecla x
char av_dcha = 0; //tecla e
char av_izqda = 0; //tecla q
char antihorario = 0; //tecla a
char horario = 0; //tecla d
char ret_izqda = 0; //tecla z
char ret_dcha = 0; //tecla c

char max_par = 0; //tecla m
char estado = 0;//declaramos la variable para la máquina de estados
char inicio = 0;
char deltax = 0; //distancia del objeto al centro de la pantalla en horizontal
char deltay = 0; //distancia del objeto al centro de la pantalla en vertical

char xerror = 0;
char yerror = 0;
//----------------------------------------------------------------------------------------------------

//DECLARACION DE VARIABLES DEL BRAZO
// La secuencia de pulsos que hay que dar a cada bobina del paso a paso para que gire el eje.
int Paso[8][4] =
{ {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

// Definimos los pines del paso a paso. Hay que cambiarlo en funcion de los disponibles del Ard.Mega
#define IN1  32
#define IN2  36
#define IN3  34
#define IN4  38

int steps_left = 4095; // Variable que contiene el numero de pasos quedan por dar.
boolean Direction = true; // Variable que indica la direccion, es decir, si la secuencia de pulsos va en un sentido o el otro.
int Steps = 0; // Define el paso actual de la secuencia.
int currentAngle = 0; // Indica el angulo actual del motor.
int angulo = 0; // Variable que contiene el angulo al que se quiere llegar.

Servo hombroServo;
Servo codoServo;
Servo efectorfinal;
Servo baseServo;

int i = 0;
int estadobrazo = 1;
int color = -1;  // rojo  = 0
// verde = 1
// Azul  = 2
// Amarillo = 3
float A;
float L0 = 55;  // No es necesario porque el sistema de referencia se pone en la primera articulación para facilitar calculos.
float L2 = 135;
float L3 = 150; // Aproximadamente para que el objeto se encuentra dentro del efector
int x, y, z;
int coordenada[10];
float angulo0 = 0;
float angulo2 = 90;
float angulo3 = 60;
float anguloEfector = 10;
int bufferChar;
int pos_servo_base;
int pos_servo_hombro;
int pos_servo_codo;


//----------------------------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // iniciamos el puerto serie

  //Declaramos los pines como salidas de Arduino.
  pinMode(PWM_R1_IZQ, OUTPUT);
  pinMode(PWM_R2_IZQ, OUTPUT);
  pinMode(PWM_R3_IZQ, OUTPUT);
  pinMode(PWM_R1_DER, OUTPUT);
  pinMode(PWM_R2_DER, OUTPUT);
  pinMode(PWM_R3_DER, OUTPUT);

  pinMode(DIG_R1_IZQ, OUTPUT);
  pinMode(DIG_R2_IZQ, OUTPUT);
  pinMode(DIG_R3_IZQ, OUTPUT);
  pinMode(DIG_R1_DER, OUTPUT);
  pinMode(DIG_R2_DER, OUTPUT);
  pinMode(DIG_R3_DER, OUTPUT);

  pinMode(Enable_pH, OUTPUT);//senal de enable de los puentes H

  //declaramos los pines del brazo
  // Definimos el modo de los pines.
  //  pinMode(IN1, OUTPUT);
  //  pinMode(IN2, OUTPUT);
  //  pinMode(IN3, OUTPUT);
  //  pinMode(IN4, OUTPUT);
  //
  delay(2000);
  codoServo.attach(11);              //Arduino pone por defecto: para cero grados 544 y para 180 2400.
  delay(2000);
  efectorfinal.attach(10);  //Arduino pone por defecto: para cero grados 544 y para 180 2400.
  delay(2000);
  hombroServo.attach(6);            //Arduino pone por defecto: para cero grados 544 y para 180 2400.
  delay(2000);
  baseServo.attach(5);            //Arduino pone por defecto: para cero grados 544 y para 180 2400.

  hombroServo.write(0);
  delay(500);
  hombroServo.write(90);
  delay(500);
  codoServo.write(0);
  delay(500);
  codoServo.write(90);
  delay(500);
  efectorfinal.write(10);
  delay(500);
  efectorfinal.write(10);
}


//------------------------------------------------------------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:

  switch (estado) { //iniciamos la máquina de estados

    case 0:

      if (inicio == 0) { //si la variable auxiliar inicio está a 0 separan los motores. Esto es así para que en caso deno pararlos con la pausa, Se estén moviendo de forma continuada tras pulsar una tecla de movimiento.


        salida_motores (LOW , 255, HIGH , 255, HIGH , 255, HIGH , 255, HIGH , 255, HIGH , 255, HIGH); //en el estado inicial todos los motores tienen que estar apagados
      }
      //paramos los motores del brazo

      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);



      analyseData();



      break;

    case 1: //si estamos moviendo hacia delante

      salida_motores (HIGH, 200, LOW, 200, LOW, 200, LOW, 200, LOW, 200, LOW, 200, LOW); //ponemos los motores hacia delante
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 2: //si se está moviendo hacia atrás

      salida_motores (HIGH , (255 - 200), HIGH , (255 - 200), HIGH , (255 - 200), HIGH , (255 - 200), HIGH , (255 - 200), HIGH , (255 - 200), HIGH); //ponemos los motores hacia atrás
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 3: //si gira en sentido horario

      salida_motores (HIGH , 128, LOW , 128, LOW, 128, LOW , (255 - 128), HIGH , (255 - 128), HIGH , (255 - 128), HIGH); //ponemos los motores en sentido horario
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 4: //si gira en sentido antihorario

      salida_motores (HIGH , (255 - 200), HIGH , (255 - 200), HIGH, (255 - 200), HIGH, 200, LOW , 200, LOW, 200, LOW);
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 5: //si avanza hacia la izquierda

      salida_motores (HIGH , 128, LOW , 128, LOW, 128, LOW , 255, LOW , 255, LOW , 255, LOW);

      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 6: //si avanza hacia la derecha

      salida_motores (HIGH , 255, LOW , 255, LOW, 255, LOW , 128, LOW , 128, LOW , 128, LOW);
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 7: //si retrocede hacia la izquierds

      salida_motores (HIGH , (255 - 128), HIGH , (255 - 128), HIGH, (255 - 128), HIGH , (255 - 255), HIGH , (255 - 255), HIGH , (255 - 255), HIGH);
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 8: //si retrocede hacia la derecha

      salida_motores (HIGH , (255 - 255), HIGH , (255 - 255), HIGH, (255 - 255), HIGH , (255 - 128), HIGH , (255 - 128), HIGH , (255 - 128), HIGH);
      inicio = 1; // ponemos esta variable a 1 para que al volver al estado inicial no se paren

      estado = 0;

      break;

    case 9: //si queremos parar

      inicio = 0;
      estado = 0;

      break;


    case 10: //si queremos testear el brazo

      break;

    case 11: //si queremos pasar al movimiento del brazo

      salida_motores (LOW , 255, HIGH , 255, HIGH , 255, HIGH , 255, HIGH , 255, HIGH , 255, HIGH); //por seguridad paramos todos los motores

      //METEMOS TODO EL CODIGO DEL BRAZO A LO YOLO :D

      //Maquina de estadobrazos:
      switch (estadobrazo) {
        case 1:                         // giro de la base para que el brazo se encuentre orientado hacia adelante
          delay(500);
          angulo0 = 0;             //Debido a su posición
          angulo2 = 90;              //añado 90 por la pos del servo
          angulo3 = 60;
          anguloEfector = 60;           //abierto
          estadobrazo++;
          break;
        case 2:                         // Si le llegan intrucciones desde el puerto serie leemos los datos y actuamos
          delay(500);            // Le damso un tuempo para que los servos puedan posicionarse.

          Serial.print("x:");
          Serial.print(x);
          Serial.print("\t");
          Serial.print("y:");
          Serial.print(y);
          Serial.print("\t");
          Serial.print("z:");
          Serial.println(z);

          //CALCULO DEL ANGULO BASE
          angulo0 = (float)atan2(y, x) * 180 / 3.14; //Le sumamos 220 para que se coloque mirando al frente debido a su posicion inicial

          //CALCULO ANGULO MUÑECA
          A = (pow(x, 2) + pow(y, 2) + pow(z, 2) - pow(L2, 2) - pow(L3, 2)) / (2 * (L2) * (L3));
          angulo3 = (float)atan2(sqrt(1 - pow(A, 2)), A);

          //CALCULO ANGULO CODO
          angulo2 = ((float)atan2(z, sqrt(pow(x, 2) + pow(y, 2))) + (float)atan2(L3 * sin(angulo3), L2 + L3 * cos(angulo3))) * 180 / 3.1416; // le he puesto un menos al principio debido al sentido de giro que esta puesto el servo
          angulo3 = angulo3 * 180 / 3.1416;
          anguloEfector = 60;
          if (1 - pow(A, 2) > 0) {
            Serial.print("angulo0:");
            Serial.print(angulo0);
            Serial.print("\t");
            Serial.print("angulo2:");
            Serial.print(angulo2);
            Serial.print("\t");
            Serial.print("angulo3:");
            Serial.println(angulo3);
            delay(1000);
            estadobrazo++;
          }
          else {
            Serial.println("Punto geometricamente inalcanzable.Introduzca otras coordenadas o fabrique un mejor robot");
            estadobrazo = 2;
          }

          break;


        case 3://En este estadobrazo se cerrará el efector final para que coja el objeto.
          //Mantenemos los angulos en las distintas articulaciones
          //cerramos el servo que se encarga de efector final
          delay(500);
          anguloEfector = 10;
          estadobrazo++;
          break;
        case 4: //En este estadobrazo se colocara el brazo en una posicion intermedia para que no se produzca una configuración indeseada.
          delay(500);
          angulo2 = 90;
          angulo3 = 60;
          anguloEfector = 10;
          estadobrazo++;
          break;
        case 5: //En este estadobrazo se colocara el brazo mirando a los depósitos.
          delay(500);
          angulo0 = 180;
          angulo2 = 90;
          angulo3 = 60;
          anguloEfector = 10;
          estadobrazo++;
          break;
        case 6:// procedemos a mover el brazo a una posición u otra dependiendo el color.
          Serial.println("Introduzca color:");

          //VOY A COMENTAR TODO ESTO YA QUE ES LO RELACIONADO AL COLOR DEL OBJETO Y ESTO AUN NO LO HEMOS IMPLEMENTADO :d
          
          //      if (Serial.available()) { // Si hay bytes en el buffer RX leemos
          //        color = Serial.parseInt();
          //        estadobrazo++;
          //      }
          //      if (color == 0) { //ROJO
          //        x = 110;
          //        y = 0;
          //        z = 20;
          //      }
          //      if (color == 1) { //VERDE
          //        x = 110;
          //        y = 30;
          //        z = 20;
          //      }
          //      if (color != -1) {
          //        angulo0 = 180 + (float)atan2(y, x) * 180 / 3.14;
          //
          //        //ANGULO muñeca
          //        A = (pow(x, 2) + pow(y, 2) + pow(z, 2) - pow(L2, 2) - pow(L3, 2)) / (2 * (L2) * (L3));
          //        angulo3 = (float)atan2(sqrt(1 - pow(A, 2)), A);
          //        //ANGULO CODO
          //        angulo2 = ((float)atan2(z, sqrt(pow(x, 2) + pow(y, 2))) + (float)atan2(L3 * sin(angulo3), L2 + L3 * cos(angulo3))) * 180 / 3.1416;
          //        angulo3 = angulo3 * 180 / 3.1416;
          //        color = -1;
          //      }
          //      break;
          //    case 7://abrimos el efector final
          //      delay(1000);
          //      anguloEfector = 50;
          //      estadobrazo = 1;
          //      break;
          //  }
          //  //efector final
          //  efectorfinal.write(anguloEfector);
          //  //Motor paso a paso de la base
          //  //Servo hombro
          //
          //  if (angulo2 > 85) {
          //    angulo2 = 85;
          //  }
          //  if (angulo2 < -30) {
          //    angulo2 = -30;
          //  }
          //  //Angulo codo
          //  if (angulo3 > 170) {
          //    angulo3 = 170;
          //  }
          //  if (angulo3 < 0) {
          //    angulo3 = 0;
          //  }
          //  posicionar();


          break;

      }

      break;

    case 12: //para entrar en el modo de posicionamiento automático

      if (xerror > 10) {

        estado = 3;
      }

      else if (xerror < -10) {

        estado = 4;
      }

      else {

        if (yerror > 0) {

          estado = 1;
        }

        else {

          Serial.write("Objeto centrado");

        }
      }

      break;

  }
}


void salida_motores (char senal_enable, char vel_R1_izqda, char senal_R1_izqda, char vel_R2_izqda, char senal_R2_izqda, char vel_R3_izqda, char senal_R3_izqda, char vel_R1_dcha, char senal_R1_dcha, char vel_R2_dcha, char senal_R2_dcha, char vel_R3_dcha, char senal_R3_dcha)
{
  digitalWrite(Enable_pH, senal_enable);

  analogWrite(PWM_R1_IZQ, vel_R1_izqda);
  digitalWrite(DIG_R1_IZQ, senal_R1_izqda);

  analogWrite(PWM_R2_IZQ, vel_R2_izqda);
  digitalWrite(DIG_R2_IZQ, senal_R2_izqda);

  analogWrite(PWM_R3_IZQ, vel_R3_izqda);
  digitalWrite(DIG_R3_IZQ, senal_R3_izqda);

  analogWrite(PWM_R1_DER, vel_R1_dcha);
  digitalWrite(DIG_R1_DER, senal_R1_dcha);

  analogWrite(PWM_R2_DER, vel_R2_dcha);
  digitalWrite(DIG_R2_DER, senal_R2_dcha);

  analogWrite(PWM_R3_DER, vel_R3_dcha);
  digitalWrite(DIG_R3_DER, senal_R3_dcha);
}

//===================================================================================== FUNCION GIRO SUAVE SERVOS
void posicionar() {  //mejor rango va de 20º a 160º
  posicionServo(pos_servo_hombro, 30 + angulo2, hombroServo);
  posicionServo(pos_servo_codo, angulo3, codoServo);
  posicionServo(pos_servo_base, angulo0 / 2, baseServo);
  pos_servo_codo = angulo3; //En angulo futuro pasa a ser el angulo actual.
  pos_servo_hombro = 30 + angulo2; //En angulo futuro pasa a ser el angulo actual.
  pos_servo_base = angulo0 / 2;
}
//====================================================================================
void posicionServo(int grado_ini, int grado_fin, Servo servo)
{
  int posicion;
  if (grado_ini < grado_fin)
  {
    for (posicion = grado_ini; posicion <= grado_fin; posicion += 1)
    {
      servo.write(posicion);                                   // posiciona el servo
      delay(30);

    }
  }
  if (grado_ini > grado_fin)
  {
    for (posicion = grado_ini; posicion >= grado_fin; posicion -= 1)
    {
      servo.write(posicion);                                   // posiciona el servo
      delay(30);

    }
  }
}


void analyseData() //con esta funcion vamos a analizar el contenido del mensaje enviado por la raspberry a través de la pagina web
{
  if (Serial.available())
  {


    ch = Serial.readString();
    Serial.println(ch);

    if (ch[0] == 'm' && ch[1] == 'o' && ch[2] == 'v') //si llega mov indica que entramos en los comandos de movimiento
    {

      if (ch[4] == 'w') { //mov w nos movemos hacia delante

        estado = 1;
      }

      else if (ch[4] == 'x') { //mov x nos movemos hacia atras.

        estado = 2;
      }

      else if (ch[4] == 'd') {

        estado = 3;
      }

      else if (ch[4] == 'a') {

        estado = 4;

      }

      else if (ch[4] == 'q') {

        estado = 5;
      }

      else if (ch[4] == 'e') {
        estado = 6;

      }

      else if (ch[4] == 'z') {

        estado = 7;

      }

      else if (ch[4] == 'c') {

        estado = 8;

      }

      else if ( ch[4] == 'p') {

        estado = 9;
      }
    }

    else if (ch[0] == 'a' && ch[1] == 'r' && ch[2] == 'm') //si nos llega la instruccion arm, indica que el siguiente parametro pertenece al brazo
    {
      if (ch[4] == 't') //si nos llega arm t indica que entramos en modo testeo del brazo
      {
        estado = 10;
      }

      else if (ch[4] == 's') { //si llega arm s indica que las coordenadas que pasamos son manuales por la pagina web

        estado = 11;
        x=ch[6]*100+ch[7]*10+ch[8];
        y=ch[10]*100+ch[11]*10+ch[12];
        z=ch[14]*100+ch[15]*10+ch[16];

      }

      else if (ch[4]=='b')
    }

    else if ( ch[0] == 'e' && ch[1] == 'r' && ch[2] == 'r') // si llega err indica que entramos en el modo de ajustar automaticamente el coche
  {
       estado = 12;
       xerror=ch[4]*100+ch[5]*10+ch[6]*1;
       yerror=ch[8]*100+ch[9]*10+ch[10]*1;
    }
  }
}
