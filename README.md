# Vector Drawing Machine (C/C++)
## Design Overview
### What is the VDM-22?
The VDM-22 (Vector Drawing Machine – 22) is a rudimentary replica of the famous RP-21/RP-22 Sapfir radar’s pilot interface. The defining feature of the VDM-22 is its graphics system – an 8-bit dual digital-to-analog converter generating vector graphics for display on an analog oscilloscope operating in X-Y mode. 

The VDM-22 is capable of displaying both the B-Scope (before lock-on) and the C-scope (the chase mode). When the pilot starts the simulator, VDM-22 will draw a simulated bandit at the pilot’s 12 o’clock. The pilot will then have to maneuver his/her plane to achieve a lock on the bandit. 

Once a good lock is achieved, the VDM-22 will proceed to the C-scope, like its real-life counterpart. The C-scope is a forward-looking scope; its interfaced was designed so the pilot can fly the plane toward the target in 0-visibility conditions. In this scope, the pilot will need to again maneuver the aircraft to match the bandit’s evasive maneuvers, while keeping an eye on the RWR (radar warning receiver) to deploy countermeasures when the bandit attempts to lock his/her aircraft. 

When the bogey approaches around 2/3 of the ranging scale (the horizontal line on the C-Scope), the pilot will have to press the launch button to win the game. Launch authority toggle switches will have to be turned on before the launch button is pressed for VDM-22 to register the launch action.

There are three ways to fail the simulation. The first occurs when the pilot fails to keep the bandit within the radarscope using his/her joystick. The second happens when the pilot fails to launch the missile before the bandit approaches within 1/3 of the distance scale in C-scope. The third is when the pilot fails to react in time (5 seconds) by releasing countermeasures when the RWR prompts for an enemy lock.

### Design Evolution
The original design idea of the VDM-22 is to implement the F-14 tomcat’s powerful AN/AWG9 radar operated entirely by the RIO (Radar Intercept Officer). Like the MiG-21’s RP-22, the AN/AWG9 also has a CRT-based vector display, albeit much bigger in size. The difficulties of implementing such a design using the ATMega328 microcontroller soon became obvious:

1.	The F14’s B-scope and C-scope utilize two separate CRT displays, which means only one operating mode will be available at once if implemented in the project.
2.	The F14’s vector display has a much more complex user interface, with multiple characters, diagonal lines, circles, and numbers. The ATMega328’s limited computational capabilities and program space made it impossible to load all the graphics maps in memory while keeping the program running at a reasonable speed.
3.	The controls of the AWG9 radar, while rather comprehensive and easy to use (in 70s standards) still require at least 25-30 switches to operate at normal capacity. The space constraint of having to mount the entire machine in the Tektronix 5110 mainframe made it impossible to implement so many switches on board. And the idea of a 30-switch external controller is a bit too complicated for any player (and our programmer) to understand within our time constraints.
4.	During initial testing, we were able to determine that the resolution of the 5D10 digital waveform/storage plug-in (8bit @ 1MHz) used in my Tektronix 5110 oscilloscope is incapable of displaying the fine details of the AWG9 radar interface. This discovery delivered the last straw for the team’s decision to implement the relatively simple RP-22 interface instead.

Another significant design change is in the controls quadrant. The original design idea is to have the MiG-21’s symbolic left-curving center control stick 3D-printed and mounted on a “decapitated” Sparkfun joystick module. It later turned out to be infeasible as the properties of 3D-printed plastic made it extremely difficult to route any wire for the buttons through. The final design opted for a normal control quadrant with the switches on top and the original Sparkfun joystick mounted below. 

## Design Verification

### Enclosure Verification

Laser cutting models are directly loaded in the software, and we are able to see what it looks like before it gets printed. As our enclosure design is very simple, no extra steps rather than direct inspection is needed.

### Hardware Verification

Due to the complex nature of the design, no SPICE verification was conducted. The design was instead tested out on a breadboard connected to an Arduino UNO clone. The corresponding pins of the Arduino UNO were connected to the DAC situated on the breadboard. Despite the high switching speed, we managed to obtain a decent display on a testing BKPrecision oscilloscope which we shown in class during the project update presentation. 

### Software Verification

Because of the design complexity, it is impossible to debug simply using serial print from Arduino. The QT Framework is used to aid the debugging process, as it comes with a great GDB tool and a UI Framework. 

**My Quick Painter [ MyQuickPaintedItem.h extends QQuickPaintedItem.h] **

An Image buffer is encapsulated in this MyQuickPaintedItem for the QPainter to pull from synchronously before it knows what to draw on QML Canvas. This QImage object is 256*256 in dimensions, which matches our oscilloscope. QImage::Format_Mono format is used as our oscilloscope does not support color display. 
```
QImage imageToBePrinted;
```
The paint() function is reimplemented from QQuickPaintedItem, as it provides an API to use the legacy QPainter to paint an QImage to a QML Canvas using OpenGL. The second function clearScreen()simply clears up the image buffer imageToBePrinted to a blank screen. Lastly, displayPixelOnScreen()will update the pixel information to the image buffer.
```
void paint(QPainter *);
bool clearScreen();
bool displayPixelOnScreen();
```
**Communicator Object [ QtCommunicator.h extends Communicator.h ]**
The basic functionalities of Communicator Object interface are explained with details in Software Design Implementation section. Here the Communicator Object is reimplemented in a QT way, different from unocommunicator.h which reads input and writes output from and to digital pins, that all inputs will be read from QML buttons and sliders and outputs will be written to QML Rectangles. Apart from the overridden functions, several new are introduced. 

For outputs, three pointers rectangleRwr1Led, rectangleRwr2Led, and rectangleBuzzer pointing to QML Rectangles, the three LEDs below the circular screen (stored as its base type QQuickItem*) are encapsulated so that QML Rectangles can be called in C++. In the same way, a pointer of our customized painter myQuickPaintedItemPointer is encapsulated as well. Similarly for inputs, buttonIntermediateBuffer contains the current state of four buttons and sliderIntermediateBuffer contains the current position of two buttons, both is updated real-time in the UI thread. 
```
QPointer<MyQuickPaintedItem> myQuickPaintedItemPointer;
QPointer<QQuickItem> rectangleRwr1Led; 
QPointer<QQuickItem> rectangleRwr2Led;
QPointer<QQuickItem> rectangleBuzzer;
QVector<bool> buttonIntermediateBuffer;
QVector<qreal> sliderIntermediateBuffer;
```
**My Game Server (extending QObject)  [ MyGameServer.h ]**
The MyGameServer Object matches the top level .ino file, which works similarly to the setup and loop callbacks in Arduino IDE. The reason why an extra layer being necessary in QT is because direct access to the main event loop is impossible, so our customized event loop must be defined. 

The timerEvent() works the same with the Arduino loop callback. This function will be called indefinitely by a set period, which is defined in the constructor of MyGameServer Object.
```
void timerEvent(QTimerEvent *);
```
The Playable Object is instantiated and encapsulated in the MyGameServer Object. This step is done differently in Arduino, as an Playable Object should be manually instantiated globally. However, MyGameServer Object in this case acts like a bigger Playable Object, which will be manually instantiated in main.cpp from QT after all QML components are registered. 
```
Playable<QT_TEMPALTE_PARAM> playable;
```
Some slots are defined so that when any button or slider is toggled, a signal will be sent to the event handler, and the corresponding slot function will be called. Q_INVOKABLE macro is added before these slots so that these functions will be registered in the Meta-Object System as .moc file and subsequently can be directly in QML as a JavaScript function. 
```
Q_INVOKABLE bool onButtonValueChanged(QString, bool);
Q_INVOKABLE bool onSliderValueChanged(QString, qreal);
```
The last step is to simulate the user input output system and the oscilloscope display, so a quick UI interface skeleton is implemented in QML:

Some trivial lines of code needes to be added in the main.cpp. Our customized QObject of MyGameServer and MyQuickPaintedItem are registered as a QML Type:
```
qmlRegisterType<MyQuickPaintedItem>("…", 1, 0, "MyGameServer");
qmlRegisterType<MyQuickPaintedItem>("…", 1, 0, "MyQuickPaintedItem");
```
At the same time, create instances of MyGameServer Object and reimplemented Communicator Object:
```
MyGameServer myGameServer{};
QtCommunicator qtCommunicator{};
```
Use the QQmlContext engine to search for the pointers pointing to the three QML Rectangles representing three LEDs, and the MyQuickPaintedItem Object, aka my painter, from the Meta-Object tree, and passing them to the reimplemented Communicator Object:
```
QQmlContext* a = engine.rootContext();
a->setContextProperty("myGameServer", &myGameServer);
MyQuickPaintedItem* myQuickPaintedItem = qobject_cast<MyQuickPaintedItem*>(engine.rootObjects().at(0)->findChild<QQuickPaintedItem*>("…"));
QQuickItem* rectangleRwr1Led = qobject_cast<QQuickItem*>(engine.rootObjects().at(0)->findChild<QQuickItem*>("…"));
QQuickItem* rectangleRwr2Led = qobject_cast<QQuickItem*>(engine.rootObjects().at(0)->findChild<QQuickItem*>("…"));
QQuickItem* rectangleBuzzer = qobject_cast<QQuickItem*>(engine.rootObjects().at(0)->findChild<QQuickItem*>("…"));
qtCommunicator.setMyQuickPaintedItem(myQuickPaintedItem);
qtCommunicator.setQQuickItemLeds(rectangleRwr1Led, rectangleRwr2Led, rectangleBuzzer);
myGameServer.setCommunicator(&qtCommunicator);
```
After doing all that, a quick visualization and operation of a game play is available on the PC: 


When all the game behaviors fall within our expectation, the software implementation is deemed to be verified, and ready to be ported to the hardware.  

