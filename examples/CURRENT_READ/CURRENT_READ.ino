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
  Current read
  电流读取
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
    canvas.setPaletteColor(1, GREEN);
    while (!unit_8servo.begin(&Wire, 21, 22, M5_UNIT_8SERVO_DEFAULT_ADDR)) {
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
}

void loop() {
    canvas.fillSprite(0);
    canvas.setTextSize(2);
    canvas.drawString("CURRENT READ", 10, 10);
    canvas.drawString("FW VERSION: " + String(unit_8servo.getVersion()), 10,
                      40);
    canvas.setCursor(10, 70);
    canvas.printf("Current:%.2fmA", unit_8servo.getServoCurrent() * 1000.0f);
    delay(100);
    canvas.pushSprite(0, 0);
}
