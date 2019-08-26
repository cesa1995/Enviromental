String LoadPage(int type){
  String html;
  html="<!DOCTYPE html><html lang=\"en\">\n";
  html+="<style>\n";
  if(type!=0){
    html +="h1,p{left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); position: relative; width:300px;}\n";
  }
  if(type==0){
    html +="*{margin: 0; padding: 0;}";
    html +=".checkbox{display: none;}";
    html +="header{position: absolute; width: 100%; z-index: 10;}";
    html +=".header-container{width: 100%; position: absolute; right: 0; top: 1rem; left: 1rem; display: flex; justify-content: space-between; align-items: center ; float: right;}";
    html +=".btn-label{ width: 2rem; height: 1.4rem;}";
    html +=".header-button{ width: 2rem; height: 0.2rem; background: #000; position: absolute; transition: all 0.2s;}";
    html +=".header-button::before, .header-button::after{content: \"\"; width: 2rem; height: 0.2rem; background: #000; position: absolute; top: -0.6rem; transition: all 0.2s;}";
    html +=".header-button::after{top:0.6rem;}";
    html +=".menu{background: none; width: 200px; height: 27rem; position: absolute; z-index: 9; top: -35rem; margin-left: 5px; display: flex; flex-direction: column; justify-content: space-around; text-align: center; padding-top: 2rem; transition: all 0.5s;}";
    html +="ul {list-style: none;}";
    html +="li{ margin: 0.5rem;}";
    html +="a{ width: 200px; border: 1px solid #000; text-decoration:none; color: #fff; padding: 5px; border-radius: 5px; display: block;}";
    html +=".checkbox:checked ~ .menu { background: none; top: 0rem;}";
    html +=".checkbox:checked + header .header-container label .header-button { background: none;}";
    html +=".checkbox:checked + header .header-container label .header-button::before { transform: rotate(35deg); top: 0;}";
    html +=".checkbox:checked + header .header-container label .header-button::after { transform: rotate(-35deg); top: 0;}";
    html +=".titulo{ margin-left: 5rem;}";
    html +=".caja{ color: white; background: black; width: 300px; padding: 10px; margin: 5px; border-radius: 10px;}";
    html +=".links{ position: relative; display: block;}";
    html +=".formulario{ margin-left: 5rem; display: flex;}";
    html +=".conf{ background-color: purple;}";
    html +=".file{ background-color: green;}";
    html +=".del{ background-color: #b03a2e;}";
    html +=".reboot{ background-color:  #148f77;}";
    html +="a:hover{ background-color: black;}";
    html +="@media screen and (max-width: 750px){ .formulario{ display:block; }}";
  }else if(type==1 || type==2 || type==3 || type==4 || type==5 || type==11){
    html+="form { left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); position: relative; width:300px;}\n";
    html+="form input, select ,textarea{ -webkit-appearance:none; -moz-appearance:none; appearance:none; color: #000000; width:100%; position:relative; padding:13px 10px 12px;"; 
    html+="margin:10px auto; border:none; -webkit-box-shadow:0 0 0 2px #cccccc; box-shadow:0 0 0 2px #cccccc; border-radius:5px;}\n";
    html+="form input:focus , select:focus, textarea:focus{ -webkit-box-shadow:0 0 0 2px #9204b2; box-shadow:0 0 0 2px #9204b2; outline:none;}\n";
    html+="form input[type=\"submit\"] { width:270px; background:rgb(147,200,71); color:#fff; font-size:14px; padding:10px; -webkit-box-shadow:0 0 0 0; box-shadow:0 0 0 0;}\n";
    html+="form input[type=\"submit\"]:focus { -webkit-box-shadow:none; box-shadow:none; margin :0 auto;}\n";
    html+="a{ left: 50%; -webkit-transform: translateX(-50%); background-color: purple; transform: translateX(-50%); width: 300px; position: relative; display: block; text-decoration:none; color: white;";
    html+="margin-top: 4px; padding: 5px; border-radius: 4px;}\n";
  }else if(type==6 || type==7 || type==8 || type==9 || type==10 ){
    html+="a{ left: 50%; -webkit-transform: translateX(-50%); transform: translateX(-50%); width: 300px; position: relative; display: block; text-decoration:none; color: white;";
    html+="background-color: purple; margin-top: 4px; padding: 5px; border-radius: 4px;}\n";
    }
  html+="a:hover{ background-color: black;}\n";
  html+="</style>\n";
  html+="<head><meta charset=\"UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=0.7, user-scalable=no\">\n<meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n";
  if(type==12){
    html+="<meta http-equiv=\"Refresh\" content=\"10; url=/\" />";  
  }
  if(type==0){
    html+="<meta http-equiv=\"refresh\" content=\"30\">\n";
  }  
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
  html+="</head>\n<body>\n";
  switch(type){
      case 0:{
        html +="<input type=\"checkbox\" id=\"btn-nav\" class=\"checkbox\"> \n <header> \n <div class=\"header-container\"> \n <label for=\"btn-nav\" class=\"btn-label\"> \n <div class=\"header-button\"></div> \n </label> \n </div> \n </header>";
        html +="<nav class=\"menu\"> \n <ul> \n";
        html +="<a class=\"conf\" href=\"apn\">APN del GSM</a> \n <a class=\"conf\" href=\"sta\">Wifi Estacion</a> \n <a class=\"conf\" href=\"ap\">Wifi Punto de Acceso</a> \n <a class=\"conf\" href=\"/reloj\">Reloj</a> \n <a class=\"conf\" href=\"/time\">Tiempo de Descanso</a>";
        html +="<a class=\"file\" href=\"/?download=/Data.txt\">Descargar Datos</a> \n <a class=\"file\" href=\"/log\">Logs</a> \n <a class=\"del\" href=\"/borrar?type=0\">Borrar Datos</a> \n <a class=\"del\" href=\"/borrar?type=1\">Borrar Configuracion</a> \n <a class=\"del\" href=\"/borrar?type=2\">Borrar Logs</a>";
        html +="<a class=\"reboot\" href=\"reboot?Mode=1\">Reiniciar Modo Wifi-STA</a> \n <a class=\"reboot\" href=\"/reboot?Mode=2\">Reiniciar Modo GSM</a> \n <a class=\"reboot\" href=\"/reboot?Mode=3\">Riniciar modo sin conexion</a> \n <a class=\"reboot\" href=\"/calibrar?gas=4\">Calibrar Sensores</a> \n </ul> \n </nav>";
        html +="<div class=\"titulo\"> \n <h1>Configurar</h1> \n <p>Informacion del equipo.</p> \n </div>";
        html +="<div class=\"formulario\">\n";
        html +="<div class=\"links\">\n";
        html +="<div class=\"caja\"> \n <p><strong>TIEMPO</strong></p> \n <p>"+getTiempo()+"</p></div>";
        html +="<div class=\"caja\"><p><strong>ULTIMOS DATOS</strong></p> </br> \n";
        //BME
        html +="<p><strong>Sensor BME</strong></p>";
        html +="<p><strong>Temperatura</strong>=> "+String(Temperatura_copy)+" °C</p>";
        html +="<p><strong>Humedad</strong>=> "+String(Humedad_copy)+" %</p>";
        html +="<p><strong>Presion</strong>=> "+String(Presion_copy)+" hPa</p>";
        html +="<p><strong>Altitud</strong>=> "+String(Altitud_copy)+" m</p> </br> \n";
        //CO
        html +="<p><strong>Sensor MQ-9</strong></p>";
        html +="<p><strong>CO</strong>=> "+String(voltage_CO,4)+" V</p>";
        html +="<p><strong>CO</strong>=> "+String(resistence_CO,0)+" ohm</p>";
        html +="<p><strong>CO</strong>=> "+String(iPPM_CO)+" PPM</p>";
        html +="<p><strong>CO(PPM-Ro)</strong>=> "+String(atmCO)+" PPM</p>";
        html +="<p><Strong>CO(Ro)</strong>=> "+String(RoCO)+" ohm</p> </br> \n";
        //CH4
        html +="<p><strong>Sensor MQ-4</strong></p>";
        html +="<p><strong>CH4</strong>=> "+String(voltage_CH4,4)+" V</p>";
        html +="<p><strong>CH4</strong>=> "+String(resistence_CH4,0)+" ohm</p>";
        html +="<p><strong>CH4</strong>=> "+String(iPPM_CH4)+" PPM</p>";
        html +="<p><strong>CH4(PPM-Ro)</strong>=> "+String(atmCH4)+" PPM</p>";
        html +="<p><Strong>CH4(Ro)</strong>=> "+String(RoCH4)+" ohm</p> </br>\n";
        //CO2
        html +="<p><strong>Sensor MQ-135</strong></p>";
        html +="<p><strong>CO2</strong>=> "+String(voltage_CO2, 4)+" V</p>";
        html +="<p><strong>CO2</strong>=> "+String(resistence_CO2, 0)+" ohm</p>";
        html +="<p><strong>CO2</strong>=> "+String(iPPM_CO2)+" PPM</p>";
        html +="<p><strong>CO2(PPM-Ro)</strong>=> "+String(atmCO2)+" PPM</p>";
        html +="<p><Strong>CO2(Ro)</strong>=> "+String(RoCO2)+" ohm</p> \n </div>";
        
        html +="<div class=\"caja\"> \n <p><strong>INTERVALO DE LECTURA</strong></p> \n <p>"+String(TIME_TO_SLEEP)+" minutos</p> \n </div> \n </div>";
        html +="<div class=\"links\"> \n <div class=\"caja\">\n";
        html +="<p><strong>GSM-SIM</strong></p>\n<p><strong>Apn:</strong> "+String(sim.apn)+"</p>\n<p><strong>Usuario:</strong> "+String(sim.user)+"</p> \n <p><strong>Contrase&ntilde;a:</strong> "+String(sim.pass)+"</p></div>";
        html +="<div class=\"caja\"> \n <p><strong>WIFI-STA</strong></p> \n <p><strong>SSID:</strong> "+String(STA.ssid)+"</p> \n <p><strong>Contrase&ntilde;a:</strong> "+String(STA.pass)+"</p> \n </div>";
        html +="<div class=\"caja\"> \n <p><strong>WIFI-AP</strong></p> \n <p><strong>SSID:</strong> "+String(AP.ssid)+"</p> \n <p><strong>Contrase&ntilde;a:</strong> "+String(AP.pass)+"</p>\n</div>";
        html +="<div class=\"caja\"> \n <p><strong>TIPO DE TRABAJO GUARDADO</strong></p> \n <p>";
        switch(ModeTemp){
          case 1:{html += "Wifi";}break;
          case 2:{html += "GSM";}break;
          case 3:{html += "Sin Conexion";}break;
        }        
        "</p>\n</div>\n</div>\n</div>";
      }break;
      case 1:{
        html+="<div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar apn del GSM</h1><p>Ingresa el apn, usuario y contrase&ntilde;a. Si el operador no requiere usuario ni contraseña dejar en blanco</p>\n";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"text\" name=\"APN\" placeholder=\"APN\" autofocus required>\n";
        html+="<input type=\"text\" name=\"USUARIO\" placeholder=\"USUARIO\">\n";
        html+="<input type=\"text\" name=\"PASSWORD\" placeholder=\"PASSWORD\">\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"1\">\n";
        html+="</form></div>\n";
      }break;
      case 2:{
        html+="<div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar Credenciales Wifi-STA</h1><p>Configure las credenciales del punto de acceso a conectar. si el punto de acceso no contiene contrase&ntilde;a deje en blanco.</p>\n";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"text\" name=\"SSID\" placeholder=\"SSID\" autofocus required>\n";
        html+="<input type=\"text\" name=\"PASSWORD\" placeholder=\"PASS\">\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"2\">\n";
        html+="</form></div>\n";
      }break;
      case 3:{
        html+="<div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar credenciales Wifi-AP</h1><p>Configure las credenciales del dispositivo como punto de acceso para que pueda conectarse y realizar las configuraciones correspondiente. Si desea una red libre deje la contrase&ntilde;a en blanco.</p>";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"text\" name=\"SSID\" placeholder=\"SSID\" autofocus required>\n";
        html+="<input type=\"text\" name=\"PASSWORD\" placeholder=\"PASS\">\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"3\">\n";
        html+="</form></div>\n";
      }break;
      case 4:{
        html+="<div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar el Reloj del dispositivo</h1><p>Configure el reloj para usarlo sin ninguna conexion a internet.</p>";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"date\" name=\"date\" autofocus required>\n";
        html+="<input type=\"time\" name=\"time\" value=\"00:00\" required>\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"4\">\n";
        html+="</form></div>\n";
        }break;
      case 5:{
        html+="<div class=\"formulario\">\n";
        html+="<a href=\"/\">Regresar</a><h1>Configurar Tiempo de Descanso</h1><p>Configurar el tiempo de descanso del dispositivo para tomar datos, por defecto 1 Minuto.</p>";
        html+="<form action=\"save\" method=\"post\">\n";
        html+="<input type=\"number\" name=\"time\" placeholder=\"Minutos\" autofocus required>\n";
        html+="<input type=\"submit\" value=\"GUARDAR\">\n";
        html+="<input type=\"hidden\" name=\"type\" value=\"5\">\n";
        html+="</form></div>\n";
        }break;
      case 6:{
        html+="<div class=\"formulario\"><a href=\"/\">Regresar</a><h1>Configurado..!</h1></div>\n";
      }break;
      case 7:{
        html+="<div class=\"formulario\"><a href=\"/\">Regresar</a><h1>ESP32 Error</h1><p>"+error+"</p></div>\n";
      }break;
      case 8:{
        html+="<div class=\"formulario\"><a href=\"/\">Regresar</a><h1>El equipo ha sido reiniciado.</h1></div>\n";
      }break;
      case 9:{
        html+="<div class=\"formulario\"><a href=\"/\">Regresar</a><h1>Borrado..!</h1></div>\n";
      }break;
      case 10:{
        html+="<div class=\"formulario\"><a href=\"/\">Regresar</a><h1>No existe el archivo.</h1></div>\n";
      }break;
      case 11:{
        html+="<div class=\"formulario\">\n";
        html+="<form action=\"calibrar\" method=\"POST\" accept-charset=\"utf-8\">\n";
        html+="<h2>Calibrar Sensores</h2>\n";
        html+="<p>Coloque el sensor en el ambiente conocido, seleccione el sensor a calibrar e ingrese ese valor en ppm y presione calibrar.</p>\n";
        html+="<select name=\"gas\" required>\n";
        html+="<option value=\"0\">Dioxido de Carbono (CO2)</option>\n";
        html+="<option value=\"1\">Metano (CH4)</option>\n";
        html+="<option value=\"2\">Monoxido de carbono (CO)</option>\n";
        html+="</select>\n";
        html+="<input type=\"number\" name=\"ppm\" placeholder=\"PPM del gas conocido\" required>\n";
        html+="<input type=\"submit\" value=\"Calibrar\">\n";
        html+="</form>\n";
        html+="</div>\n";
      }break;
      case 12:{
        html+="<div class=\"formulario\"><h1>Se esta calibrando el sensor espere...!</h1></div>\n";
      }
        
    }
    html+="</body></html>";
    return html;
  }
