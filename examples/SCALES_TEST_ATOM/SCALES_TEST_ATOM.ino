/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Atom sample source code
*                          配套 M5Atom 示例源代码
*
* Visit for more information: https://docs.m5stack.com/en/unit/scales
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/scales
*
* Product: Unit Scales.
* Date: 2022/7/5
*******************************************************************************

*/

#include <M5Atom.h>
#include "M5_Scales.h"

M5_Scales scales;

unsigned long delay_time = 0;

void setup() {
    M5.begin(true, false, true);
    while (!scales.begin(&Wire1, 26, 32, M5_SCALES_DEFAULT_ADDR)) {
        Serial.println("scales connect error");
        delay(1000);
    }
    bool ret = true;
    ret &= scales.setLEDSyncWeight(true);
    ret &= scales.setBtnOffsetControl(true);
    ret &= scales.setLEDColor(0x0000ff);
    if (ret) {
        Serial.println("OK");
    } else {
        Serial.println("INIT ERROR");
    }
    scales.setBtnOffsetControl(false);
    delay_time = millis() + 500;
}

bool LEDSync          = false;
uint32_t color_list[] = {0xff0000, 0x00ff00, 0x0000ff};
uint8_t color_index   = 0;

void loop() {
    if (delay_time < millis()) {
        delay_time = millis() + 500;
        int weight = scales.getWeight();
        int adc    = scales.getRawADC();
        Serial.println("=========================");
        Serial.printf("WEIGHT: %+dg\r\n", weight);
        Serial.printf("ADC: %d\r\n", adc);
        Serial.println("=========================");
        if (scales.getBtnStatus()) {
            Serial.printf("BUTTON: Pressed\r\n");
        } else {
            Serial.printf("BUTTON: Released\r\n");
        }
        Serial.printf("BUTTON COUNT: %d\r\n", scales.getBtnPressedCount());
        Serial.printf("LONG PRESS COUNT: %d\r\n",
                      scales.getBtnLongPressedCount());
        Serial.println("=========================");

        Serial.printf("LED COLOR: 0x%X\r\n", scales.getLEDColor());

        if (LEDSync) {
            Serial.printf("LED Sync ON\r\n");
        } else {
            Serial.printf("LED Sync OFF\r\n");
        }
        Serial.println("=========================");
        Serial.println();
    }

    M5.update();
    if (M5.Btn.wasPressed()) {
        scales.setOffset();
        if (color_index >= 2) {
            color_index = 0;
        } else {
            color_index++;
        }
        scales.setLEDColor(color_list[color_index]);
    }
}
