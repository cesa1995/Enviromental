const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<style>
h1{
    left: 50%;
  -webkit-transform: translateX(-50%);
          transform: translateX(-50%);
  position: relative;
  width:300px;

}

form {
  left: 50%;
  -webkit-transform: translateX(-50%);
          transform: translateX(-50%);
  position: relative;
  width:300px;
}

form input, select ,textarea{
  -webkit-appearance:none;
     -moz-appearance:none;
          appearance:none;
  color: #000000;
  width:100%;
  position:relative;
  padding:13px 10px 12px;
  margin:10px auto;
  border:none;
  -webkit-box-shadow:0 0 0 2px #cccccc;
          box-shadow:0 0 0 2px #cccccc;
  border-radius:5px;
}
form input:focus , select:focus, textarea:focus{
  -webkit-box-shadow:0 0 0 2px #9204b2;
          box-shadow:0 0 0 2px #9204b2;
  outline:none;
}
form input[type="submit"] {
  width:270px;
  background:rgb(147,200,71);
  color:#fff;
  font-size:14px;
  padding:10px;
  -webkit-box-shadow:0 0 0 0;
          box-shadow:0 0 0 0;
}
form input[type="submit"]:focus {
  -webkit-box-shadow:none;
          box-shadow:none;
  margin :0 auto;
}
</style>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>WifiSettings</title>
</head>
<body>
    <div class="formulario">
        <h1>Configurar Wifi ESP32</h1>
        <form action="" method="post">
            <input type="text" name="SSID" placeholder="SSID" autofocus required>
            <input type="text" name="PASSWORD" placeholder="PASS" required>
            <input type="submit" value="GUARDAR">
        </form>
    </div>
</body>
</html>)=====";

const char DONE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<style>
h1{
    left: 50%;
  -webkit-transform: translateX(-50%);
          transform: translateX(-50%);
  position: relative;
  width:300px;

}
</style>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>WifiSettings</title>
</head>
<body>
    <div class="formulario">
        <h1>Configurado..!</h1>
    </div>
</body>
</html>)=====";
