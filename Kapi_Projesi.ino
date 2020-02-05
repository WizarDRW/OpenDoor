#include <WiFiNINA.h>
#include <SPI.h>
#include <Servo.h>

//IPAddress ip(10, 11, 14, 30);
//IPAddress subnet(255, 255, 252, 0);

char ssid[] = "sg";            
char pass[] = "klm.mbly.";    
int keyIndex = 0;                 // ag keyi index numarasi (Sadece web de kullanilacak)

int status = WL_IDLE_STATUS;      //Baglanti Durumu
WiFiServer server(80);            //Sunucu Soketi(Portu)

//special characters
char quote = '"';
char slash = '/';

Servo myservo;

void setup() {
  Serial.begin(9600);      // Seri haberlesme baslangici
  pinMode(6, OUTPUT);      // Led in pini
  pinMode(7, OUTPUT);
  myservo.attach(9);
  myservo.write(45);
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMacAddress(mac);

  // guvenlik kontrolu:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Wifi kalkanı mevcut değil.");
    while (true);       // Devam etme
  }

  // Aga baglanma denemesi:
  while ( status != WL_CONNECTED) {
    Serial.print("Baglanilan Network: ");
    Serial.println(ssid);                   // baglanilan ag yazdirildi;  7

    // WPA/WPA2 baglantisi:
    status = WiFi.begin(ssid, pass);
    // 10 sn bekle:
    delay(10000);
  }
  server.begin();                           // web sunucuya baglanti 80 portu
  printWiFiStatus();                        // baglanti saglandi
}


void loop() {
  // Wifiye baglaniyor:
  while ( status != WL_CONNECTED) {
    Serial.print("Baglanti Networku: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    delay(10000);
  }
  WiFiClient client = server.available();   // gelen baglantilari dinliyor

if (client) {                             // eger bir client var ise,
    Serial.println("new client");           
    String currentLine = "";                // istemciden gelen verileri tutmak için bir String
    while (client.connected()) {            // Istemci bagliyken
      if (client.available()) {         
        char c = client.read();          
        Serial.write(c);                  
        if (c == '\n') {                  

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.print("<head>");
            client.print("<link rel=");
            client.print(quote);
            client.print("stylesheet");
            client.print(quote);
            client.print("href=");
            client.print(quote);
            client.print("https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css");
            client.print(quote);
            client.print(slash);
            client.print (">");
            client.print("</head>");
            client.print("<body style='heigth:100%'>");
            client.println("<center><br><br><div class='label'><h1>KILIM ARGE OTOMASYON KONTROL<h1/></div></center>");
            client.println("<center><div class='container'><left><button class='btn btn-success' type='submit' value='ON' style='width: 50%; heigth:50%' onmousedown=location.href='/H\'>Kapiyi Ac</button>");
            client.println("</div><br>");
            client.println("</div>");
            client.println("</div>");
            client.println("</div>"); 
            client.println("</body>");
            client.println("</html>");

            client.println();
            break;
          }
          else {      
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;      
        }

        if (currentLine.endsWith("GET /H")) {
          digitalWrite(7, HIGH);
          Serial.println("Acti");
          delay(500);
          digitalWrite(7, LOW);
          Serial.println("Kapatti");
        }

        
      }
    }
    // baglantiyi kapat:
    client.stop();
    Serial.println("Baglanti kapatildi");
}
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Sinyalin guclulugu:
  long rssi = WiFi.RSSI();
  Serial.print("Sinyalin gucu (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // Tarayicidaki arayuz:
  Serial.print("http://");
  Serial.println(ip);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
