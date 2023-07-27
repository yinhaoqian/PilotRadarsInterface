#ifndef QDISPLAY_H
#define QDISPLAY_H

#include <QObject>
#include <QDebug>
#include <QImage>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QRandomGenerator>

class MyQuickPaintedItem:  public QQuickPaintedItem {
	Q_OBJECT
  private:
	void paint(QPainter *painter) override;
  public:
	MyQuickPaintedItem(QQuickItem *parent = nullptr);
	bool clearScreen();
	bool displayPixelOnScreen(const int x, const int y);
//	template <unsigned int P, typename T>
//	void uploadDisplayableToBuffer(Displayable<P, T> &displayable);
  private:
	QImage imageToBePrinted{256, 256, QImage::Format_Mono};

  public:
	Q_INVOKABLE void helloWorld();
};


inline void MyQuickPaintedItem::helloWorld() {
	clearScreen();
	imageToBePrinted.fill(1);//把所有像素点设置为低电平
	update();
}

inline MyQuickPaintedItem::MyQuickPaintedItem(QQuickItem *parent) : QQuickPaintedItem(parent) {
	clearScreen();
}

inline void MyQuickPaintedItem::paint(QPainter *painter) {
	painter->setClipping(true);
	painter->drawImage(0, 0, imageToBePrinted);
}

inline bool MyQuickPaintedItem::clearScreen() {
	imageToBePrinted.fill(0);//把所有像素点设置为低电平
	update();
	return true;
}

inline bool MyQuickPaintedItem::displayPixelOnScreen(const int x, const int y) {
	bool status{true};
	int convertedX = static_cast<int>(x) + 128L;
	int convertedY = -static_cast<int>(y) + 128L;
	status &= convertedX >= 0L;
	status &= convertedX <= imageToBePrinted.width();
	status &= convertedY >= 0L ;
	status &= convertedY <= imageToBePrinted.height();
	if(status)
		imageToBePrinted.setPixel(convertedX, convertedY, 1U);
	update();
	return status;
}





#endif // QDISPLAY_H
