void send_data_handle()
{
  String payload;  
  payload=server.arg("path");
  payload="/"+payload;
  Serial.print("payload : ");
  Serial.println(payload);
  if(SPIFFS.exists(payload)) 
    {
      File file = SPIFFS.open(payload, "r");
      payload="."+server.arg("type");
      Serial.print("extension : ");
      Serial.println(payload);
    size_t sent = server.streamFile(file,payload);
    file.close();
    }
    else 
    {
      File file = SPIFFS.open("/acceuil.html", "r");
    size_t sent = server.streamFile(file,".hmtl");
    file.close();
    }
}
void update_data()
{
send_data_html(true);
}
void send_data_json()
{
Serial.println("data json requested");
  String trame="{ \"capteur\" : [";
  trame+="{ \"nom\":\""+rel1Name+"\" , \"code\":\"rel1\", \"format\":\"bool\", \"editable\":\"yes\", \"value\":\""+String(auth_rel1)+"\", \"priority\":\"0\", \"unit\":\"none\" }";
  trame+="],\"Time\":\""+String(now.hour())+":"+String(now.minute())+":"+String(now.second())+"\",\"hour_ON\":\""+String(hour_on)+":"+String(min_on)+"\",\"hour_OFF\":\""+String(hour_off)+":"+String(min_off)+"\",\"device_name\":\""+espName+"\",\"ssid\":\""+ssid+"\"}";
  server.send ( 200, "text/json",trame);
}
void send_data_html(boolean html_response) 
{
boolean status;
      
// This will send the request to the server
IPAddress ip = WiFi.localIP();
ClientIP = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
String trame="data/identifiant="+id_str+"&ip="+ClientIP+"&interval="+String(sendInterval)+"&rel1="+String(auth_rel1)+"&rel2="+String(auth_rel2)+'/';

status=send_data(trame);
if(html_response)
{
  if(status) server.send ( 200, "text/html", "Done");
  else server.send ( 200, "text/html", "Error");
}
}

void send_notification(String mode,String message)
{
}
void send_labels()
{
}

boolean send_data(String trame)
{
}


