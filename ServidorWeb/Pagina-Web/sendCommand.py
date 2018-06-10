#!/usr/bin/python3

import serial, time
import cgi, cgitb

rcvcommand = "" 
<<<<<<< HEAD

form = cgi.FieldStorage()

rcvcommand = "Default MSG"

rcvcommand = form['command'].value

dataFile = open("Resources/data.txt", 'w') 
# dataFile.write(rcvcommand + " " + parametro1 + " " + parametro2 + " " + parametso3)
dataFile.write(rcvcommand)

print("Content-Type: text/html\n\n")
print("200 OK")

dataFile.close()
=======

form = cgi.FieldStorage()

rcvcommand = "Default MSG"

x_submit = "Default MSG"
y_submit = "Default MSG"
z_submit = "Default MSG"

rcvcommand = form['command'].value

if rcvcommand is "PICK BUTTON":

	Anuncio=open("Resources/anuncio.txt",'w')
	Anuncio.write('TRUE')
	Anuncio.close()
	#abrir archivo y escribit true y cerrar archivo 
else:
	dataFile = open("Resources/data.txt", 'w') 
	# dataFile.write(rcvcommand + " " + parametro1 + " " + parametro2 + " " + parametso3)
	dataFile.write(rcvcommand)
	dataFile.close()

if submit in form
	if request.method == 'GET':
		x_submit = form['coor_x'].value
		y_submit = form['coor_y'].value
		z_submit = form['coor_z'].value
	dataFile = open("Resources/data.txt", 'w')
	dataFile.write("arm s" + x_submit+" " + y_submit+" " + z_submit)
	dataFile.close()

print("Content-Type: text/html\n\n")
print("200 OK")


>>>>>>> ce4ae3557e6a83df3aa752d4b51e53421290cd4d


