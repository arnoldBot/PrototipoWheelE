INSTRUCCIONES DE INSTALACIÓN Y PREPARACIÓN DE UN SERVIDOR WEB EMPLEANDO APACHE Y RASPBERRY PI

REQUISITOS PREVIOS
1-  Descargar de la página oficial de Raspberry Pi la distro de Jessie:
    https://www.raspberrypi.org/downloads/raspbian/

2-  Descargar la herramienta Rufus, para grabar imagenes iso en una SD:
    https://rufus.akeo.ie/?locale=es_ES

3-  Es NECESARIO emplear una tarjeta SD mayor a 8gb. (Formateada)
---------------------------------------------------------------------------------------------
INSTALACIÓN DE APACHE
4-  Grabamos la imagen de Jessie en nuestra SD y, posteriormente, introducimos la SD en la 
    Raspberry Pi y conectamos la alimentacion, una salida HDMI, un teclado y un ratón.
5-  Una vez se haya iniciado la Raspberry, abrimos la terminal y escribimos:
    "$ sudo raspi-config". Nos saltará otra interfaz, buscamos "Interfacing Options" y lo
    abrimos, ahí seleccionamos Camera, SSH y Serial.
    Vamos a atrás y le damos a "Advance Options" y seleccionamos "Expand Filesystem".
    Le damos a finish y la Raspberry Pi se reiniciará automaticamente.
6-  Posteriormente, de vuelta en el escritorio de Jessie, abrimos la terminal y escribimos:
    "$ sudo apt-get update".
7-  A continuación tecleamos: "$ sudo apt-get install apache2"
8-  Una vez instalado Apache en la Raspberry Pi, conviene comprobar si el servidor funciona.
    Para ello, necesitaremos un ordenador o dispositivo móvil, y conocer la ip de la Raspberry.
    Para comprobarlo, abrimos el navegador e introducimos en el browser la ip de la Raspberry
    y debería aparecer la página de Apache.

CONFIGURACIÓN DE APACHE
9-  Para configurarlo, en primer lugar debemos activar el cgi mediante: "$ sudo a2enmod cgi".
    Posteriormente, debemos configurar el directorio html, el cúal se encuentra en 
    /var/www/html/ para que pueda ejecutar scripts CGI. Para ello, debemos añadir las
    siguientes lineas de código al fichero apache2.config. (Éste fichero se encontrará dentro
    de /etc (creo), para abrirlo bastaría con: "$ sudo nano apache2.config".):

		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		~	ScriptAlias  /cgi-bin/ /var/www/html/   ~
		~                                               ~
		~		<Directory /var/www/html/>      ~
		~    	    Options +ExecCGI                    ~
		~    	    AddHandler cgi-script .cgi .py      ~
		~		</Directory>                    ~
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   Una vez añadido, guardamos el archivo y reiniciamos la raspberry mediante: "$ sudo shutdown
   now".

ASIGNACIÓN DE PERMISOS
9-  Por defecto, el usuario apache estará dentro del grupo www-data, para tener los mismos
    permisos que apache, debemos añadir nuestro usuario, pi, a dicho grupo, es decir, www-data.
    Para ello, emplearemos el comando: "$ sudo adduser pi www-data".
10- A continuación, debemos hacer que el dueño del directorio /var/www/ pertenezca a al grupo
    www-data. Para ello, emplearemos: "$ sudo chown -R www-data:www-data /var/www".
    Ahora que hemos cambiado de dueño vamos a darle todo tipo de permimos al nuevo, para ello
    emplearemos el comando: "$sudo chmod -R g+rwX /var/www".
11- Por otro lado, debemos darle permisos de apertura, lectura y escritura a los archivos 
    (o carpetas) cgi que queremos que la página ejecute. Para ello emplearemos el comando:
    "$ sudo chmod 777 <nombrefichero.loquesea> (Hay que darselo a todos los archivos, es
    importante no olvidarlo).
12- Además, también es necesario darle permisos al grupo www-data para poder abrir los
    puertos. 
    Para ello, emplearemos: "$ sudo usermod -a -G dialout www-data".

TRANSFERENCIA DE DATOS CREADOS PARA LA WEB (CSS,HMTL,JS,ETC.)
13-  De vuelta a nuestro ordenador,debemos transferimos los datos creados en para la página web
    a la Raspberry, para ello emplearemos la herramienta "Filezilla". Para instalarla, abrimos
    la terminal y tecleamos: "$ sudo apt-get install filezilla".
14- Una vez dentro del programa, rellenamos los siguientes campos:
		host: <ip raspberry>
		username: <pi>
		password: <contraseña de la rasp> (Por defecto, pass=raspberry)
		port: <22>
15- Una vez nos hayas conectado a la Raspberry Pi mediante ssh empleando Filezilla, copiamos el
    contenido de la carpeta <Servidor_web> dentro de la ruta "/var/www/html/".
----------------------------------------------------------------------------------------
      DE ÉSTE MODO QUEDARÁ TOTALMENTE OPERATIVA LA RASPBERRY PI CON EL SERVIDOR PARA TRABAJAR
      CON ELLA.

----------------------------------------------------------------------------------------
ANEXO: ANTE POSIBLES PROBLEMAS CON PYTHON

Asegurar que el directorio de python es correcta en la primera lina de los scripts:
	para ello hacemos miramos si estan la libreria pySerial instalada correctamnte con:
	
		~$pip list 		// Si es python
			ó 
		~$pip3 list		// Si es python3
				
	si no esta la instalamos con:
	
		~$pip install <nombre del paquete> (la que necesitamos es pySerial) // Si es python
			ó
		~$pip3 install <nombre del paquete>		// Si es python3

	segun en donde este instalado (python3 o python) hacemos lo siguiente:
	
		~$which python
			ó 
		~$which python3
		
	nos aparecera el directorio del interprete de python
	y dentro de los scripts CGI de python ponemos en la primera linea:
	
		#!<RUTA DEL INTERPRETE> 


