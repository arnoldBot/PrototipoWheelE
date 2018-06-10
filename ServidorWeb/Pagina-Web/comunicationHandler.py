#!/usr/bin/python3
import serial
import os
import cgi
import time

port = '/dev/ttyACM0' # Nombre del puerto serie

open("Resources/arduinoData.txt", 'w').close()
open("Resources/data.txt", 'w').close()

arduinoDataFile = open("Resources/arduinoData.txt", 'a')
arduinoDataFile.write("Comunications Handler running...\n")
arduinoDataFile.close()

try:
        puertoArdu = serial.Serial(port, 9600,timeout=0.1)
        arduinoDataFile = open("Resources/arduinoData.txt", 'a')
        arduinoDataFile.write("Connection to serial port established succesfully!\n")
        arduinoDataFile.close()
except Exception as e:
        print(e)

while 1:
        if(os.path.getsize("Resources/data.txt") > 0):
                dataFile = open("Resources/data.txt", 'r')
                data = dataFile.readline()
                puertoArdu.write(bytes(data, 'UTF-8'))
                dataFile.close()
                open("Resources/data.txt", 'w').close()
        buffer = puertoArdu.readline().decode("ascii")
        arduinoDataFile = open("Resources/arduinoData.txt", 'a')
        arduinoDataFile.write(buffer)
        arduinoDataFile.close()

puertoArdu.close()


