/*!
 * @brief An compact peripherals for weighing From M5Stack
 * @copyright Copyright (c) 2022 by M5Stack[https://m5stack.com]
 *
 * @Links [Scales](https://docs.m5stack.com/en/unit/scales)
 * @version  V0.0.1
 * @date  2022-07-04
 */
#ifndef _M5_SCALES_H_
#define _M5_SCALES_H_

#include <Arduino.h>
#include <Wire.h>
#include "pins_arduino.h"

// I2C
#define M5_SCALES_DEFAULT_ADDR 0x26

// SCALES REGISTER
#define M5_SCALES_RAW_ADC_REG             0x10
#define M5_SCALES_CAL_DATA_REG            0x14
#define M5_SCALES_SET_OFFESET_REG         0x18
#define M5_SCALES_SET_CURRENT_OFFESET_REG 0x24

// CAL
#define M5_SCALES_ZERO_GRAM_REG 0x30
#define M5_SCALES_CAL_GRAM_REG  0x32

// RGB LED REGISTER RGB888
#define M5_SCALES_RGB_LED_SYNC_REG 0x25
#define M5_SCALES_RGB_LED_REG      0x50

// BUTTON REGISTER
#define M5_SCALES_BUTTON_COUNT_REG              0x20
#define M5_SCALES_BUTTON_LONG_PRESS_COUNT_REG   0x21
#define M5_SCALES_BUTTON_STATUS_REG             0x22
#define M5_SCALES_BUTTON_OFFSET_FUNC_ENABLE_REG 0x23

// Device Info
#define M5_SCALES_FW_VERSION_REG 0xFE
#define M5_SCALES_ADDRESS_REG    0xFF

class M5_Scales {
   private:
    uint8_t _addr;
    TwoWire *_wire;
    uint8_t _sda;
    uint8_t _scl;
    bool writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);
    bool readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer, uint8_t length);

   public:
    bool begin(TwoWire *wire = &Wire, uint8_t sda = SDA, uint8_t scl = SCL,
               uint8_t addr = M5_SCALES_DEFAULT_ADDR);
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
