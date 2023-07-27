#include "HardwareSerial.h"
#include "Arduino.h"
#ifndef UNOCOMMUNICATOR_H
#define UNOCOMMUNICATOR_H

#include "qt_lib/communicator.h"

#define VECTOR_GRAPHICS_DISPLAY_PERIOD 10000
#define UNO_TEMPALTE_B 8
#define UNO_TEMPALTE_T int16_t
#define UNO_TEMPALTE_D int16_t
#define UNO_TEMPALTE_A float
#define UNO_TEMPALTE_PARAM UNO_TEMPALTE_B, UNO_TEMPALTE_T, UNO_TEMPALTE_D, UNO_TEMPALTE_A
//数字针脚和模式


/**
 * 数字模拟转换器需要转换的数字信号的每一位代表的针脚索引
 * 注意！DB0是最右面的位 DB7是最左面的位
 * @brief DIGITALOUTPUT_DAC_DB[*]
 */
#define DIGITALOUTPUT_DAC_DB0 A0
#define DIGITALOUTPUT_DAC_DB1 A1
#define DIGITALOUTPUT_DAC_DB2 A2
#define DIGITALOUTPUT_DAC_DB3 A3
#define DIGITALOUTPUT_DAC_DB4 4
#define DIGITALOUTPUT_DAC_DB5 5
#define DIGITALOUTPUT_DAC_DB6 6
#define DIGITALOUTPUT_DAC_DB7 7

/**
 * 数字模拟转换器需要转换的模拟信号输出通道控制
 * 注意！低电平输出到通道A；高电平输出到通道B
 */
#define DIGITALOUTPUT_DAC_AB 8

/**
 * 数字模拟转换器需要转换的模拟信号更新控制组
 * 注意！低电平时会触发
 * 注意！如果低电平被保持，那么WR的高电平会更新输出模拟信号
 */
#define DIGITALOUTPUT_DAC_LDAC 3
#define DIGITALOUTPUT_DAC_WR 10

/**
 * 数字信号输出针脚，包含蜂鸣器和发光二极管
 * @brief DIGITALOUTPUT_[*]
 */
#define DIGITALOUTPUT_BUZZER 9
#define DIGITALOUTPUT_RWR1 0
#define DIGITALOUTPUT_RWR2 1

/**
 * 数字信号输入针脚，包含所有按钮
 * @brief DIGITALINPUT_[*]
 */
#define DIGITALINPUT_LAUNCHBUTTON 2
#define DIGITALINPUT_COUNTERMEASURE 11
#define DIGITALINPUT_PIPERUP 12
#define DIGITALINPUT_PIPERDOWN 13

/**
 * 串行接口针脚
 * 注意！串行针脚被用于发光二极管显示，谨慎使用
 */
#define DIGITALOUTPUT_RX 1
#define DIGITALOUTPUT_TX 0


//模拟针脚和模式
#define ANALOGINPUT_STICKXAXIS A4
#define ANALOGINPUT_STICKYAXIS A5

enum DacState { STATE_DAC_S1,
                STATE_DAC_S2,
                STATE_DAC_S3,
                STATE_DAC_S4,
                STATE_DAC_S5,
                STATE_DAC_S6 };

class UnoCommunicator : public Communicator<UNO_TEMPALTE_PARAM> {

public:
  bool serialUsage{ false };
private:
  bool digitalOutputVectorGraphicsXYAxisPortControl{ true };
  bool digitalOutputVectorGraphicsXYAxisVoltageBuffer[8]{ false };
  DacState currentDacState{ STATE_DAC_S1 };
  bool convertDecimalToBinaryAndStoreInDigitalOutputVoltageBuffer(const UNO_TEMPALTE_T xOrY);
  void platformSpecificPrint(char *messageText) override final;
  void platformSpecificExit() override final;
  void unoDigitalWrite(uint8_t pin, uint8_t val);
public:
  UnoCommunicator(bool serialUsage);
  bool platformSpecificUpdateBufferToPins() override final;
  bool platformSpecificUpdatePinsToBuffer() override final;
  bool platformSpecificFlashPixelToScreen(const UNO_TEMPALTE_T x, const UNO_TEMPALTE_T y) override final;
  bool platformSpecificClearScreen() override final;
  UNO_TEMPALTE_T platformSpecificRandomGenerator(const UNO_TEMPALTE_T lowerLimit, const UNO_TEMPALTE_T upperLimit) override final;
};


inline UnoCommunicator::UnoCommunicator(bool serialUsage) {

  this->serialUsage = serialUsage;
  pinMode(DIGITALOUTPUT_DAC_DB0, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB1, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB2, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB3, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB4, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB5, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB6, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_DB7, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_AB, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_LDAC, OUTPUT);
  pinMode(DIGITALOUTPUT_DAC_WR, OUTPUT);
  // pinMode(DIGITALOUTPUT_BUZZER, OUTPUT);
  if (this->serialUsage) {
    Serial.begin(9600);
    Serial.println("Serial_ON");
  } else {
    pinMode(DIGITALOUTPUT_RWR1, OUTPUT);
    pinMode(DIGITALOUTPUT_RWR2, OUTPUT);
  }
  pinMode(DIGITALINPUT_LAUNCHBUTTON, INPUT);
  pinMode(DIGITALINPUT_COUNTERMEASURE, INPUT);
  pinMode(DIGITALINPUT_PIPERUP, INPUT);
  pinMode(DIGITALINPUT_PIPERDOWN, INPUT);
}

inline bool UnoCommunicator::platformSpecificUpdateBufferToPins() {
  unoDigitalWrite(DIGITALOUTPUT_RWR1, digitalOutputRwr1LedBuffer ? HIGH : LOW);
  unoDigitalWrite(DIGITALOUTPUT_RWR2, digitalOutputRwr2LedBuffer ? HIGH : LOW);
  analogWrite(DIGITALOUTPUT_BUZZER, digitalOutputBuzzerBuffer ? 20 : 0);
  // analogWrite(DIGITALOUTPUT_BUZZER, analogInputStickXAxisBuffer*8);
}

inline bool UnoCommunicator::platformSpecificUpdatePinsToBuffer() {
  digitalInputLaunchButtonBuffer = (digitalRead(DIGITALINPUT_LAUNCHBUTTON) == HIGH) ? false : true;
  if (serialUsage) Serial.println(digitalInputLaunchButtonBuffer ? "TRUE" : "FALSE");
  digitalInputCounterMeasureBuffer = (digitalRead(DIGITALINPUT_COUNTERMEASURE) == HIGH) ? false : true;
  if (serialUsage) Serial.println(digitalInputLaunchButtonBuffer ? "TRUE" : "FALSE");
  digitalInputPiperUpButtonBuffer = (digitalRead(DIGITALINPUT_PIPERUP) == HIGH) ? false : true;
  if (serialUsage) Serial.println(digitalInputLaunchButtonBuffer ? "TRUE" : "FALSE");
  digitalInputPiperDownButtonBuffer = (digitalRead(DIGITALINPUT_PIPERDOWN) == HIGH) ? false : true;
  if (serialUsage) Serial.println(digitalInputLaunchButtonBuffer ? "TRUE" : "FALSE");
  analogInputStickXAxisBuffer = -((analogRead(ANALOGINPUT_STICKXAXIS) * (5.0 / 1023.0))-5);
  analogInputStickYAxisBuffer = -((analogRead(ANALOGINPUT_STICKYAXIS) * (5.0 / 1023.0))-5);
}

inline bool UnoCommunicator::platformSpecificFlashPixelToScreen(const UNO_TEMPALTE_T x, const UNO_TEMPALTE_T y) {
  if (x < -120 || x > 120 || y < -120 || y > 120) return false;
  digitalOutputVectorGraphicsXYAxisPortControl = false;
  currentDacState = STATE_DAC_S1;
  while (currentDacState != STATE_DAC_S6) {
    switch (currentDacState) {
      case STATE_DAC_S1:
        unoDigitalWrite(DIGITALOUTPUT_DAC_AB, digitalOutputVectorGraphicsXYAxisPortControl ? HIGH : LOW);
        currentDacState = STATE_DAC_S2;
        break;
      case STATE_DAC_S2:
        unoDigitalWrite(DIGITALOUTPUT_DAC_WR, LOW);
        currentDacState = STATE_DAC_S3;
        break;
      case STATE_DAC_S3:
        convertDecimalToBinaryAndStoreInDigitalOutputVoltageBuffer(digitalOutputVectorGraphicsXYAxisPortControl ? x : y);  //注意这里的LSB MSB是反过来的,并且x和y已经被互换、
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB0, digitalOutputVectorGraphicsXYAxisVoltageBuffer[0] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB1, digitalOutputVectorGraphicsXYAxisVoltageBuffer[1] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB2, digitalOutputVectorGraphicsXYAxisVoltageBuffer[2] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB3, digitalOutputVectorGraphicsXYAxisVoltageBuffer[3] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB4, digitalOutputVectorGraphicsXYAxisVoltageBuffer[4] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB5, digitalOutputVectorGraphicsXYAxisVoltageBuffer[5] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB6, digitalOutputVectorGraphicsXYAxisVoltageBuffer[6] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB7, digitalOutputVectorGraphicsXYAxisVoltageBuffer[7] ? HIGH : LOW);
        currentDacState = STATE_DAC_S4;
        break;
      case STATE_DAC_S4:
        unoDigitalWrite(DIGITALOUTPUT_DAC_WR, HIGH);
        currentDacState = STATE_DAC_S5;
        break;
      case STATE_DAC_S5:
        unoDigitalWrite(DIGITALOUTPUT_DAC_LDAC, LOW);
        currentDacState = STATE_DAC_S6;
        break;
      case STATE_DAC_S6:
        unoDigitalWrite(DIGITALOUTPUT_DAC_LDAC, HIGH);
        break;
    }
  }
  digitalOutputVectorGraphicsXYAxisPortControl = true;
   currentDacState = STATE_DAC_S1;
  while (currentDacState != STATE_DAC_S6) {
    switch (currentDacState) {
      case STATE_DAC_S1:
        unoDigitalWrite(DIGITALOUTPUT_DAC_AB, digitalOutputVectorGraphicsXYAxisPortControl ? HIGH : LOW);
        currentDacState = STATE_DAC_S2;
        break;
      case STATE_DAC_S2:
        unoDigitalWrite(DIGITALOUTPUT_DAC_WR, LOW);
        currentDacState = STATE_DAC_S3;
        break;
      case STATE_DAC_S3:
        convertDecimalToBinaryAndStoreInDigitalOutputVoltageBuffer(digitalOutputVectorGraphicsXYAxisPortControl ? x : y);  //注意这里的LSB MSB是反过来的,并且x和y已经被互换、
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB0, digitalOutputVectorGraphicsXYAxisVoltageBuffer[0] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB1, digitalOutputVectorGraphicsXYAxisVoltageBuffer[1] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB2, digitalOutputVectorGraphicsXYAxisVoltageBuffer[2] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB3, digitalOutputVectorGraphicsXYAxisVoltageBuffer[3] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB4, digitalOutputVectorGraphicsXYAxisVoltageBuffer[4] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB5, digitalOutputVectorGraphicsXYAxisVoltageBuffer[5] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB6, digitalOutputVectorGraphicsXYAxisVoltageBuffer[6] ? HIGH : LOW);
        unoDigitalWrite(DIGITALOUTPUT_DAC_DB7, digitalOutputVectorGraphicsXYAxisVoltageBuffer[7] ? HIGH : LOW);
        currentDacState = STATE_DAC_S4;
        break;
      case STATE_DAC_S4:
        unoDigitalWrite(DIGITALOUTPUT_DAC_WR, HIGH);
        currentDacState = STATE_DAC_S5;
        break;
      case STATE_DAC_S5:
        unoDigitalWrite(DIGITALOUTPUT_DAC_LDAC, LOW);
        currentDacState = STATE_DAC_S6;
        break;
      case STATE_DAC_S6:
        unoDigitalWrite(DIGITALOUTPUT_DAC_LDAC, HIGH);
        break;
    }
  }
  return true;
}

inline bool UnoCommunicator::platformSpecificClearScreen() {
}

inline UNO_TEMPALTE_T UnoCommunicator::platformSpecificRandomGenerator(const UNO_TEMPALTE_T lowerLimit, const UNO_TEMPALTE_T upperLimit) {
  UNO_TEMPALTE_T toReturn = static_cast<UNO_TEMPALTE_T>(random(lowerLimit, upperLimit));
  if (serialUsage) Serial.println(toReturn);
  return toReturn;
}

inline void UnoCommunicator::platformSpecificPrint(char *messageText) {
  if (serialUsage) Serial.println(messageText);
}

inline void UnoCommunicator::platformSpecificExit() {
  exit(1);
}

inline bool UnoCommunicator::convertDecimalToBinaryAndStoreInDigitalOutputVoltageBuffer(const UNO_TEMPALTE_T xOrY) {
  uint16_t shiftedXOrY = static_cast<uint16_t>(xOrY + 127U);
  for (size_t i = 0; i < 8; ++i) {
    digitalOutputVectorGraphicsXYAxisVoltageBuffer[i] = shiftedXOrY % 2;
    shiftedXOrY = shiftedXOrY / 2;
  }
  return true;
}

inline void UnoCommunicator::unoDigitalWrite(uint8_t pin, uint8_t val) {
  if (serialUsage) {
    String stringToSendToSerial{ "DW_" + String(pin) + (val == HIGH) ? "_H" : "_L" };
    Serial.println(stringToSendToSerial);
  } else {
    digitalWrite(pin, val);
  }
}

#endif