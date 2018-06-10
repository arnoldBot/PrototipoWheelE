String buff = ""; 

void setup() 
{
  Serial.begin(9600);
  Serial.println("Arduino esta listo");
}

void loop() {
  if (Serial.available() > 0)
  {
    buff = Serial.readString();
    Serial.println(buff);
  }
  
}
