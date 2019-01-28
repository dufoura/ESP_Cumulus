void config_local_network()
{ 
String payload;  
//if(init_esp!=1)//processus appareillage réseau wifi
//{
  payload=server.arg("wifiname");
  if (payload.length() > 0 ) 
  {
    ssid = payload;
    Serial.println("SSID mis a jour");
    
    payload=server.arg("wifipass");
    if (payload.length() > 0 ) 
    {
      password = payload;
      
      Serial.println("password mis a jour");
        
        //Sauvegarde code wifi
        write_StringEE(3,ssid);
        write_StringEE(35,password);
        String success="Appareillage effectué. Vous pouvez vous reconnecter sur votre wifi local"+ssid;
        server.send ( 200, "text/html", success);
        //Connexion au reseau : 
        WiFi.disconnect();
        Serial.println("switch station");
        delay(100);
        WiFi.mode(WIFI_STA);
        delay(100);
        WiFi.begin(ssid.c_str(), password.c_str());
        while (WiFi.status() != WL_CONNECTED) 
        {
          delay(500);
          Serial.print(".");
        } 
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
        return;
      
      
    }
  }
//}
//else
//{
 boolean send_trame=false;
      payload=server.arg("relais1_ON");
      if (payload.length() > 0 ) 
      {
        Serial.println("RelaisON conf");
        hour_on = payload.substring(0,payload.indexOf(':')).toInt();
        min_on = payload.substring(payload.indexOf(':')+1).toInt();
        EEPROM.write(175, hour_on);
        EEPROM.commit();
        EEPROM.write(176, min_on);
        EEPROM.commit();

        send_trame=true;
      } 
      payload=server.arg("relais1_OFF");
      if (payload.length() > 0 ) 
      {
        Serial.println("RelaisOFF conf");
        hour_off = payload.substring(0,payload.indexOf(':')).toInt();
        min_off = payload.substring(payload.indexOf(':')+1).toInt();
        EEPROM.write(177, hour_off);
        EEPROM.commit();
        EEPROM.write(178, min_off);
        EEPROM.commit();
        send_trame=true;
      } 
      payload=server.arg("time");
      if (payload.length() > 0 ) 
      {
        Serial.println("Time conf");
        int jour=payload.substring(0,payload.indexOf('/')).toInt();
        int mois=payload.substring(payload.indexOf('/')+1,payload.lastIndexOf('/')).toInt();
        int annee=payload.substring(payload.lastIndexOf('/')+1,payload.indexOf(' ')).toInt();
        int heure=payload.substring(payload.indexOf(' ')+1,payload.indexOf(':')).toInt();
        int minut=payload.substring(payload.indexOf(':')+1,payload.lastIndexOf(':')).toInt();
        int seconde=payload.substring(payload.lastIndexOf(':')+1).toInt();
        String Test="Time : "+String(jour)+"/"+String(mois)+"/"+String(annee)+" "+String(heure)+":"+String(minut)+":"+String(seconde);
        Serial.println(Test);
        if(jour<32 && jour>0 && mois<13 && mois>0 && heure<25 && minut<60 && seconde<60)
        {
          Serial.println("Correct Time, updating");
        rtc.adjust(DateTime(annee, mois, jour, heure, minut, seconde));
        }
        else Serial.println("Error Time");
        send_trame=true;
      } 
     payload=server.arg("rel1name");
      if (payload.length() > 0 ) 
      {
        rel1Name = payload;
        write_StringEE(105,rel1Name);
        send_trame=true;
      } 
        
    payload=server.arg("wifiname");
 if (payload.length() > 0 ) 
  {
    ssid = payload;
    payload=server.arg("wifipass");
    if (payload.length() > 0 ) 
    {
      password = payload;
      send_trame=true;
        //Sauvegarde code wifi
        write_StringEE(3,ssid);
        write_StringEE(35,password);
        String success="Appareillage effectué. Vous pouvez vous reconnecter sur votre wifi local"+ssid;
        server.send ( 200, "text/html", success);
    }
  }
  if(send_trame) 
  {
    if(init_esp!=1) send_labels();//sert à mettre à jour le serveur, uniquement s'il existe une connexion
      File file = SPIFFS.open("/acceuil.html", "r");
    size_t sent = server.streamFile(file,".hmtl");
    file.close();
  }
//}
}
