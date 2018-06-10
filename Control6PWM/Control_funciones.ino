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
//s - stop

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

char ch; //en esta variable se almacenará la direccion que le pasemos por puerto serie.

//con estas variables guardamos la direccion en la que se estaba moviendo el coche para no estar enviando constantemente una tecla de direccion
int delante=0; //tecla w
int atras=0; //tecla x
int av_dcha=0; //tecla e
int av_izqda=0; //tecla q
int antihorario=0; //tecla a
int horario=0; //tecla d
int ret_izqda=0; //tecla z
int ret_dcha=0; //tecla c

int max_par=0; //tecla m

//----------------------------------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); // iniciamos el puerto serie
  
  //Declaramos los pines como salidas de Arduino.
  pinMode(PWM_R1_IZQ,OUTPUT); 
  pinMode(PWM_R2_IZQ,OUTPUT);  
  pinMode(PWM_R3_IZQ,OUTPUT);
  pinMode(PWM_R1_DER,OUTPUT);
  pinMode(PWM_R2_DER,OUTPUT);
  pinMode(PWM_R3_DER,OUTPUT);

  pinMode(DIG_R1_IZQ,OUTPUT); 
  pinMode(DIG_R2_IZQ,OUTPUT);  
  pinMode(DIG_R3_IZQ,OUTPUT);
  pinMode(DIG_R1_DER,OUTPUT);
  pinMode(DIG_R2_DER,OUTPUT);
  pinMode(DIG_R3_DER,OUTPUT);  
  
  pinMode(Enable_pH, OUTPUT);//senal de enable de los puentes H

}

//------------------------------------------------------------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:
  
  //Vamos a crear 6 posibilidades de movimiento: al pulsar 'W' avanzará hacia delante, al pulsar 'S' retrocederá, al pulsar la
  //tecla 'A' girará sobre si mismo en sentido antihorario, al pulsar 'D' girará sobre sí mismo en sentido horario, al pulsar
  //la tecla 'Q', avanzará hacia la izquierda y al pulsar 'E' avanzará hacia la derecha.
  
if(Serial.available()) //si el puerto serie esta libre
{
    ch=Serial.read(); //almacenamos el valor que se lea del puerto serie.

        if(ch=='w' && not(( delante==1 && ch!='w'))) // si pulsamos hacia delante o se estaba moviendo hacia delante
        {
          delante=1; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c          
          max_par=0; //tecla m        
          salida_motores (HIGH, 200,LOW, 200,LOW, 200,LOW, 200,LOW, 200,LOW, 200,LOW);   
        }
    
        else if (ch== 'x'  && not(( atras==1 && ch!='x'))) // si pulsamos 'X' hacia atras o se estaba moviendo hacia atras
        { 
          delante=0; //tecla w
          atras=1; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c          
          max_par=0; //tecla m          
          salida_motores (HIGH ,(255-200),HIGH ,(255-200),HIGH ,(255-200),HIGH ,(255-200),HIGH ,(255-200),HIGH ,(255-200),HIGH);
        }
        
      else if( ch =='d' && not(( horario==1 && ch!='d'))) // si pulsamos 'D', girará sobre sí mismo en sentido horario.
      { 
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=1; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c          
          max_par=0; //tecla m
          salida_motores (HIGH ,128,LOW ,128,LOW, 128,LOW ,(255-128),HIGH ,(255-128),HIGH ,(255-128),HIGH);        
      }
    
      else if(ch =='a' && not(( antihorario==1 && ch!='a')))   // si pulsamos 'A', girará sobre sí mismo en sentido antihorario.
      {
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=1; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c          
          max_par=0; //tecla m
          salida_motores (HIGH ,(255-200),HIGH ,(255-200), HIGH,(255-200), HIGH, 200,LOW ,200,LOW, 200,LOW);        
      }    
     
      else if(ch =='q' && not(( av_izqda==1 && ch!='q'))) //Al pulsar 'Q' avanzará hacia la izquierda.
      { 
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=1; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c          
          max_par=0; //tecla m
          salida_motores (HIGH ,128,LOW ,128,LOW, 128,LOW ,255,LOW ,255,LOW ,255,LOW);
      }

      else if(ch =='e' && not(( av_dcha==1 && ch!='e')))  //Al pulsar 'E', avanzará hacia la derecha
      {   
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=1; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c
          max_par=0; //tecla m
          salida_motores (HIGH ,255,LOW ,255,LOW, 255,LOW ,128,LOW ,128,LOW ,128,LOW);
      }
      
      else if(ch =='z' && not(( ret_izqda==1 && ch!='z')))  //Al pulsar 'Z', retrocederá hacia la izquierda
      {   
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=1; //tecla z
          ret_dcha=0; //tecla c 
          max_par=0; //tecla m
          salida_motores (HIGH ,(255-128),HIGH ,(255-128),HIGH, (255-128),HIGH ,(255-255),HIGH ,(255-255),HIGH ,(255-255),HIGH);
      }
      
      else if(ch =='c' && not(( ret_dcha==1 && ch!='c')))  //Al pulsar 'C', retrocederá hacia la derecha
      {   
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=1; //tecla c          
          max_par=0; //tecla m
          salida_motores (HIGH ,(255-255),HIGH ,(255-255),HIGH,(255-255),HIGH ,(255-128),HIGH ,(255-128),HIGH ,(255-128),HIGH);
      }

      else if(ch =='m' && not(( max_par==1 && ch!='m')))  //Al pulsar 'M', intentaremos obtener el maximo par de las ruedas
      {   
          delante=0; //tecla w
          atras=0; //tecla x
          av_dcha=0; //tecla e
          av_izqda=0; //tecla q
          antihorario=0; //tecla a
          horario=0; //tecla d
          ret_izqda=0; //tecla z
          ret_dcha=0; //tecla c          
          max_par=1; //tecla m
          salida_motores (HIGH , 65, LOW , 120 ,LOW, 230 , LOW,65 ,LOW, 120 ,LOW, 230 ,LOW);
      }

       else if(ch =='p') // si pulsamos 'P' se parará el vehículo.
      { 
          salida_motores (LOW ,255,HIGH ,255,HIGH ,255,HIGH ,255,HIGH ,255,HIGH ,255,HIGH);       
      }
  }
}


//----------------------------------------------------------------------------------------------------------------------
//FUNCION PARA OPTIMIZAR EL CÓDIGO DE SALIDA DEL MOTOR

//Esta función está basada en la tabla de verdad del motor y controlandolo mediante una señal PWM y otra lógica.
//http://www.iescamp.es/miarduino/2016/02/21/2-motores-de-cc-velocidad-y-cambio-de-sentido/ en este enlace aparece la tabla de verdad y el porqué de las cosas.
//ADEMÁS, LA TABLA DE VERDAD SE ENCONTRARÁ EN EL .TXT DE GIT DÓNDE SE ENCUENTRAS LAS VARIABLES DEL SISTEMA DE CONTROL DE MOVIMIENTO.

void salida_motores (char senal_enable,char vel_R1_izqda,char senal_R1_izqda,char vel_R2_izqda,char senal_R2_izqda,char vel_R3_izqda,char senal_R3_izqda,char vel_R1_dcha,char senal_R1_dcha,char vel_R2_dcha,char senal_R2_dcha,char vel_R3_dcha,char senal_R3_dcha)
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
