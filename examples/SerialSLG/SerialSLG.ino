#include "I2C_SLG46826.h"

I2C_SLG46826 slg(Wire);

#define SLG46826_I2C_SDA 32
#define SLG46826_I2C_SCL 33

void setup() {
  // Init Serial
  Serial.begin(115200);
  delay(500);

  // Init I2C
  Wire.begin(SLG46826_I2C_SDA, SLG46826_I2C_SCL);

  // print
  dispHelp();
  scanSLG();
}

void dispHelp(void) {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 Command List\n");
  Serial.printf("===============================================================\n");
  Serial.printf(" ?                                 : This print\n");
  Serial.printf(" SCAN                              : SCAN SLG46826\n");
  Serial.printf(" READ  [Control Code:0-15]         : READ SLG46826\n");
  Serial.printf(" ERASE [Control Code:0-15]         : ERASE & RESET SLG46826\n");
  Serial.printf(" WRITE [Control Code:0-15] [DATA]  : WRITE SLG46826\n");
  Serial.printf(" RESET [Control Code:0-15]         : RESET SLG46826\n");
  Serial.printf(" GPIO  [Control Code:0-15]         : Display IO\n");
}

void scanSLG() {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 SCAN\n");
  Serial.printf("===============================================================\n");
  for (int i = 0; i < 16; i++) {
    if (slg.scanSlg(i)) {
      Serial.printf(" Control Code = %d\n", i);
    }
  }
}

void readSLG(int controlCode) {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 READ [Control Code = %d]\n", controlCode);
  Serial.printf("===============================================================\n");
  uint8_t data[256];
  memset(data, 0, sizeof(data));
  if (slg.readSlg(controlCode, data)) {
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        Serial.printf("%02X", data[i * 16 + j]);
      }
      Serial.println();
    }
  } else {
    Serial.printf("SLG46826 not found.\n");
  }
}

void eraseSLG(int controlCode) {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 ERASE [Control Code = %d]\n", controlCode);
  Serial.printf("===============================================================\n");
  if (slg.eraseSlg(controlCode)) {
    Serial.printf("SLG46826 erased.\n");
    slg.resetSlg(controlCode);
    Serial.printf("Resetting SLG46826.\n");
  } else {
    Serial.printf("SLG46826 not found.\n");
  }
}

void writeSLG(int controlCode, String hexString) {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 WRITE [Control Code = %d]\n", controlCode);
  Serial.printf("===============================================================\n");
  if (slg.eraseSlg(controlCode)) {
    Serial.printf("SLG46826 erased.\n");
    slg.resetSlg(controlCode);
    Serial.printf("Resetting SLG46826.\n");

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
        Serial.println("Error");
        while (1)
          ;
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
        Serial.printf("%02X", hexData[i][j]);
      }
      Serial.println();
    }

    slg.writeSlg(0, &hexData[0][0]);
    slg.resetSlg(0);
    Serial.printf("Resetting SLG46826.\n");
  } else {
    Serial.printf("SLG46826 not found.\n");
  }
}

void resetSLG(int controlCode) {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 RESET [Control Code = %d]\n", controlCode);
  Serial.printf("===============================================================\n");
  if (slg.resetSlg(controlCode)) {
    Serial.printf("Resetting SLG46826.\n");
  } else {
    Serial.printf("SLG46826 not found.\n");
  }
}

void gpioSLG(int controlCode) {
  Serial.printf("===============================================================\n");
  Serial.printf("SLG46826 GPIO [Control Code = %d]\n", controlCode);
  Serial.printf("===============================================================\n");
  for (int i = 0; i <= 14; i++) {
    Serial.printf("%d : IO%2d\n", slg.digitalRead(controlCode, i), i);
  }
}

void loop() {
  while (Serial.available()) {
    char input[1024];
    String command = Serial.readStringUntil('\n');
    command.trim();
    strncpy(input, command.c_str(), sizeof(input) - 1);
    char* command0 = strtok(input, " ");
    command = command0;
    command0 = strtok(NULL, " ");
    String command2 = command0;
    String command3 = "";
    while (1) {
      command0 = strtok(NULL, " ");
      if (command0 == NULL) {
        break;
      }
      command3 += command0;
    }

    if (command == "") {
      // Skip
    } else if (command == "?") {
      dispHelp();
    } else if (command == "SCAN") {
      scanSLG();
    } else if (command == "READ") {
      readSLG(command2.toInt());
    } else if (command == "ERASE") {
      eraseSLG(command2.toInt());
    } else if (command == "WRITE") {
      writeSLG(command2.toInt(), command3);
    } else if (command == "RESET") {
      resetSLG(command2.toInt());
    } else if (command == "GPIO") {
      gpioSLG(command2.toInt());
    } else {
      dispHelp();
    }

    Serial.println(">");
  }
}
