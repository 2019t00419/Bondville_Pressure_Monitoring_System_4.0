// 
//_____________________________________________________________________________

String newSSID;
const char* PARAM_INPUT_1 = "newssid";
const char* PARAM_INPUT_2 = "newpwd";
const char* PARAM_INPUT_3 = "cautionlimit";
const char* PARAM_INPUT_4 = "sendDelay";
const char* PARAM_INPUT_5 = "mailDelay";
const char* PARAM_INPUT_6 = "calibSensor";
const char* PARAM_INPUT_7 = "calibPressure";
const char* PARAM_INPUT_8 = "cautionemail";


// Set these to your desired credentials.
const char *APssid = "BPMS_Setup";
const char *APpassword = "Bondville";
bool APCreated=false;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Pressure Monitor Configuration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f0f0f0;
    }

    #container {
      background-color:#ddd ;
      padding: 20px;
      margin: 20px auto;
      border: 1px solid #bbb;
      max-width: 600px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }

    table {
      width: 100%;
      border-collapse: collapse;
    }

    th, td {
      padding: 10px;
      text-align: left;
    }

    th {
      background-color: #222;
      font-weight: bold;
      text-align:center;
    }

    button {
      background-color: #333;
      border: none;
      padding: 8px 12px;
      cursor: pointer;      
      border-radius: 4px;
      color: #f0f0f0;  
      width: 250px;
    }

    a {
      text-decoration: none;
      color: inherit;
    } 
    Caption{
      background-color: #333;
      color: #f0f0f0;    
    }
    img{
      padding: 10px 0px 0px 0px;   
    }


    input[type="text"], input[type="password"],input[type="number"] {
      padding: 8px;
      width: 100%;
      border: 1px solid #ddd;
      border-radius: 4px;
    }

    input[type="submit"] {
      background-color: #4caf50;
      color: #fff;
      border: none;
      padding: 10px 15px;
      border-radius: 4px;
      cursor: pointer;
    }

    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
  </head><body>
  <div id="container">
  <table style="margin: 0 auto;border-collapse: collapse;border: 1px solid #ddd;">
    <caption>
    <img width="50" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADgAAABACAMAAAB4KUSAAAABSlBMVEUAAACgoKCpqamhoaGhoaGpqamfn5+pqamkpKSZmZmkpKShoaGZmZn19fX39/f09PTy8vL8/Pzs7Ozl5eXg4ODk5OTa2trR0dHQ0NDS0tLS0tLJycm5ubm2trawsLC0tLS1tbWzs7O3t7eysrK3t7e2tra2tra2tra3t7esrKyurq68vLy+vr68vLzExMTNzc3Pz8/IyMjX19fV1dXT09PT09PNzc3T09PW1tbd3d3o6Oj29vb29vb7+/vi4uLT09PS0tLV1dWoqKjv7+/8/Pz5+fnn5+fi4uKbm5uenp6ioqKlpaWoqKirq6utra2wsLCzs7O1tbW2tra3t7e8vLzCwsLFxcXIyMjLy8vNzc3Ozs7Q0NDR0dHS0tLT09PU1NTW1tbZ2dna2trc3Nze3t7h4eHk5OTm5ubq6urs7Ozv7+/z8/P4+PiXl5dkTy/sAAAASHRSTlMAPktZaXmIlKS2zeXw/unh2ePw7+/h3+Dq8fnu6/Xr4tPEtK+ehm1bQjAjFA0HAxsmMT9MYHqBjZuKmZy2wbizx8/2Zn1GMynhmXFMAAADx0lEQVR42pTTVbryMBCA4W4Ad7aAu7t0B7+cehM8+7+uzMkkOHxwBc9bm6nytOpmPG1Wla9T1wlGs736l6y8ijMWIY6zaJc+NXCVEcbYhdiOY89alY+vchhlQRfXDir01Y9YaRWPQGfHCrKtXKf2yVWeIxcogJA5b5ffXWb8gp1s0+IVZm9Go8bO2NE2pfIvR1NV4+iSw37BkHs5GjV+gpKjZqXSmhUlaBYfjIbf+zZzDDolV+EvpfZCl6RR6DcUudp6zc8I8Jjc8r86OV1OHk15M2EjhIewFIdVpdHPa1I6jmY7urAnEKo0ZwVdCkajrmOMyXAfBpBXbs+LMs33Goq/X8xfr5cQblWTyynBfkUilxu4Q4jVe/kfKeUCCbgLS/vwZscqzX5Bw5QzNLyH6mCgosJbfQ5pWLq1ylIne/s+lToFDk9hAmZ/YZZQYtv27X7WchweIQGJHyXU/7qWaQb7Wf4Mui5xfzOD/TQLffEqVgU8QBLEHEMPMgx/3pzWn0MHs3Ux7xaHeQ730Buodd9DG7O0H+we7iCEOcvmHx/iOT+BmA+1t7CP0ORZ5ktIoaGAmKEJeQ8JJM6oQ4bx8+cPuFeQ9m8GrP/88SqpizzVgSAA44VD4Aa4a8KPK7CKWwd39tx/+zrdQ6ojz75ZjfwjVT0Qxw0O6ameefDmITzTLucDsW3H9cjueDqGCfDCvuDF+4EKg5RZpkWh73vB7pCAPIR4x2Ng6gaHxPdJsD8cDghPPHjy8I7HwNK0CBJGd/sINo88ePCiO7ZsVdU0AyGlHgn+CpflxodCPQ7d/gx4mwjeeV8Icq6WumN3vIAIHnhwY10jCMqqKMXu2B7OFEjDK49CbFGuI+xNV4AhvLDOMQhKrrrlsMWeEps19zw4s3a9wgrE5EKfwvYgfEpsNe0FOwHubU1t1PKyeE9Ylrq9yVJk8mzQ9jw/2IXB6bR3tM9HVdVGNaeA2HwR/3bUopuhu2EU9i5lDGpavTKnNLv1uEsIg57rekEA1QZzDGp6vbzIZKtJjxCEbmcULjykP1A3pNI6xeT8oE0ESDcbzmNVqEWQSkMqLuO72YxChrD/3ayyLEoCNLaSuJsFezmEsVEr9KwgNM3t8LubJX05DI8t0nm1gdC0Omw3q2nf8wXXGs0hYwI1AVp2pzKfDduej5CeIzl75gVJgLbT6bguQp/PJLt1SRKg4wgQZ5KZsqjUsyDOJCP8j0rC1mijwN+T89JWhDiTv7YqShHEc/IPKbAodTnsTdbwf80rHcfJnMnfX7WPH2/pfgGkqmN+GyfUkgAAAABJRU5ErkJggg==">
    
    <h1>Pressure Monitor Configuration</h1></caption>
    <tr>
      <th colspan="2">
        <a href="/"><button style="background-color: #888">WIFI settings</button></a>
      </th>
      <th colspan="2">
        <a href="/calib"><button>Calibration</button></a>
      </th>
    </tr>
   </table>
    <table style="margin: 0 auto;border-collapse: collapse;border: 1px solid #ddd;">
    <form action="/get">
    <tr>
      <td>
        New SSID: 
      </td>
      
      <td>
         <input type="text" name="newssid">
      </td> 
    </tr>
    <tr>
      <td>
        New Password: 
      </td>
      
      <td>
         <input type="password" name="newpwd" required>
      </td>
    </tr>
    <tr> 
      <td Style="color: #aaa;">
      <td>  
        <input type="submit" value="Submit" required>
      </td>
    </table>
  </form>
  </div>
</body></html>)rawliteral";

const char calib_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Pressure Monitor Configuration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f0f0f0;
    }

    #container {
      background-color:#ddd ;
      padding: 20px;
      margin: 20px auto;
      border: 1px solid #bbb;
      max-width: 600px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }

    table {
      width: 100%;
      border-collapse: collapse;
    }

    th, td {
      padding: 10px;
      text-align: left;
    }


    th {
      background-color: #222;
      font-weight: bold;
      text-align:center;
    }

    button {
      background-color: #333;
      border: none;
      padding: 8px 12px;
      cursor: pointer;      
      border-radius: 4px;
      color: #f0f0f0;  
      width: 250px;
    }

    a {
      text-decoration: none;
      color: inherit;
    } 
    Caption{
      background-color: #333;
      color: #f0f0f0;    
    }
    img{
      padding: 10px 0px 0px 0px;   
    }


    input[type="text"], input[type="password"],input[type="number"] {
      padding: 8px;
      width: 100%;
      border: 1px solid #ddd;
      border-radius: 4px;
    }

    input[type="submit"] {
      background-color: #4caf50;
      color: #fff;
      border: none;
      padding: 10px 15px;
      border-radius: 4px;
      cursor: pointer;
    }

    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
  </head><body>
  <div id="container">
  <table style="margin: 0 auto;border-collapse: collapse;border: 1px solid #ddd;">
    <caption><img width="50" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADgAAABACAMAAAB4KUSAAAABSlBMVEUAAACgoKCpqamhoaGhoaGpqamfn5+pqamkpKSZmZmkpKShoaGZmZn19fX39/f09PTy8vL8/Pzs7Ozl5eXg4ODk5OTa2trR0dHQ0NDS0tLS0tLJycm5ubm2trawsLC0tLS1tbWzs7O3t7eysrK3t7e2tra2tra2tra3t7esrKyurq68vLy+vr68vLzExMTNzc3Pz8/IyMjX19fV1dXT09PT09PNzc3T09PW1tbd3d3o6Oj29vb29vb7+/vi4uLT09PS0tLV1dWoqKjv7+/8/Pz5+fnn5+fi4uKbm5uenp6ioqKlpaWoqKirq6utra2wsLCzs7O1tbW2tra3t7e8vLzCwsLFxcXIyMjLy8vNzc3Ozs7Q0NDR0dHS0tLT09PU1NTW1tbZ2dna2trc3Nze3t7h4eHk5OTm5ubq6urs7Ozv7+/z8/P4+PiXl5dkTy/sAAAASHRSTlMAPktZaXmIlKS2zeXw/unh2ePw7+/h3+Dq8fnu6/Xr4tPEtK+ehm1bQjAjFA0HAxsmMT9MYHqBjZuKmZy2wbizx8/2Zn1GMynhmXFMAAADx0lEQVR42pTTVbryMBCA4W4Ad7aAu7t0B7+cehM8+7+uzMkkOHxwBc9bm6nytOpmPG1Wla9T1wlGs736l6y8ijMWIY6zaJc+NXCVEcbYhdiOY89alY+vchhlQRfXDir01Y9YaRWPQGfHCrKtXKf2yVWeIxcogJA5b5ffXWb8gp1s0+IVZm9Go8bO2NE2pfIvR1NV4+iSw37BkHs5GjV+gpKjZqXSmhUlaBYfjIbf+zZzDDolV+EvpfZCl6RR6DcUudp6zc8I8Jjc8r86OV1OHk15M2EjhIewFIdVpdHPa1I6jmY7urAnEKo0ZwVdCkajrmOMyXAfBpBXbs+LMs33Goq/X8xfr5cQblWTyynBfkUilxu4Q4jVe/kfKeUCCbgLS/vwZscqzX5Bw5QzNLyH6mCgosJbfQ5pWLq1ylIne/s+lToFDk9hAmZ/YZZQYtv27X7WchweIQGJHyXU/7qWaQb7Wf4Mui5xfzOD/TQLffEqVgU8QBLEHEMPMgx/3pzWn0MHs3Ux7xaHeQ730Buodd9DG7O0H+we7iCEOcvmHx/iOT+BmA+1t7CP0ORZ5ktIoaGAmKEJeQ8JJM6oQ4bx8+cPuFeQ9m8GrP/88SqpizzVgSAA44VD4Aa4a8KPK7CKWwd39tx/+zrdQ6ojz75ZjfwjVT0Qxw0O6ameefDmITzTLucDsW3H9cjueDqGCfDCvuDF+4EKg5RZpkWh73vB7pCAPIR4x2Ng6gaHxPdJsD8cDghPPHjy8I7HwNK0CBJGd/sINo88ePCiO7ZsVdU0AyGlHgn+CpflxodCPQ7d/gx4mwjeeV8Icq6WumN3vIAIHnhwY10jCMqqKMXu2B7OFEjDK49CbFGuI+xNV4AhvLDOMQhKrrrlsMWeEps19zw4s3a9wgrE5EKfwvYgfEpsNe0FOwHubU1t1PKyeE9Ylrq9yVJk8mzQ9jw/2IXB6bR3tM9HVdVGNaeA2HwR/3bUopuhu2EU9i5lDGpavTKnNLv1uEsIg57rekEA1QZzDGp6vbzIZKtJjxCEbmcULjykP1A3pNI6xeT8oE0ESDcbzmNVqEWQSkMqLuO72YxChrD/3ayyLEoCNLaSuJsFezmEsVEr9KwgNM3t8LubJX05DI8t0nm1gdC0Omw3q2nf8wXXGs0hYwI1AVp2pzKfDduej5CeIzl75gVJgLbT6bguQp/PJLt1SRKg4wgQZ5KZsqjUsyDOJCP8j0rC1mijwN+T89JWhDiTv7YqShHEc/IPKbAodTnsTdbwf80rHcfJnMnfX7WPH2/pfgGkqmN+GyfUkgAAAABJRU5ErkJggg==">
    <h1>Pressure Monitor Configuration</h1></caption>
    <tr>
      <th>
        <a href="/"><button>WIFI settings</button></a>
      </th>
      <th>
        <a href="/calib"><button style="background-color: #888">Calibration</button></a>
      </th>
    </tr>
   </table>
      <table style="margin: 0 auto;border-collapse: collapse;border: 1px solid #ddd;">
    <form action="/get">
    <tr>
      <td>
        Sensor value:  
      </td>
      
      <td>
         <input type="number" name="calibSensor">
      </td>
    </tr>
    <tr>
      <td>
        Pressure (Bar): 
      </td>
      
      <td>
         <input type="text" pattern="[0-9]+(\.[0-9]+)?" name="calibPressure">
      </td>
    </tr>
    <tr> 
      <td></td>
      <td>  
        <input type="submit" value="Submit">
      </td>
    </table>
    </form>
  </div>
</body></html>)rawliteral";



const char done_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Pressure Monitor Configuration</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background-color: #f0f0f0;
    }

    #container {
      background-color:#ddd ;
      padding: 20px;
      margin: 20px auto;
      border: 1px solid #bbb;
      max-width: 600px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }

    table {
      width: 100%;
      border-collapse: collapse;
    }

    th, td {
      padding: 10px;
      text-align: left;
    }

    

    th {
      background-color: #222;
      font-weight: bold;
      text-align:center;
    }

    button {
      background-color: #333;
      border: none;
      padding: 8px 12px;
      cursor: pointer;      
      border-radius: 4px;
      color: #f0f0f0;  
      width: 250px;
    }

    a {
      text-decoration: none;
      color: inherit;
    } 
    Caption{
      background-color: #333;
      color: #f0f0f0;    
    }
    img{
      padding: 10px 0px 0px 0px;   
    }


    input[type="text"], input[type="password"],input[type="number"] {
      padding: 8px;
      width: 100%;
      border: 1px solid #ddd;
      border-radius: 4px;
    }

    input[type="submit"] {
      background-color: #4caf50;
      color: #fff;
      border: none;
      padding: 10px 15px;
      border-radius: 4px;
      cursor: pointer;
    }

    input[type="submit"]:hover {
      background-color: #45a049;
    }
  </style>
  </head><body>
  <div id="container">
  <table style="margin: 0 auto;border-collapse: collapse;border: 1px solid #ddd;">
    <caption><img width="50" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADgAAABACAMAAAB4KUSAAAABSlBMVEUAAACgoKCpqamhoaGhoaGpqamfn5+pqamkpKSZmZmkpKShoaGZmZn19fX39/f09PTy8vL8/Pzs7Ozl5eXg4ODk5OTa2trR0dHQ0NDS0tLS0tLJycm5ubm2trawsLC0tLS1tbWzs7O3t7eysrK3t7e2tra2tra2tra3t7esrKyurq68vLy+vr68vLzExMTNzc3Pz8/IyMjX19fV1dXT09PT09PNzc3T09PW1tbd3d3o6Oj29vb29vb7+/vi4uLT09PS0tLV1dWoqKjv7+/8/Pz5+fnn5+fi4uKbm5uenp6ioqKlpaWoqKirq6utra2wsLCzs7O1tbW2tra3t7e8vLzCwsLFxcXIyMjLy8vNzc3Ozs7Q0NDR0dHS0tLT09PU1NTW1tbZ2dna2trc3Nze3t7h4eHk5OTm5ubq6urs7Ozv7+/z8/P4+PiXl5dkTy/sAAAASHRSTlMAPktZaXmIlKS2zeXw/unh2ePw7+/h3+Dq8fnu6/Xr4tPEtK+ehm1bQjAjFA0HAxsmMT9MYHqBjZuKmZy2wbizx8/2Zn1GMynhmXFMAAADx0lEQVR42pTTVbryMBCA4W4Ad7aAu7t0B7+cehM8+7+uzMkkOHxwBc9bm6nytOpmPG1Wla9T1wlGs736l6y8ijMWIY6zaJc+NXCVEcbYhdiOY89alY+vchhlQRfXDir01Y9YaRWPQGfHCrKtXKf2yVWeIxcogJA5b5ffXWb8gp1s0+IVZm9Go8bO2NE2pfIvR1NV4+iSw37BkHs5GjV+gpKjZqXSmhUlaBYfjIbf+zZzDDolV+EvpfZCl6RR6DcUudp6zc8I8Jjc8r86OV1OHk15M2EjhIewFIdVpdHPa1I6jmY7urAnEKo0ZwVdCkajrmOMyXAfBpBXbs+LMs33Goq/X8xfr5cQblWTyynBfkUilxu4Q4jVe/kfKeUCCbgLS/vwZscqzX5Bw5QzNLyH6mCgosJbfQ5pWLq1ylIne/s+lToFDk9hAmZ/YZZQYtv27X7WchweIQGJHyXU/7qWaQb7Wf4Mui5xfzOD/TQLffEqVgU8QBLEHEMPMgx/3pzWn0MHs3Ux7xaHeQ730Buodd9DG7O0H+we7iCEOcvmHx/iOT+BmA+1t7CP0ORZ5ktIoaGAmKEJeQ8JJM6oQ4bx8+cPuFeQ9m8GrP/88SqpizzVgSAA44VD4Aa4a8KPK7CKWwd39tx/+zrdQ6ojz75ZjfwjVT0Qxw0O6ameefDmITzTLucDsW3H9cjueDqGCfDCvuDF+4EKg5RZpkWh73vB7pCAPIR4x2Ng6gaHxPdJsD8cDghPPHjy8I7HwNK0CBJGd/sINo88ePCiO7ZsVdU0AyGlHgn+CpflxodCPQ7d/gx4mwjeeV8Icq6WumN3vIAIHnhwY10jCMqqKMXu2B7OFEjDK49CbFGuI+xNV4AhvLDOMQhKrrrlsMWeEps19zw4s3a9wgrE5EKfwvYgfEpsNe0FOwHubU1t1PKyeE9Ylrq9yVJk8mzQ9jw/2IXB6bR3tM9HVdVGNaeA2HwR/3bUopuhu2EU9i5lDGpavTKnNLv1uEsIg57rekEA1QZzDGp6vbzIZKtJjxCEbmcULjykP1A3pNI6xeT8oE0ESDcbzmNVqEWQSkMqLuO72YxChrD/3ayyLEoCNLaSuJsFezmEsVEr9KwgNM3t8LubJX05DI8t0nm1gdC0Omw3q2nf8wXXGs0hYwI1AVp2pzKfDduej5CeIzl75gVJgLbT6bguQp/PJLt1SRKg4wgQZ5KZsqjUsyDOJCP8j0rC1mijwN+T89JWhDiTv7YqShHEc/IPKbAodTnsTdbwf80rHcfJnMnfX7WPH2/pfgGkqmN+GyfUkgAAAABJRU5ErkJggg==">
    <h1>Pressure Monitor Configuration</h1></caption>
    <tr>
      <th>
        <a href="/"><button>WIFI settings</button></a>
      </th>
      <th>
        <a href="/calib"><button>Calibration</button></a>
      </th>
    </tr>
   </table>
      <table style="margin: 0 auto;border-collapse: collapse;border: 1px solid #ddd;">
    <tr>
      <th style="background-color: #ddd;text-align: center;">
        <h2>Configuration Successful!</h2>
      </th>
    </tr>
    </table>
  </div>
</body></html>)rawliteral";



//"

void createAP(){
  // Configure ESP32 as an Access Point
  if(updateMode){
    if(!APCreated){
      WiFi.mode(WIFI_AP);
      WiFi.softAP(APssid, APpassword);

      Serial.println();
      Serial.print("AP SSID: ");
      Serial.println(APssid);
      Serial.print("AP Password: ");
      Serial.println(APpassword);
      Serial.print("AP IP Address: ");
      Serial.println(WiFi.softAPIP());

      // Send web page with input fields to client
      server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
      });
      server.on("/calib", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", calib_html);
      });

      server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    // GET newssid value on <ESP_IP>/get?newssid=<inputMessage>
      if (request->hasParam(PARAM_INPUT_1)) {
        newSSID = request->getParam(PARAM_INPUT_1)->value();
      }
      Serial.println(newSSID);
    
      request->send_P(200, "text/html", done_html);
   
      });
      
      server.onNotFound(notFound);
      server.begin();
      APCreated=true;
    }
  }else{
      APCreated=false;  
      if(WiFi.status() != WL_CONNECTED){ 
        setupWifi();
      }
  }

}

