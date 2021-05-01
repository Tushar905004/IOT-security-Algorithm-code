#include <DHT.h>  // Including library for dht

#include <ESP8266WiFi.h>

String apiKey = "X07J193T2GE02AKC";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Tushar";     // replace with your wifi ssid and wpa2 key
const char *pass =  "905004tushar";
const char* server = "api.thingspeak.com";

#define DHTPIN 0          //pin where the dht11 is connected

DHT dht(DHTPIN, DHT11);

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop()
{

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  float id=1;
  double mes=21;
  float tim=floor(t);
  tim=t-tim;
  tim=tim*100;
  
  float m=id*t;
  float a=pow(id,2)+pow(t,2);
  //Serial.println(a);
  float alpha= (id+tim);
  float ta=cos(alpha);
  float s=a-(2*m*ta);
  float si=sqrt(s);
  //Serial.println(si);
  float k=(si+id+tim)/2;
  float en= (pow(id,tim)*mes)/k;
  Serial.println(en);
  

  
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(en);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(en);
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();

  Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
}
