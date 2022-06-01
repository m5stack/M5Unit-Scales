#ifndef _UNIT_SCALES_H_
#define _UNIT_SCALES_H_

#include <Arduino.h>
#include <Wire.h>

// I2C
#define UNIT_SCALES_DEFAULT_ADDR 0x26

// SCALES REGISTER
#define UNIT_SCALES_RAW_ADC_REG             0x10
#define UNIT_SCALES_CAL_DATA_REG            0x14
#define UNIT_SCALES_SET_OFFESET_REG         0x18
#define UNIT_SCALES_SET_CURRENT_OFFESET_REG 0x24

// CAL
#define UNIT_SCALES_ZERO_GRAM_REG 0x30
#define UNIT_SCALES_CAL_GRAM_REG  0x32

// RGB LED REGISTER RGB888
#define UNIT_SCALES_RGB_LED_SYNC_REG 0x25
#define UNIT_SCALES_RGB_LED_REG      0x50

// BUTTON REGISTER
#define UNIT_SCALES_BUTTON_COUNT_REG              0x20
#define UNIT_SCALES_BUTTON_LONG_PRESS_COUNT_REG   0x21
#define UNIT_SCALES_BUTTON_STATUS_REG             0x22
#define UNIT_SCALES_BUTTON_OFFSET_FUNC_ENABLE_REG 0x23

// Device Info
#define UNIT_SCALES_FW_VERSION_REG 0xFE
#define UNIT_SCALES_ADDRESS_REG    0xFF

class UNIT_SCALES {
   private:
    uint8_t _addr;
    TwoWire *_wire;
    uint8_t _sda;
    uint8_t _scl;
    bool writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);
    bool readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);

   public:
    bool begin(TwoWire *wire = &Wire, uint8_t sda = 21, uint8_t scl = 22,
               uint8_t addr = UNIT_SCALES_DEFAULT_ADDR);
    // SCALES
    uint32_t getRawADC();
    int getWeight();
    bool setOffset();
    bool setOffset(uint32_t offset);
    bool setZeroGram();
    bool setCalGram(uint16_t g);
    // RGB LED CTL
    // uint8_t status();
    bool setLEDColor(uint32_t color);
    uint32_t getLEDColor();
    bool setLEDSyncWeight(bool enable);
    // BUTTON
    bool getBtnStatus();
    uint8_t getBtnPressedCount();
    uint8_t getBtnLongPressedCount();
    bool setBtnOffsetControl(bool enable);
    // FW
    bool setDeviceAddr(uint8_t addr);
    uint8_t getVersion();
};

#endif
