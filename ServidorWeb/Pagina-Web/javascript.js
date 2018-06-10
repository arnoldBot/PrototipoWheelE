function loadXMLDoc(url) {
	var xmlhttp;
	var txt;
        xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			txt = xmlhttp.responseText; // alert(txt);
			document.getElementById('consoleArea').innerHTML = txt;
			document.getElementById("consoleArea").scrollTop = document.getElementById("consoleArea").scrollHeight 
		}
	}
	xmlhttp.open("GET", url, true);
	xmlhttp.send();
}
$(document).ready(function(){
	var video = document.querySelector("#videoElement");
	navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia || navigator.oGetUserMedia;
	if (navigator.getUserMedia) {       
	    navigator.getUserMedia({video: true}, handleVideo, videoError);
	}
	function handleVideo(stream) {
	    video.src = window.URL.createObjectURL(stream);
	}
	function videoError(e) {
	    // do something
	}	    
	
 	$.ajax({
 		url: "comunicationHandler.py",
// 		success: function(response) {
// 	 		console.log("serialLogger.py script executed sucessfully");
// 		},
// 		error: function(jqXHR, textStatus, errorThrown){
// 			console.log("Error executing serialLogger.py script");
//       	console.log(errorThrown);
// 		} 
	});
});

setInterval(function(){
    loadXMLDoc("Resources/arduinoData.txt")
}, 1000);

function sendCommand(parametro){
	$.ajax({
		method: "GET",
		url: "sendCommand.py",
		data:{
			'command' : parametro},
		dataType: "json",
			
//		success: function(response){
//			console.log("Comando enviado");
//     			 },
//		error: function(errorThrown){
//			console.log(errorThrown);
//			}
	});
}
