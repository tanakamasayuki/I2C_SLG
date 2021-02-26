#ifndef __I2C_SLG46826_H__
#define __I2C_SLG46826_H__

#include <Wire.h>

class I2C_SLG46826 {
  public:
    I2C_SLG46826(TwoWire &i2cWire = Wire) {
      _i2cWire = &i2cWire;
    }

    bool scanSlg(uint8_t controlCode) {
      if (16 <= controlCode) {
        return false;
      }

      for (uint8_t i = 0; i < 4; i++) {
        _i2cWire->beginTransmission(controlCode * 8 + i);
        byte error = _i2cWire->endTransmission();
        if (error != 0) {
          return false;
        }
      }

      return true;
    }

    bool readSlg(uint8_t controlCode, uint8_t data[256]) {
      if (!scanSlg(controlCode)) {
        return false;
      }

      for (uint8_t i = 0; i < 16; i++) {
        readRegEx(controlCode * 8 + 2, i * 16, &data[i * 16], 16);
      }

      return true;
    }

    bool writeSlg(uint8_t controlCode, const uint8_t data[256]) {
      if (!scanSlg(controlCode)) {
        return false;
      }

      for (uint8_t i = 0; i < 16; i++) {
        writeRegEx(controlCode * 8 + 2, i * 16, &data[i * 16], 16);
      }

      return true;
    }

    bool eraseSlg(uint8_t controlCode) {
      if (!scanSlg(controlCode)) {
        return false;
      }

      for (uint8_t i = 0; i < 16; i++) {
        writeReg(controlCode * 8 + 1, 0xE3, 0x80 + i);
      }

      return true;
    }

    bool resetSlg(uint8_t controlCode) {
      if (!scanSlg(controlCode)) {
        return false;
      }

      writeReg(controlCode * 8 + 1, 0xC8, 0x02);

      return true;
    }

    bool digitalRead(uint8_t controlCode, uint8_t pin) {
      if (!scanSlg(controlCode)) {
        return false;
      }

      if (pin <= 8) {
        uint8_t val = readReg(controlCode * 8 + 1, 0x74);
        if (pin == 0) {
          return val & (1 << 1);
        } else if (pin == 1) {
          return val & (1 << 2);
        } else if (pin == 2) {
          return val & (1 << 3);
        } else if (pin == 3) {
          return val & (1 << 4);
        } else if (pin == 4) {
          return val & (1 << 5);
        } else if (pin == 5) {
          return val & (1 << 6);
        } else if (pin == 8) {
          return val & (1 << 7);
        }
      } else {
        uint8_t val = readReg(controlCode * 8 + 1, 0x75);
        if (pin == 9) {
          return val & (1 << 0);
        } else if (pin == 10) {
          return val & (1 << 1);
        } else if (pin == 11) {
          return val & (1 << 2);
        } else if (pin == 12) {
          return val & (1 << 3);
        } else if (pin == 13) {
          return val & (1 << 4);
        } else if (pin == 14) {
          return val & (1 << 5);
        }
      }

      return false;
    }

    void writeReg(uint8_t deviceAddress, uint8_t reg, uint8_t data) {
      uint32_t clock = _i2cWire->getClock();
      _i2cWire->setClock(100000);
      _i2cWire->beginTransmission(deviceAddress);
      _i2cWire->write(reg);
      _i2cWire->write(data);
      _i2cWire->endTransmission();
      _i2cWire->setClock(clock);
      delay(100);
    }

    void writeRegEx(uint8_t deviceAddress, uint8_t reg, const uint8_t *data, uint8_t size) {
      uint32_t clock = _i2cWire->getClock();
      _i2cWire->setClock(100000);
      _i2cWire->beginTransmission(deviceAddress);
      _i2cWire->write(reg);
      for (int i = 0; i < size; i++) {
        _i2cWire->write(data[i]);
      }
      _i2cWire->endTransmission();
      _i2cWire->setClock(clock);
      delay(100);
    }

    uint8_t readReg(uint8_t deviceAddress, uint8_t reg) {
      _i2cWire->beginTransmission(deviceAddress);
      _i2cWire->write(reg);
      _i2cWire->endTransmission();
      _i2cWire->requestFrom(deviceAddress, (uint8_t)1);
      return _i2cWire->read();
    }

    void readRegEx(uint8_t deviceAddress, uint8_t reg, uint8_t *data, uint8_t size) {
      _i2cWire->beginTransmission(deviceAddress);
      _i2cWire->write(reg);
      _i2cWire->endTransmission();
      _i2cWire->requestFrom(deviceAddress, size);
      for (uint8_t i = 0; i < size; i++) {
        data[i] = _i2cWire->read();
      }
    }

  private:
    TwoWire *_i2cWire;
};

#endif
