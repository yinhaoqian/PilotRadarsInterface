#ifndef QTCOMMUNICATOR_H
#define QTCOMMUNICATOR_H
#include "communicator.h"
#include <QDebug>
#include <QString>
#include <QPointer>
#include <QQuickItem>
#include <QRandomGenerator>
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include "myquickpainteditem.h"

#define SERIAL_ARDUINO_UNO_PRODUCT_IDENTIFIER 67
#define SERIAL_ARDUINO_UNO_VENDOR_IDENTIFIER 9025
#define SERIAL_ARDUINO_UNO_PORT_NAME COM10

#define QT_TEMPALTE_B 8
#define QT_TEMPALTE_T int
#define QT_TEMPALTE_D int
#define QT_TEMPALTE_A float

#define QT_TEMPALTE_PARAM QT_TEMPALTE_B,QT_TEMPALTE_T,QT_TEMPALTE_D,QT_TEMPALTE_A

class QtCommunicator: public Communicator<QT_TEMPALTE_PARAM> {
  private:
	QPointer<MyQuickPaintedItem> myQuickPaintedItemPointer{nullptr};
	QPointer<QQuickItem> rectangleRwr1Led{nullptr}, rectangleRwr2Led{nullptr}, rectangleBuzzer{nullptr};
	void platformSpecificPrint(char *messageText) override final {
		qInfo() << messageText;
	}
	void platformSpecificExit() override final {
		exit(1);
	}
	bool serialReceivingMode{false};
  public:
	QVector<bool> buttonIntermediateBuffer = QVector<bool>(4, false);
	QVector<qreal> sliderIntermediateBuffer = QVector<qreal>(2, 2.5);
	bool setMyQuickPaintedItem(MyQuickPaintedItem* myQuickPaintedItemPointer);
	bool setQQuickItemLeds(QQuickItem* rectangleRwr1Led, QQuickItem* rectangleRwr2Led, QQuickItem* rectangleBuzzer);
	bool platformSpecificUpdateBufferToPins() override final ;
	bool platformSpecificUpdatePinsToBuffer() override final ;
	bool platformSpecificFlashPixelToScreen(const QT_TEMPALTE_T x, const QT_TEMPALTE_T y) override final ;
	bool platformSpecificClearScreen() override final;
	QT_TEMPALTE_T platformSpecificRandomGenerator(const QT_TEMPALTE_T lowerLimit, const QT_TEMPALTE_T upperLimit) override final;
};


inline bool QtCommunicator::setMyQuickPaintedItem(MyQuickPaintedItem *myQuickPaintedItemPointer) {
	if(myQuickPaintedItemPointer == nullptr) return false;
	this->myQuickPaintedItemPointer = myQuickPaintedItemPointer;
	if(this->myQuickPaintedItemPointer == nullptr) return false;
	return true;
}

inline bool QtCommunicator::setQQuickItemLeds(QQuickItem *rectangleRwr1Led, QQuickItem *rectangleRwr2Led, QQuickItem *rectangleBuzzer) {
	if(rectangleRwr1Led == nullptr) return false;
	if(rectangleRwr2Led == nullptr) return false;
	if(rectangleBuzzer == nullptr) return false;
	this->rectangleRwr1Led = rectangleRwr1Led;
	this->rectangleRwr2Led = rectangleRwr2Led;
	this->rectangleBuzzer = rectangleBuzzer;
	if(this->rectangleRwr1Led == nullptr) return false;
	if(this->rectangleRwr2Led == nullptr) return false;
	if(this->rectangleBuzzer == nullptr) return false;
	return true;
}

inline bool QtCommunicator::platformSpecificUpdateBufferToPins() {
	if(this->rectangleRwr1Led == nullptr) return false;
	if(this->rectangleRwr2Led == nullptr) return false;
	if(this->rectangleBuzzer == nullptr) return false;
	this->rectangleRwr1Led->setProperty("color", this->digitalOutputRwr1LedBuffer ? QColor(Qt::red) : QColor(Qt::black));
	this->rectangleRwr2Led->setProperty("color", this->digitalOutputRwr2LedBuffer ? QColor(Qt::red) : QColor(Qt::black));
	this->rectangleBuzzer->setProperty("color", this->digitalOutputBuzzerBuffer ? QColor(Qt::red) : QColor(Qt::black));
	//To-Do Analog output not implemented
	return true;
}

inline bool QtCommunicator::platformSpecificUpdatePinsToBuffer() {
	this->digitalInputLaunchButtonBuffer = buttonIntermediateBuffer.at(0);
	this->digitalInputCounterMeasureBuffer = buttonIntermediateBuffer.at(1);
	this->digitalInputPiperUpButtonBuffer = buttonIntermediateBuffer.at(2);
	this->digitalInputPiperDownButtonBuffer = buttonIntermediateBuffer.at(3);
	this->analogInputStickXAxisBuffer = sliderIntermediateBuffer.at(0);
	this->analogInputStickYAxisBuffer = sliderIntermediateBuffer.at(1);
	return true;
}

inline bool QtCommunicator::platformSpecificFlashPixelToScreen(const QT_TEMPALTE_T x, const QT_TEMPALTE_T y) {
	if(this->myQuickPaintedItemPointer == nullptr) return false;
	bool status{true};
	status &= myQuickPaintedItemPointer->displayPixelOnScreen(static_cast<int>(x), static_cast<int>(y));
	return status;
}

inline bool QtCommunicator::platformSpecificClearScreen() {
	bool status{true};
	if(this->myQuickPaintedItemPointer == nullptr) return false;
	status &= this->myQuickPaintedItemPointer->clearScreen();
	return status;
}

inline QT_TEMPALTE_T QtCommunicator::platformSpecificRandomGenerator(const QT_TEMPALTE_T lowerLimit, const QT_TEMPALTE_T upperLimit) {
	if(lowerLimit >= upperLimit) return 0;
	qint32 maximumAbsoluteNumber = upperLimit - lowerLimit;
	quint32 generatedRandomNumber = QRandomGenerator::global()->bounded(0U, static_cast<quint32>(maximumAbsoluteNumber));
	QT_TEMPALTE_T convertedGeneratedRandomNumber = static_cast<QT_TEMPALTE_T>(generatedRandomNumber) + lowerLimit;
	return convertedGeneratedRandomNumber;
}
#endif // QTCOMMUNICATOR_H

