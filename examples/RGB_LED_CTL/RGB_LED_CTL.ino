/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套 M5Core 示例源代码
*
* Visit for more information: https://docs.m5stack.com/en/unit/8Servos%20Unit
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/8Servos%20Unit
*
* Product: 8Servos Unit
* Date: 2023/8/2
*******************************************************************************
  RGB LED control
  RGB LED 灯控制
*/
#include <M5Stack.h>
#include <M5GFX.h>
#include "M5_UNIT_8SERVO.h"

M5GFX display;
M5Canvas canvas(&display);
M5_UNIT_8SERVO unit_8servo;

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setPaletteColor(1, GREEN);  // set palette color. 设置色调颜色
    while (!unit_8servo.begin(&Wire, 21, 22, M5_UNIT_8SERVO_DEFAULT_ADDR)) {
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
    // unit_8servo.setAllPinMode(DIGITAL_INPUT_MODE);
    // unit_8servo.setAllPinMode(DIGITAL_OUTPUT_MODE);
    // unit_8servo.setAllPinMode(ADC_INPUT_MODE);
    // unit_8servo.setAllPinMode(SERVO_CTL_MODE);
    unit_8servo.setAllPinMode(RGB_LED_MODE);  // Set all pins to RGB LED mode.
                                              // 设置所有引脚为RGB LED 模式
}

void loop() {
    for (uint8_t m = 0; m < 3; m++) {
        canvas.fillSprite(0);
        canvas.setTextSize(2);
        canvas.drawString("RGB LED MODE", 10, 10);
        canvas.drawString("FW VERSION: " + String(unit_8servo.getVersion()), 10,
                          40);
        for (uint8_t i = 0; i < 8; i++) {
            unit_8servo.setLEDColor(i, 0xff << (m * 8));
        }
        canvas.setCursor(10, 120);
        canvas.printf("COLOR: 0x%X", 0xff << (m * 8));
        canvas.pushSprite(0, 0);
        vTaskDelay(800);
    }
}
