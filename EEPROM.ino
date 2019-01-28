void write_StringEE(int Addr, String input)
{
    char cbuff[size_buf];//Finds length of string to make a buffer
    input.toCharArray(cbuff,size_buf);//Converts String into character array
    for(int i=Addr;i<=Addr+size_buf;i++)
    {
      EEPROM.write(i,cbuff[i-Addr]);
      delay(1);
    }
    EEPROM.commit();
    delay(10);
}

String read_StringEE(int Addr)
{
    char cbuff[size_buf];//Finds length of string to make a buffer
    for(int i=Addr;i<=Addr+size_buf;i++)
    {
      cbuff[i-Addr]=EEPROM.read(i);
      delay(1);
    }
    return cbuff;
}

