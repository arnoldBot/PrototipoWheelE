void setup() {
  Serial.begin(9600);
  Serial.println("Arduino esta listo");
}

void loop() {
  if(millis()%1000==0){
    Serial.print("Hola! sigo aqui. Tiempo: "); 
    Serial.println(millis()/1000);
    delay(10);
  }
}
