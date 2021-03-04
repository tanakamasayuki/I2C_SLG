#include <WiFi.h>
#include <WebServer.h>
#include "html.h"
#include "I2C_SLG46826.h"

WebServer server(80);

#define SLG46826_I2C_SDA 32
#define SLG46826_I2C_SCL 33

I2C_SLG46826 slg(Wire);

void handleRoot(void) {
  String html = htmlBodyString;
  html.replace("#TITLE#", "SLG Lib Home");
  String body = "";

  for (int controlCode = 0; controlCode < 16; controlCode++) {
    String level[2] = {"LOW", "HIGH"};
    if (slg.scanSlg(controlCode)) {
      // GPIO
      String htmlItem = htmlIODisplayString;
      htmlItem.replace("#CONTROL_CODE#", String(controlCode));
      htmlItem.replace("#IO0#", level[slg.digitalRead(controlCode, 0)]);
      htmlItem.replace("#IO1#", level[slg.digitalRead(controlCode, 1)]);
      htmlItem.replace("#IO2#", level[slg.digitalRead(controlCode, 2)]);
      htmlItem.replace("#IO3#", level[slg.digitalRead(controlCode, 3)]);
      htmlItem.replace("#IO4#", level[slg.digitalRead(controlCode, 4)]);
      htmlItem.replace("#IO5#", level[slg.digitalRead(controlCode, 5)]);
      htmlItem.replace("#IO8#", level[slg.digitalRead(controlCode, 8)]);
      htmlItem.replace("#IO9#", level[slg.digitalRead(controlCode, 9)]);
      htmlItem.replace("#IO10#", level[slg.digitalRead(controlCode, 10)]);
      htmlItem.replace("#IO11#", level[slg.digitalRead(controlCode, 11)]);
      htmlItem.replace("#IO12#", level[slg.digitalRead(controlCode, 12)]);
      htmlItem.replace("#IO13#", level[slg.digitalRead(controlCode, 13)]);
      htmlItem.replace("#IO14#", level[slg.digitalRead(controlCode, 14)]);

      body += htmlItem;

      // Read
      uint8_t data[256];
      htmlItem = "<h3>Read</h3>\n";
      memset(data, 0, sizeof(data));
      if (slg.readSlg(controlCode, data)) {
        for (int i = 0; i < 16; i++) {
          for (int j = 0; j < 16; j++) {
            char num[16];
            snprintf(num, 16, "%02X", data[i * 16 + j]);
            htmlItem += num;
          }
          htmlItem += "<br />\n";
        }
      }
      body += htmlItem;

      // Erase
      htmlItem = "<h3>Erase</h3>\n";
      htmlItem += "[<a href=\"/erase?controlCode=" + String(controlCode) + "\">Erase</a>]<br />\n";
      body += htmlItem;

      // Write
      htmlItem = htmlWriteString;
      htmlItem.replace("#CONTROL_CODE#", String(controlCode));
      body += htmlItem;
    }
  }

  html.replace("#BODY#", body);

  server.send(200, "text/html", html);
}

void handleErase(void) {
  int controlCode = server.arg("controlCode").toInt();

  String html = htmlBodyString;
  html.replace("#TITLE#", "SLG Lib Erase");

  String body = "";
  body += "<h2>Erase</h2>\n";
  if (slg.eraseSlg(controlCode)) {
    body += "SLG46826 erased.<br />\n";
  } else {
    body += "SLG46826 erase failure!<br />\n";
  }

  body += "Resetting SLG46826.<br />\n";
  slg.resetSlg(controlCode);

  body += "<br />\n";
  body += "[ <a href=\"/\">Retarn HOME</a> ]<br />\n";

  html.replace("#BODY#", body);
  server.send(200, "text/html", html);
}

void handleWrite(void) {
Serial.println("handleWrite");
  int controlCode = server.arg("controlCode").toInt();
  String hexString = server.arg("data");
  String html = htmlBodyString;
  html.replace("#TITLE#", "SLG Lib Write");

  String body = "";
  body += "<h2>Write</h2>\n";

  uint8_t hexData[16][16];
  memset(hexData, 0, sizeof(hexData));
  uint16_t hexStrLen = hexString.length();

  uint16_t pos = 0;
  for (int i = 0; i < 16; i++) {
    // find :
    while (pos < hexStrLen && hexString[pos] != ':') {
      pos++;
    }

    // data check
    if (hexStrLen < (pos + 43)) {
      body += "Intel HEX parse error!<br />\n";
      body += "<br />\n";
      body += "[ <a href=\"/\">Retarn HOME</a> ]<br />\n";

      html.replace("#BODY#", body);
      server.send(200, "text/html", html);
      return;
    }

    // data skip
    pos += 9;

    // hex to data
    for (int j = 0; j < 16; j++) {
      uint8_t byteData = 0;
      if ('A' <= hexString[pos]) {
        byteData += (hexString[pos] - 'A' + 10) * 16;
      } else {
        byteData += (hexString[pos] - '0') * 16;
      }
      pos++;
      if ('A' <= hexString[pos]) {
        byteData += hexString[pos] - 'A' + 10;
      } else {
        byteData += hexString[pos] - '0';
      }
      pos++;
      hexData[i][j] = byteData;
    }
  }

  // print
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      char num[16];
      snprintf(num, 16, "%02X", hexData[i][j]);
      body += num;
    }
    body += "<br />\n";
  }
  body += "<br />\n";

  slg.eraseSlg(controlCode);
  slg.resetSlg(controlCode);

  if (slg.writeSlg(0, &hexData[0][0])) {
    body += "Write SLG46826.<br />\n";
  } else {
    body += "Write SLG46826 failure!<br />\n";
  }
  slg.resetSlg(0);
  body += "Resetting SLG46826.<br />\n";

  body += "<br />\n";
  body += "[ <a href=\"/\">Retarn HOME</a> ]<br />\n";

  html.replace("#BODY#", body);
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Init I2C
  Wire.begin(SLG46826_I2C_SDA, SLG46826_I2C_SCL);

  WiFi.begin();
  Serial.println("WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  Serial.println("WiFi Connected.");
  Serial.print("IP = ");
  Serial.println(WiFi.localIP());

  // bind
  server.on("/", handleRoot);
  server.on("/erase", handleErase);
  server.on("/write", handleWrite);

  server.begin();
}

void loop() {
  server.handleClient();
}
