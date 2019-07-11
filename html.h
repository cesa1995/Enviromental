String LoadPage(int type){
  String html;
  html="<!DOCTYPE html><html lang=\"en\">\n";
  html+="<style>h1,p{left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); position: relative; width:300px;}\n";
  if(type==0){
    html +="a{ position: relative; display: block; text-decoration:none; color: white; margin-top: 4px; padding: 5px; border-radius: 4px;}\n";
    html +=".links{ left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); position: relative; display: block; width:300px;}\n";
    html +=".formulario{ position: relative;}\n";
    html+=".conf{background-color: purple;}";
    html+=".file{background-color: green;}";
    html+=".del{background-color: #b03a2e;}";
    html+=".reboot{background-color:  #148f77;}";
  }else if(type==1 || type==2 || type==3 || type==4 || type==5){
    html+="form { left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); position: relative; width:300px;}\n";
    html+="form input, select ,textarea{ -webkit-appearance:none; -moz-appearance:none; appearance:none; color: #000000; width:100%; position:relative; padding:13px 10px 12px;"; 
    html+="margin:10px auto; border:none; -webkit-box-shadow:0 0 0 2px #cccccc; box-shadow:0 0 0 2px #cccccc; border-radius:5px;}\n";
    html+="form input:focus , select:focus, textarea:focus{ -webkit-box-shadow:0 0 0 2px #9204b2; box-shadow:0 0 0 2px #9204b2; outline:none;}\n";
    html+="form input[type=\"submit\"] { width:270px; background:rgb(147,200,71); color:#fff; font-size:14px; padding:10px; -webkit-box-shadow:0 0 0 0; box-shadow:0 0 0 0;}\n";
    html+="form input[type=\"submit\"]:focus { -webkit-box-shadow:none; box-shadow:none; margin :0 auto;}\n";
    html+="a{ left: 50%; -webkit-transform: translateX(-50%); background-color: purple; transform: translateX(-50%); width: 300px; position: relative; display: block; text-decoration:none; color: white;";
    html+="margin-top: 4px; padding: 5px; border-radius: 4px;}\n";
  }else if(type==6 || type==7 || type==8 || type==9 || type==10){
    html+="a{ left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); width: 300px; position: relative; display: block; text-decoration:none; color: white;";
    html+="background-color: purple; margin-top: 4px; padding: 5px; border-radius: 4px;}\n";
    }
  html+="a:hover{ background-color: black;}\n";
  html+="</style>\n";
  html+="<head><meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n";  
  html +="<title>";
  switch(type){
      case 0:{
        html+="Principal";
      }break;
      case 1:{
        html+="GSMSettings";  
      }break;
      case 2:{
        html+="WifiSTASettings";
      }break;
      case 3:{
        html+="WifiAPSettings";
      }break;
      case 4:{
        html+="Reloj";
      }break;
      case 5:{
        html+="Sleep";
      }break;
      case 6:{
        html+="Saved";
      }break;
      case 7:{
        html+="Error";
      }break;
      case 8:{
        html+="Reinicio";
      }break;
      case 9:{
        html+="Borrado";
      }break;
      case 10:{
        html+="Borrado";
      }break;
    }
  html+="</title>\n";
  html+="</head>\n";
  switch(type){
      case 0:{
        html +="<body>\n";
        html +="<div class=\"formulario\">\n";
        html +="<h1>Configurar</h1><p>Elija la opcion a configurar</p>\n";
        html +="<div class=\"links\">";
        html +="<a class=\"conf\" href=\"apn\">APN del GSM</a><a class=\"conf\" href=\"sta\">Wifi Estacion</a><a class=\"conf\" href=\"ap\">Wifi Punto de Acceso</a><a class=\"conf\" href=\"/reloj\">Reloj</a><a class=\"conf\" href=\"/time\">Tiempo de Descanso</a>";
        html +="<a class=\"file\" href=\"/?download="+String(Log)+"\">Descargar Datos</a><a class=\"file\" href=\"/log\">Logs</a>";
        html +="<a class=\"del\" href=\"/borrar?type=0\">Borrar Datos</a><a class=\"del\" href=\"/borrar?type=1\">Borrar Configuracion</a><a class=\"del\" href=\"/borrar?type=2\">Borrar Logs</a>";
        html +="<a class=\"reboot\" href=\"reboot?Mode=1\">Reiniciar Modo Wifi-STA</a><a class=\"reboot\" href=\"/reboot?Mode=2\">Reiniciar Modo GSM</a><a class=\"reboot\" href=\"/reboot?Mode=3\">Riniciar modo sin conexion</a></div>\n";
        html +="<div class=\"links\">\n";
        html +="<p><strong>GSM-SIM</strong></p><p><strong>Apn:</strong> "+String(sim.apn)+"\n <strong>Usuario:</strong> "+String(sim.user)+"\n <strong>Contrase&ntilde;a:</strong> "+String(sim.pass)+"</p>\n";
        html +="<p><strong>Wifi-STA</strong></p><p><strong>SSID:</strong> "+String(STA.ssid)+" <strong>Contrase&ntilde;a:</strong> "+String(STA.pass)+"</p>\n";
        html +="<p><strong>Wifi-AP</strong></p><strong>SSID:</strong> "+String(AP.ssid)+" <strong>Contrase&ntilde;a:</strong> "+String(AP.pass)+"<p></p>\n";
        html +="<p><strong>Tiempo</strong></p><p>"+getTiempo()+"</p>\n";
        html +="<p><strong>Intervalo</strong></p><p>"+String(TIME_TO_SLEEP)+" Segundos</p>\n";
        html +="<p><strong>Tipo de trabajo guardado</strong></p><p>";
        switch(ModeTemp){
          case 0:{html += "Servidor";}break;
          case 1:{html += "Wifi";}break;
          case 2:{html += "GSM";}break;
          case 3:{html += "Sin Conexion";}break;
        }
        html +="</p>\n";
        html +="</div></div></body></html>\n";
      }break;
      case 1:{
        html+="<body>\n";
        html+="<div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar apn del GSM</h1><p>Ingresa el apn, usuario y contrase&ntilde;a. Si el operador no requiere usuario ni contraseña dejar en blanco</p>\n";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"text\" name=\"APN\" placeholder=\"APN\" autofocus required>\n";
        html+="<input type=\"text\" name=\"USUARIO\" placeholder=\"USUARIO\">\n";
        html+="<input type=\"text\" name=\"PASSWORD\" placeholder=\"PASSWORD\">\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"1\">\n";
        html+="</form></div></body></html>";
      }break;
      case 2:{
        html+="<body><div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar Credenciales Wifi-STA</h1><p>Configure las credenciales del punto de acceso a conectar. si el punto de acceso no contiene contrase&ntilde;a deje en blanco.</p>\n";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"text\" name=\"SSID\" placeholder=\"SSID\" autofocus required>\n";
        html+="<input type=\"text\" name=\"PASSWORD\" placeholder=\"PASS\">\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"2\">\n";
        html+="</form></div></body></html>";
      }break;
      case 3:{
        html+="<body><div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar credenciales Wifi-AP</h1><p>Configure las credenciales del dispositivo como punto de acceso para que pueda conectarse y realizar las configuraciones correspondiente. Si desea una red libre deje la contrase&ntilde;a en blanco.</p>";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"text\" name=\"SSID\" placeholder=\"SSID\" autofocus required>\n";
        html+="<input type=\"text\" name=\"PASSWORD\" placeholder=\"PASS\">\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"3\">\n";
        html+="</form></div></body></html>";
      }break;
      case 4:{
        html+="<body><div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar el Reloj del dispositivo</h1><p>Configure el reloj para usarlo sin ninguna conexion a internet.</p>";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"date\" name=\"date\" autofocus required>\n";
        html+="<input type=\"time\" name=\"time\" value=\"00:00\" required>\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"4\">\n";
        html+="</form></div></body></html>";
        }break;
      case 5:{
        html+="<body><div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar Tiempo de Descanso</h1><p>Configurar el tiempo de descanso del dispositivo para tomar datos, por defecto 1 Minuto.</p>";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"number\" name=\"time\" placeholder=\"Minutos\" autofocus required>\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"5\">\n";
        html+="</form></div></body></html>";
        }break;
      case 6:{
        html+="<body><div class=\"formulario\"><a href=\"/\">Regresar</a><h1>Configurado..!</h1></div></body></html>";
      }break;
      case 7:{
        html+="<body><div class=\"formulario\"><a href=\"/\">Regresar</a><h1>ESP32 Error</h1><p>"+error+"</p></div></body></html>";
      }break;
      case 8:{
        html+="<body><div class=\"formulario\"><a href=\"/\">Regresar</a><h1>El equipo ha sido reiniciado.</h1></div></body></html>";
      }break;
      case 9:{
        html+="<body><div class=\"formulario\"><a href=\"/\">Regresar</a><h1>Borrado..!</h1></div></body></html>";
      }break;
      case 10:{
        html+="<body><div class=\"formulario\"><a href=\"/\">Regresar</a><h1>No existe el archivo.</h1></div></body></html>";
      }break;
        
    }
    return html;
  }
