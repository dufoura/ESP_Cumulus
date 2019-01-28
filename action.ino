void handle_action()
{
  String payload=server.arg("rel1");
  if (payload.length() > 0 ) 
  {
    Serial.println("action rel1 demandee");
    if(payload.toInt()==0)
    {
      Serial.println("relais1 off");
      auth_rel1=false;
      EEPROM.write(70, 0);
      EEPROM.commit();
    }
    else if(payload.toInt()==1)
    {
      Serial.println("relais1 on");
      auth_rel1=true;
      EEPROM.write(70, 1);
      EEPROM.commit();      
    }
    flag_data=true;
  }

  payload=server.arg("rel2");
  if (payload.length() > 0 ) 
  {
    Serial.println("action rel2 demandee");
    if(payload.toInt()==0)
    {
      Serial.println("relais2 off");
      auth_rel2=false;
      EEPROM.write(71, 0);
      EEPROM.commit();
    }
    else if(payload.toInt()==1)
    {
      Serial.println("relais2 on");
      auth_rel2=true;
      EEPROM.write(71, 1);
      EEPROM.commit();
    }
    flag_data=true;
  }

  if(flag_data) send_data_html(true);
}

