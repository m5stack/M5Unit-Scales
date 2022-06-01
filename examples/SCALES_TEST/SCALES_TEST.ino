/**
 * @file SCALES_TEST.ino
 * @author Sean
 * @brief Scales Unit Test.
 * @version 0.1
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <M5Stack.h>
#include <M5GFX.h>
#include "UNIT_SCALES.h"

M5GFX display;
M5Canvas canvas(&display);
UNIT_SCALES scales;

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(1);
    canvas.setPaletteColor(1, ORANGE);
    while (!scales.begin(&Wire, 21, 22, UNIT_SCALES_DEFAULT_ADDR)) {
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
    scales.setBtnOffsetControl(false);
}

bool LEDSync          = false;
uint32_t color_list[] = {0xff0000, 0x00ff00, 0x0000ff};
uint8_t color_index   = 0;

void loop() {
    canvas.fillSprite(0);
    canvas.setTextSize(1);
    canvas.drawString("UNIT SCALES STATUS", 10, 10);
    int weight = scales.getWeight();
    int adc    = scales.getRawADC();
    canvas.drawString("WEIGHT: " + String(weight) + "g", 10, 35);
    canvas.drawString("ADC: " + String(adc), 10, 60);
    canvas.drawString(
        "BUTTON: " + String(scales.getBtnStatus() ? "Pressed" : "Released"), 10,
        85);
    canvas.drawString("BUTTON COUNT: " + String(scales.getBtnPressedCount()),
                      10, 110);
    canvas.drawString(
        "LONG PRESS COUNT: " + String(scales.getBtnLongPressedCount()), 10,
        135);

    canvas.drawString("LED COLOR: 0x" + String(scales.getLEDColor(), HEX), 10,
                      160);

    canvas.drawString("SCALES GAIN: " + String(scales.getScale()), 10, 185);

    canvas.drawString("FW VERSION: " + String(scales.getVersion()), 10, 210);

    canvas.pushSprite(0, 0);

    if (M5.BtnA.wasPressed()) {
        LEDSync = !LEDSync;
        if (scales.setLEDSyncWeight(LEDSync)) {
            if (LEDSync) {
                Serial.println("Enable LED Sync Weight");
            } else {
                Serial.println("Disable LED Sync Weight");
                scales.setLEDColor(color_list[color_index]);
            }
        };
    }
    if (M5.BtnB.wasPressed()) {
        // Set the current ADC value to Offset
        scales.setOffset();
        // Manually set Offset
        // scales.setOffset(12345);
        // scales.setScale(1.56);
    }
    if (M5.BtnC.wasPressed()) {
        if (color_index >= 2) {
            color_index = 0;
        } else {
            color_index++;
        }
        scales.setLEDColor(color_list[color_index]);
    }
    M5.update();
}
