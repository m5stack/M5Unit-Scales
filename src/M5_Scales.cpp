#include "M5_Scales.h"

bool M5_Scales::begin(TwoWire *wire, uint8_t sda, uint8_t scl, uint8_t addr) {
    _wire = wire;
    _addr = addr;
    _sda  = sda;
    _scl  = scl;
    _wire->begin((int)_sda, (int)_scl, 400000UL);
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

bool M5_Scales::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                           uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->write(buffer, length);
    if (_wire->endTransmission() == 0) return true;
    return false;
}

bool M5_Scales::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                          uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    if (_wire->requestFrom(addr, length)) {
        for (uint8_t i = 0; i < length; i++) {
            buffer[index++] = _wire->read();
        }
        return true;
    }
    return false;
}

bool M5_Scales::getBtnStatus() {
    uint8_t data = 0;
    readBytes(_addr, M5_SCALES_BUTTON_STATUS_REG, &data, 1);
    return !data;
}
uint8_t M5_Scales::getBtnPressedCount() {
    uint8_t data = 0;
    readBytes(_addr, M5_SCALES_BUTTON_COUNT_REG, &data, 1);
    return data;
}

uint8_t M5_Scales::getBtnLongPressedCount() {
    uint8_t data = 0;
    readBytes(_addr, M5_SCALES_BUTTON_LONG_PRESS_COUNT_REG, &data, 1);
    return data;
}

bool M5_Scales::setLEDSyncWeight(bool enable) {
    uint8_t data = enable ? 1 : 0;
    return writeBytes(_addr, M5_SCALES_RGB_LED_SYNC_REG, &data, 1);
}

bool M5_Scales::setBtnOffsetControl(bool enable) {
    uint8_t data = enable ? 1 : 0;
    return writeBytes(_addr, M5_SCALES_BUTTON_OFFSET_FUNC_ENABLE_REG, &data, 1);
}

bool M5_Scales::setLEDColor(uint32_t colorHEX) {
    uint8_t color[4] = {0};
    // RED
    color[0] = (colorHEX >> 16) & 0xff;
    // GREEN
    color[1] = (colorHEX >> 8) & 0xff;
    // BLUE
    color[2] = colorHEX & 0xff;
    return writeBytes(_addr, M5_SCALES_RGB_LED_REG, color, 4);
}

uint32_t M5_Scales::getLEDColor() {
    uint8_t color[4]  = {0};
    uint32_t colorHEX = 0;
    if (readBytes(_addr, M5_SCALES_RGB_LED_REG, color, 4)) {
        colorHEX = color[0];
        colorHEX = (colorHEX << 8) | color[1];
        colorHEX = (colorHEX << 8) | color[2];
    };
    return colorHEX;
}

int M5_Scales::getWeight() {
    uint8_t data[4] = {0};
    int weight      = 0;
    if (readBytes(_addr, M5_SCALES_CAL_DATA_REG, data, 4)) {
        weight = data[0];
        weight = (weight << 8) | data[1];
        weight = (weight << 8) | data[2];
        weight = (weight << 8) | data[3];
    };
    return weight / 100;
}

uint32_t M5_Scales::getRawADC() {
    uint8_t data[4] = {0};
    int rawADC      = 0;
    if (readBytes(_addr, M5_SCALES_RAW_ADC_REG, data, 4)) {
        rawADC = data[0];
        rawADC = (rawADC << 8) | data[1];
        rawADC = (rawADC << 8) | data[2];
        rawADC = (rawADC << 8) | data[3];
    };
    return rawADC;
}

bool M5_Scales::setOffset() {
    uint8_t data = 1;
    return writeBytes(_addr, M5_SCALES_SET_CURRENT_OFFESET_REG, &data, 1);
}

bool M5_Scales::setOffset(uint32_t offset) {
    uint8_t data[4] = {0};
    data[0]         = (offset >> 16) & 0xff;
    data[1]         = (offset >> 8) & 0xff;
    data[2]         = offset & 0xff;
    return writeBytes(_addr, M5_SCALES_SET_OFFESET_REG, data, 4);
}

bool M5_Scales::setZeroGram() {
    uint8_t data[2] = {0};
    return writeBytes(_addr, M5_SCALES_ZERO_GRAM_REG, data, 2);
}

bool M5_Scales::setCalGram(uint16_t g) {
    uint8_t data[2] = {0};
    data[0]         = g & 0xff;
    data[1]         = (g >> 8) & 0xff;
    return writeBytes(_addr, M5_SCALES_CAL_GRAM_REG, data, 2);
}

uint8_t M5_Scales::getVersion() {
    uint8_t data = 0;
    readBytes(_addr, M5_SCALES_FW_VERSION_REG, &data, 1);
    return data;
}

bool M5_Scales::setDeviceAddr(uint8_t addr) {
    if (writeBytes(_addr, M5_SCALES_ADDRESS_REG, &addr, 1)) {
        _addr = addr;
        return true;
    } else {
        return false;
    }
}
