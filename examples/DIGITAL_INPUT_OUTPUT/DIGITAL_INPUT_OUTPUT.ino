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
  Digital signal input and output
  数字信号输入和输出
*/
#include <M5Stack.h>
#include <M5GFX.h>
#include "M5_UNIT_8SERVO.h"

M5GFX display;
M5Canvas canvas(&display);
M5_UNIT_8SERVO unit_8servo;

extio_io_mode_t mode = DIGITAL_OUTPUT_MODE;

void btnTask(void *pvParameters) {
    while (1) {
        if (M5.BtnA.wasPressed()) {
            if (mode == DIGITAL_INPUT_MODE) {
                mode = DIGITAL_OUTPUT_MODE;
            } else {
                mode = DIGITAL_INPUT_MODE;
            }
        }
        M5.update();
        vTaskDelay(80);
    }
}

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(2);
    canvas.setPaletteColor(1, ORANGE);
    while (!unit_8servo.begin(&Wire, 21, 22, M5_UNIT_8SERVO_DEFAULT_ADDR)) {
        Serial.println("extio Connect Error");
        M5.Lcd.print("extio Connect Error");
        delay(100);
    }
    // unit_8servo.setAllPinMode(DIGITAL_INPUT_MODE);
    unit_8servo.setAllPinMode(
        DIGITAL_OUTPUT_MODE);  // Set all pins to digital output mode.
                               // 设置所有引脚为数字输出模式
    // unit_8servo.setAllPinMode(ADC_INPUT_MODE);
    // unit_8servo.setAllPinMode(SERVO_CTL_MODE);
    // unit_8servo.setAllPinMode(RGB_LED_MODE);

    // Create a task of btnTask and assign core 0 to handle it。
    // 创建 btnTask 任务并将其分配给 CPU 0 处理。
    xTaskCreatePinnedToCore(btnTask, "btnTask"  // A name just for humans
                            ,
                            4096  // This stack size can be checked & adjusted
                                  // by reading the Stack Highwater
                            ,
                            NULL,
                            1  // Priority, with 3 (configMAX_PRIORITIES - 1)
                               // being the highest, and 0 being the lowest.
                            ,
                            NULL, 0);
}

char info[50];

void loop() {
    if (mode == DIGITAL_INPUT_MODE) {
        canvas.fillSprite(0);
        canvas.drawString("DIGITAL INPUT MODE", 10, 10);
        canvas.drawString("FW VERSION: " + String(unit_8servo.getVersion()), 10,
                          40);
        for (uint8_t i = 0; i < 8; i++) {
            if (unit_8servo.getDigitalInput(i)) {
                canvas.fillRect(i * 30 + 30, 145, 28, 30, 1);
            } else {
                canvas.drawRect(i * 30 + 30, 145, 28, 30, 1);
            }
        }
        canvas.pushSprite(0, 0);
    } else if (mode == DIGITAL_OUTPUT_MODE) {
        canvas.fillSprite(0);
        canvas.drawString("DIGITAL OUTPUT MODE", 10, 10);
        canvas.drawString("FW VERSION: " + String(unit_8servo.getVersion()), 10,
                          40);
        for (uint8_t i = 0; i < 8; i++) {
            unit_8servo.setDigitalOutput(i, HIGH);
            canvas.drawRect(i * 30 + 30, 145, 28, 30, 1);
            canvas.pushSprite(0, 0);
            vTaskDelay(100);
        }
        for (uint8_t i = 0; i < 8; i++) {
            unit_8servo.setDigitalOutput(i, LOW);
            canvas.fillRect(i * 30 + 30, 145, 28, 30, 1);
            canvas.pushSprite(0, 0);
            vTaskDelay(100);
        }
        canvas.pushSprite(0, 0);
    }
    unit_8servo.setAllPinMode(mode);
    vTaskDelay(100);
}
