/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套 M5Core 示例源代码
*
* Visit for more information: https://docs.m5stack.com/en/unit/scales
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/scales
*
* Product: Unit Scales.
* Date: 2022/7/5
*******************************************************************************
  Config Scales Unit I2C Address
  配置Scales Unit I2C 地址
*/

#include <M5Stack.h>
#include <M5GFX.h>
#include "M5_Scales.h"

M5GFX display;
M5Canvas canvas(&display);
M5_Scales scales;

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(1);
    canvas.setPaletteColor(1, ORANGE);
    while (!scales.begin(&Wire, 21, 22, M5_SCALES_DEFAULT_ADDR)) {
        Serial.println("scales connect error");
        M5.Lcd.print("scales connect error");
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
}

void loop() {
    M5.Lcd.drawString("UNIT SCALES", 70, 80, 4);
    M5.Lcd.drawString("FW VERSION:" + String(scales.getVersion()), 70, 120, 4);
    M5.Lcd.drawString("Click BtnB Update Addr to 0x66", 60, 160, 2);
    if (M5.BtnB.wasPressed()) {
        if (scales.setDeviceAddr(0x66)) {
            Serial.println("Update Addr: 0x66");
            M5.Lcd.drawString("Update Addr: 0x66 success", 60, 200, 2);
        }
    }
    M5.update();
}
