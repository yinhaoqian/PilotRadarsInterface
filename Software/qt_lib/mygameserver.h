#ifndef MYGAMESERVER_H
#define MYGAMESERVER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <playable.h>
#include <qtcommunicator.h>
#include <QThread>

class MyGameServer: public QObject {
	Q_OBJECT
  private:
	Playable<QT_TEMPALTE_PARAM> playable;
  public:
	MyGameServer(QObject *parent = nullptr);
	Q_INVOKABLE bool onButtonValueChanged(QString buttonId, bool value);
	Q_INVOKABLE bool onSliderValueChanged(QString sliderId, qreal value);
	bool setCommunicator(Communicator<QT_TEMPALTE_PARAM> *communicatorPointer);
  protected:
	void timerEvent(QTimerEvent *event) override final;
};

inline MyGameServer::MyGameServer(QObject *parent): QObject(parent) {
	startTimer(1);
}

inline bool MyGameServer::onButtonValueChanged(QString buttonId, bool value) {
	if(playable.communicatorPointer == nullptr) return false;
	bool status{true};
	if(buttonId == "button_launchButton") {
		dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer[0] = value;
		//qInfo() << "button_launchButton=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer.at(0) ;
	} else if(buttonId == "button_counterMeasureButton") {
		dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer[1] = value;
		//qInfo() << "button_counterMeasureButton=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer.at(1) ;
	} else if(buttonId == "button_piperUpButton") {
		dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer[2] = value;
		//qInfo() << "button_piperUpButton=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer.at(2) ;
	} else if(buttonId == "button_piperDownButton") {
		dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer[3] = value;
		//qInfo() << "button_piperDownButton=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer.at(3) ;
	} else if(buttonId == "SPACE") {
		playable.currentScopeMode = MODE_NOTSTARTED;
		//qInfo() << "button_piperDownButton=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->buttonIntermediateBuffer.at(3) ;
	} else
		status = false;
	return status;
}

inline bool MyGameServer::onSliderValueChanged(QString sliderId, qreal value) {
	value = value * 5;
	if(playable.communicatorPointer == nullptr) return false;
	bool status{true};
	if(sliderId == "slider_stickX") {
		dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->sliderIntermediateBuffer[0] = value;
		//qInfo() << "analogInputStickXAxisBuffer=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->sliderIntermediateBuffer.at(0);
	} else if(sliderId == "slider_stickY") {
		dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->sliderIntermediateBuffer[1] = value;
		//qInfo() << "analogInputStickYAxisBuffer=" << dynamic_cast<QtCommunicator*>(playable.communicatorPointer)->sliderIntermediateBuffer.at(1);
	} else
		status = false;
	return status;
}


inline bool MyGameServer::setCommunicator(Communicator<QT_TEMPALTE_PARAM> *communicatorPointer) {
	if(communicatorPointer == nullptr) return false;
	this->playable.setCommunicator(communicatorPointer);
	return true;
}



inline void MyGameServer::timerEvent(QTimerEvent *event) {
	playable.executeCycle();
}

#endif // MYGAMESERVER_H




