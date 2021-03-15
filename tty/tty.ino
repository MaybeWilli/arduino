int x=0;
String str;
char buff[8];
void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(1000);
  while (Serial.available()) {
    Serial.read();
  }
}

void loop()
{
  int i=0;
  char c;
  while (!Serial.available()) {
    delay(1);
    return;
  }
  int synced = 0;
  unsigned long ltime;
  while (i!=3){
    if(Serial.available()) {
      c = Serial.read();
      if (c=='A') { synced = 1; ltime = millis();}
      else if (synced ==1) {
        buff[i]=c;
        i+=1;
      } else {
        ;//;
      }
    }
    if (millis() - ltime > 10) { return;}
  }
  if (millis() - ltime > 10) { return;}
  //Serial.print(i);
  if (i==3) {
    for (int j=0; j<3; j++) {
      x=x*10+(buff[j]-0x30);
    }
    Serial.println(x+1);
    Serial.flush();
    x=0;
  }
}
