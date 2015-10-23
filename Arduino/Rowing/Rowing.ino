#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>


const int range = 2295;
//## PINS
const byte PIN_ROT = D2;
const byte PIN_GRUEN = D5;
const byte PIN_BLAU = D8;
//## ENDE PINS

//## RGB Kalibrierung
int gRot = 9;
int gGruen = 6;
int gBlau = 2;
//## ENDE RGB Kalibrierung

//## WiFi
const char* ssid = "Netzwerk 1";
const char* passwort = "";
const String newLine = "\n";
MDNSResponder mdns;
ESP8266WebServer server(80);
//## ENDE WiFi

void setup() {
  //Serial.begin(115200);
  pinMode(PIN_ROT, OUTPUT);
  pinMode(PIN_GRUEN, OUTPUT);
  pinMode(PIN_BLAU, OUTPUT);
  analogWriteRange(range);
  analogWriteFreq(2000);
  setColor(0, 0, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwort);
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print("..");
    delay(500);
  }
  //Serial.println("Verbunden");
  //Serial.print("IP: ");
  //Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    //Serial.println("MDNS responder gestartet");
  } else  {
    //Serial.println("MDNS fehlgeschalgen");
  }

  server.on("/", handleRootSeite);
  server.on("/weissabgleich", handleWeissabgleich);

  server.begin();
  //Serial.println("Server gestartet");
}

void loop() {
  server.handleClient();
  yield();
}

void handleRootSeite() {
  int serverArgs = server.args();
  if (serverArgs == 3) {
    //Manueller RGB Wert
    float rot = server.arg("rot").toInt();
    float gruen = server.arg("gruen").toInt();
    float blau = server.arg("blau").toInt();
    setColor(rot, gruen, blau);
  }
  else if (serverArgs == 1) {
    //Definierter RGB Wert
    int farbeCode = server.arg("sColor").toInt();
    byte helligkeit = server.arg("sHelligkeit").toInt();
    if (farbeCode > 255255255 || farbeCode < 0 || countDigits(farbeCode) != 9) {
      server.send(501, "text/plain", "Parameter sColor ist falsch. Muss 9 stellig und zwischen 0 und 255255255 sein!");
      return;
    }
    byte rot = farbeCode / 1000000;
    byte gruen = (farbeCode % 1000000) / 1000;
    byte blau = farbeCode % 1000;
    //Heligkeit ist Prozentual von 0 bis 100
    rot = helligkeit / 100.0 * rot;
    gruen = helligkeit / 100.0 * gruen;
    blau = helligkeit / 100.0 * blau;
    setColor((int) rot, (int) gruen, (int) blau);
  }

  //Index.html:
  String html = F("<!DOCTYPE html>");
  html += newLine;
  html += F("<html lang=\"de\">");
  html += newLine;
  html += F("<head>");
  html += newLine;
  html += F("<meta charset=\"UTF-8\">");
  html += newLine;
  html += F("<title>RGB Rowing</title>");
  html += newLine;
  html += F("<link rel=\"stylesheet\" href=\"http://yui.yahooapis.com/pure/0.6.0/pure-min.css\">");
  html += newLine;
  html += F("<link rel=\"stylesheet\" href=\"http://code.ionicframework.com/ionicons/2.0.1/css/ionicons.min.css\">");
  html += newLine;
  html += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  html += newLine;
  html += F("</head>");
  html += newLine;
  html += F("<body>");
  html += newLine;
  html += F("<h1>Rowing RGB Konfiguration</h1>");
  html += newLine;
  html += F("<div class=\"pure-g\">");
  html += newLine;
  html += F("<div class=\"pure-u-1 pure-u-md-1-1\">");
  html += newLine;
  html += F("<h2>Farbe ändern</h2>");
  html += newLine;
  html += F("<fieldset>");
  html += newLine;
  html += F("<legend>Vordefinierte Farbe zeigen</legend>");
  html += newLine;
  html += F("<form class=\"pure-form\">");
  html += newLine;
  html += F("<select name=\"sColor\">");
  html += newLine;
  html += F("<option name=\"sAus\" value=\"000000000\">LEDs ausschalten</option>");
  html += newLine;
  html += F("<option name=\"sRot\" value=\"255000000\">Rot</option>");
  html += newLine;
  html += F("<option name=\"sGruen\" value=\"000255000\">Grün</option>");
  html += newLine;
  html += F("<option name=\"sBlau\" value=\"000000255\">Blau</option>");
  html += newLine;
  html += F("<option name=\"sWeiss\" value=\"255255255\">Weiß</option>");
  html += newLine;
  html += F("<option name=\"sOrange\" value=\"19350204\">Orange</option>");
  html += newLine;
  html += F("</select>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<input type=\"number\" min=\"0\" max=\"100\" name=\"sHelligkeit\" id=\"1\" autocomplete=\"off\"");
  html += newLine;
  html += F("placeholder=\"Helligkeit in Prozent\" required>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<button type=\"submit\" class=\"pure-button ion-ios-pulse-strong\"> LEDs schalten</button>");
  html += newLine;
  html += F("</form>");
  html += newLine;
  html += F("</fieldset>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<fieldset>");
  html += newLine;
  html += F("<legend>RGB-Farbe definieren</legend>");
  html += newLine;
  html += F("<form class=\"pure-form\">");
  html += newLine;
  html += F("<input type=\"number\" name=\"rot\" autofocus autocomplete=\"off\" placeholder=\"Rot\" min=\"0\" max=\"255\" required>");
  html += newLine;
  html += F("<input type=\"number\" name=\"gruen\" autocomplete=\"off\" placeholder=\"Grün\" min=\"0\" max=\"255\" required>");
  html += newLine;
  html += F("<input type=\"number\" name=\"blau\" autocomplete=\"off\" placeholder=\"Blau\" min=\"0\" max=\"255\" required>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<button type=\"submit\" class=\"pure-button ion-ios-pulse-strong\"> LEDs schalten</button>");
  html += newLine;
  html += F("</form>");
  html += newLine;
  html += F("</fieldset>");
  html += newLine;
  html += F("</div>");
  html += newLine;
  html += F("<!--Einstellungen-->");
  html += newLine;
  html += F("<div class=\"pure-u-1 pure-u-md-1-1\">");
  html += newLine;
  html += F("<h2>Einstellungen</h2>");
  html += newLine;
  html += F("<a href=\"/weissabgleich\" class=\"pure-button\">");
  html += newLine;
  html += F("<i class=\"ion-android-options\"></i> Weißabgleich");
  html += newLine;
  html += F("</a>");
  html += newLine;
  html += F("</div>");
  html += newLine;
  html += F("</div>");
  html += newLine;
  html += F("</body>");
  html += newLine;
  html += F("</html>");
  html += newLine;




  server.send(200, "text/html", html);
}

void handleWeissabgleich() {
  if (server.args() == 3) {
    int neuRot = server.arg("wrot").toInt();
    int neuGruen = server.arg("wgruen").toInt();
    int neuBlau = server.arg("wblau").toInt();
    if (neuRot != 9 && neuGruen != 9 && neuBlau != 9) {
      server.send(200, "text/plain", "Die Eingabe war ungueltig. Mindestens 1 Wert muss 9 sein!");
      return;
    }
    gRot = neuRot;
    gGruen = neuGruen;
    gBlau = neuBlau;
  }

  //Weissabgleich.html

  String html = F("<!DOCTYPE html>");
  html += newLine;
  html += F("<html lang=\"de\">");
  html += newLine;
  html += F("<head>");
  html += newLine;
  html += F("<!-- http://ionicons.com/#cdn-->");
  html += newLine;
  html += F("<meta charset=\"UTF-8\">");
  html += newLine;
  html += F("<title>Weißabgleich</title>");
  html += newLine;
  html += F("<link rel=\"stylesheet\" href=\"http://yui.yahooapis.com/pure/0.6.0/pure-min.css\">");
  html += newLine;
  html += F("<link rel=\"stylesheet\" href=\"http://code.ionicframework.com/ionicons/2.0.1/css/ionicons.min.css\">");
  html += newLine;
  html += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  html += newLine;
  html += F("</head>");
  html += newLine;
  html += F("<body>");
  html += newLine;
  html += F("<div class=\"pure-g\">");
  html += newLine;
  html += F("<div class=\"pure-u-1 pure-u-md-1-1\">");
  html += newLine;
  html += F("<h1>Weißabgleich</h1>");
  html += newLine;
  html += F("<p>Rot, Grün und Blau der LEDs leuchten auf der max. Helligkeit. Schieben Sie die Regler so, dass ein natürlich wirkendes Weiß ensteht.</p>");
  html += newLine;
  html += F("<p>Setzen Sie min. 1 Regler auf 9, sonst wird niemals die max Helligkeit erzielt. Je höher die eingstellten Werte, desto heller scheinen die LEDs.</p>");
  html += newLine;
  html += F("<p>Default ist 9:6:2</p>");
  html += newLine;
  html += F("<fieldset>");
  html += newLine;
  html += F("<legend>Wertebereich jeweils von 1 bis 9</legend>");
  html += newLine;
  html += F("<form class=\"pure-form\">");
  html += newLine;
  html += "Rot <input type=\"number\" min=\"1\" max=\"9\" value=\"" + String(gRot) + "\" name=\"wrot\">";
  html += newLine;
  html += "Grün <input type=\"number\" min=\"1\" max=\"9\" value=\"" + String(gGruen) + "\" name=\"wgruen\">";
  html += newLine;
  html += "Blau <input type=\"number\" min=\"1\" max=\"9\" value=\"" + String(gBlau) + "\" name=\"wblau\">";
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<button type=\"submit\" class=\"pure-button ion-checkmark\"> Speichern / Testen</button>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("<a href=\"/\" class=\"pure-button ion-close\"> Verwerfen</a>");
  html += newLine;
  html += F("</form>");
  html += newLine;
  html += F("</fieldset>");
  html += newLine;
  html += F("<br>");
  html += newLine;
  html += F("</div>");
  html += newLine;
  html += F("</div>");
  html += newLine;
  html += F("</body>");
  html += newLine;
  html += F("</html>");
  html += newLine;


  setColor(255, 255, 255);
  server.send(200, "text/html", html);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send (404, "text/plain", message );
}

void setColor(int rot, int gruen, int blau) {
  //Serial.print("RAW Werte: ");
  //Serial.print(rot);
  //Serial.print(" ");
  //Serial.print(gruen);
  //Serial.print(" ");
  //Serial.print(blau);
  //Serial.println(" ");

  //RGB FIX
  //r:g:b
  //3:6:1
  rot = rot * gRot;
  gruen = gruen * gGruen;
  blau = blau  * gBlau;
  //RGB korrigieren Ende

  //Serial.print("KORR Werte: ");
  //Serial.print(rot);
  //Serial.print(" ");
  //Serial.print(gruen);
  //Serial.print(" ");
  //Serial.print(blau);
  //Serial.println(" ");
  analogWrite(PIN_ROT, rot);
  analogWrite(PIN_GRUEN, gruen);
  analogWrite(PIN_BLAU, blau);
}

byte countDigits(int num) {
  byte count = 0;
  while (num) {
    num = num / 10;
    count++;
  }
  return count;
}



