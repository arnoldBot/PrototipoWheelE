#include <Servo.h>

// Pines del motor paso a paso
#define pin1 5 // FALTA ASIGNAR PINES
#define pin2 6
#define pin3 7
#define pin4 8
char motorOut[][4]={{1,0,0,0}, // Vector de valores a la salida de los pines
                   {1,1,0,0},
                   {0,1,0,0},
                   {0,1,1,0},
                   {0,0,1,0},
                   {0,0,1,1},
                   {0,0,0,1},
                   {1,0,0,1}};  
int tiempoAnterior = 0;
char pasos=0;
char ind=0;
int anguloActual=0; // <--Requiere calibración

Servo miservo0;
Servo miservo1;
Servo miservo2;
Servo efectorfinal;
int i =0;
int estado=0;
int color;	// rojo  = 0
		// verde = 1
		// Azul	 = 2
		// Amarillo = 3
float A;
float L0=30;	// No es necesario porque el sistema de referencia se pone en la primera articulación para facilitar calculos.
float L2=135;
float L3=150;	// Aproximadamente para que el objeto se encuentra dentro del efector
int x,y,z;
int coordenada[10];
float angulo0;
float angulo2;
float angulo3;
char unid,cent,dec;
int bufferChar;


void setup(){ 
  Serial.begin(9600);	//Para programar el puerto serie
  miservo0.attach(5);	//Arduino pone por defecto: para cero grados 544 y para 180 2400.
  miservo1.attach(6);	//Arduino pone por defecto: para cero grados 544 y para 180 2400.
  miservo2.attach(3);	//Arduino pone por defecto: para cero grados 544 y para 180 2400.
  efectorfinal.attach(10);	//Arduino pone por defecto: para cero grados 544 y para 180 2400.
}

void loop() {
 	//Maquina de estados:
 	switch(estado){
  	case 0: // Se colocara el brazo en una posicion intermedia para que no se produzca una configuración indeseada.
     	miservo0.write(0);//mirando hacia adelante
     	miservo1.write(60);
     	miservo2.write(0);
     	efectorfinal.write(0);//abierto;
     	estado++;
    	break;
   	case 1: // giro de la base para que el brazo se encuentre horientado hacia los depósitos
     	miservo0.write(0);//mirando hacia adelante
     	miservo1.write(60);
     	miservo2.write(0);
     	estado++;
   	 	break;      
  	case 2: // Si le llegan intrucciones desde el puerto serie leemos los datos y actuamos 
  	  while(i<3){ 
      	if(Serial.available()){ // Si hay bytes en el buffer RX leemos
      		bufferChar=Serial.parseInt();	// Recojemos el pimer numero de la cadena
      		coordenada[i]=bufferChar;	// Lo guardamos en un vector
      		i++;
      	}
      }
      i=0;
                        
      if(coordenada!=0){	// Si hay coordenada disponibles, lo asignamos al las variables del modelo inverso
       	x=coordenada[0];
       	y=coordenada[1];
       	z=coordenada[2];                                  
      } 
                                       
      Serial.print("x:");
      Serial.print(x);
      Serial.print("\t");
      Serial.print("y:");
      Serial.print(y);
      Serial.print("\t");
      Serial.print("z:");
      Serial.println(z);         
               
      //CALCULO DEL ANGULO BASE
      angulo0=(float)atan2(y,x)*180/3.14;

      //CALCULO ANGULO MUÑECA
      A=(pow(x,2)+pow(y,2)+pow(z,2)-pow(L2,2)-pow(L3,2))/(2*(L2)*(L3));
      angulo3=(float)atan2(sqrt(1-pow(A,2)),A)*180/3.1416;

      //CALCULO ANGULO CODO
      angulo2=((float)atan2(z,sqrt(pow(x,2)+pow(y,2)))-(float)atan2(L3*sin(angulo3),L2+L3*cos(angulo3)))*180/3.1416;

      delay(50); // Le damso un tuempo para que los servos puedan posicionarse.

      efectorfinal.write(180);	// Se abre el efector final

      if(1-pow(A,2)>0){
       	miservo0.write(angulo0);
       	miservo1.write(angulo2);
       	miservo2.write(angulo3);
       	Serial.print("angulo0:");
       	Serial.print(angulo0);
       	Serial.print("\t");
       	Serial.print("angulo2:");
       	Serial.print(angulo2);
      	Serial.print("\t");
       	Serial.print("angulo3:");
       	Serial.println(angulo3);
       	delay(2000);
       	estado++;
      }
      else{
        Serial.println("Punto geometricamente inalcanzable.Introduzca otras coordenadas o fabrique un mejor robot");
        estado=2;
      }
                                    
  		break;
    case 3://En este estado se cerrará el efector final para que coja el objeto.
      	//Mantenemos los angulos en las distintas articulaciones
      miservo0.write(angulo0);
      miservo1.write(angulo2);
      miservo2.write(angulo3);
      //cerramos el servo que se encarga de efector final
      efectorfinal.write(0);
      estado++;
      break;
    case 4: //En este estado se colocara el brazo en una posicion intermedia para que no se produzca una configuración indeseada.
      miservo0.write(0);//mirando hacia adelante
      miservo1.write(60);
      miservo2.write(0);
      efectorfinal.write(0);
      estado++;
      break;
    case 5: //giro de la base para que el brazo se encuentre horientado hacia los depósitos
      miservo0.write(180);//mirando hacia adelante
      miservo1.write(60);
      miservo2.write(0);
      efectorfinal.write(0);
      estado++;
      break;
    case 6:// procedemos a mover el brazo a una posición u otra dependiendo el color.
      if(color==0){//ROJO
       	x=x;
       	y=y;
       	z=z;  
      }
      if(color==1){//VERDE
       	x=x;
       	y=y;
       	z=z;
      }
      if(color==2){//AZUL
       	x=x;
      	y=y;
       	z=z;
      }
      if(color==3){//AMARILLO
       	x=x;
       	y=y;
       	z=z;
      }
      angulo0=(float)atan2(y,x)*180/3.14;
      //ANGULO muñeca
      A=(pow(x,2)+pow(y,2)+pow(z,2)-pow(L2,2)-pow(L3,2))/(2*(L2)*(L3));
      angulo3=(float)atan2(sqrt(1-pow(A,2)),A)*180/3.1416;
      //ANGULO CODO
      angulo2=((float)atan2(z,sqrt(pow(x,2)+pow(y,2)))-(float)atan2(L3*sin(angulo3),L2+L3*cos(angulo3)))*180/3.1416;
      delay(50);
      miservo0.write(angulo0);
      miservo1.write(angulo2);
      miservo2.write(angulo3);
      efectorfinal.write(0);
      estado++;
      delay(1000);
      break;
    case 7://abrimos el efector final
      miservo0.write(angulo0);
      miservo1.write(angulo2);
      miservo2.write(angulo3);
      efectorfinal.write(180);
      delay(1000);
      estado=0;     
    	break;
 	 }
}

void rotaBase(char angulo){
  pasos=(int)((angulo-anguloActual)/5.625);
  if(pasos<0){
    for(ind=7;ind>=pasos;ind--){
      digitalWrite(pin1,motorOut[ind][0]);
      digitalWrite(pin2,motorOut[ind][1]);
      digitalWrite(pin3,motorOut[ind][2]);
      digitalWrite(pin4,motorOut[ind][3]); 
      if(millis() - tiempoAnterior >= 50){
        if(ind==3){
          ind=0;
        }else{
          ind++;
        }
        tiempoAnterior = millis();
      } 
    }  
  }else if(pasos>0){
    for(ind=0;ind<=pasos;ind++){
      digitalWrite(pin1,motorOut[ind][0]);
      digitalWrite(pin2,motorOut[ind][1]);
      digitalWrite(pin3,motorOut[ind][2]);
      digitalWrite(pin4,motorOut[ind][3]); 
      if(millis() - tiempoAnterior >= 50){
        if(ind==3){
          ind=0;
        }else{
          ind++;
        }
        tiempoAnterior = millis();
      } 
    } 
  }
}

