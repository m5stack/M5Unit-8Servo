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
  Analog signal acquisition
  模拟信号采集
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
    canvas.setPaletteColor(1, ORANGE);
    while (!unit_8servo.begin(
        &Wire, 21, 22,
        M5_UNIT_8SERVO_DEFAULT_ADDR)) {  // Initialize 8Servos Unit
                                         // 初始化EXT.IO2
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
    // unit_8servo.setAllPinMode(DIGITAL_INPUT_MODE);
    // unit_8servo.setAllPinMode(DIGITAL_OUTPUT_MODE);
    unit_8servo.setAllPinMode(
        ADC_INPUT_MODE);  // Set all pins to ADC input mode.
                          // 设置所有引脚为ADC输入模式
    // unit_8servo.setAllPinMode(SERVO_CTL_MODE);
    // unit_8servo.setAllPinMode(RGB_LED_MODE);
}

char info[50];

void loop() {
    canvas.fillSprite(0);
    canvas.setTextSize(2);
    canvas.drawString("ADC INPUT MODE", 10, 10);
    canvas.drawString("FW VERSION: " + String(unit_8servo.getVersion()), 10,
                      40);  // Get firmware version. 获取固件版本
    for (uint8_t i = 0; i < 8; i++) {
        uint16_t adc =
            unit_8servo.getAnalogInput(i, _12bit);  // Get ADC value. 获取ADC值
        Serial.printf("CH:%d ADC: %d", i, adc);
        canvas.drawRect(0, i * 20 + 75, 200, 15, 1);
        canvas.fillRect(0, i * 20 + 75, map(adc, 0, 4096, 0, 200), 15, 1);
        canvas.setCursor(220, i * 28 + 10);
        canvas.setTextSize(1);
        canvas.printf("CH:%d ADC: %d", i, adc);
    }
    canvas.pushSprite(0, 0);
    vTaskDelay(100);
}
