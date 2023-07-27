#ifndef GAMEPLAY_H
#define GAMEPLAY_H


#include "displayable.h"
#include "communicator.h"
//#include <QDebug>


#define LINE_WIDTH 1
#define CR_HEIGHT 10
#define CR_WIDTH 5
#define BR_HEIGHT 10
#define BR_WIDTH 60
#define PLANE_WIDTH 5
#define DIGIT_WIDTH 5

#define MAXABS_COORDINATE 120
#define DIGIT_DISPLAYTIME 2

#define PIPER_FREQ 100
#define REFERENCE_FREQ 100
#define TIMER_PERIOD 1000
#define AUTOMOVE_FREQ 50
#define DISTANCE_FREQ 500
#define BLINK_FREQ 10
#define ATTACK_PROBABLITY 10

#define DIGIT_PH_0 -MAXABS_COORDINATE
#define DIGIT_PH_H1 -MAXABS_COORDINATE+DIGIT_WIDTH*1-1
#define DIGIT_PH_V1 -MAXABS_COORDINATE+DIGIT_WIDTH*2-1
#define DIGIT_PH_2 0
#define DIGIT_PH_3 0

/**
 * 像素数组大小定义（自动判断）
 */
#define CR_ARRAY_SIZE (CR_WIDTH*LINE_WIDTH*2+1)*LINE_WIDTH*2
#define BR_ARRAY_SIZE LINE_WIDTH*2*BR_HEIGHT*LINE_WIDTH\
	+BR_WIDTH*LINE_WIDTH*2*LINE_WIDTH
#define PLANE_ARRAY_SIZE PLANE_WIDTH*4+1
#define DIGIT_ARRAY_SIZE DIGIT_WIDTH*14

enum ScopeMode {MODE_CSCOPE, MODE_BSCOPE, MODE_NOTSTARTED, MODE_GAMEOVER, MODE_SHOWSCORE, MODE_GAMEWIN};
enum AttackState {STATE_NEVERATTACKED, STATE_BEINGATTACKED, STATE_HAVEBEENATTACKED};
enum DigitState {STATE_DIGIT1, STATE_DIGIT2, STATE_DIGIT3};
template <unsigned char B, typename T, typename D, typename A>
class Playable {
  public:
	Communicator<B, T, D, A>* communicatorPointer;

	T planeCenter[2] {0, 0};

	T bReferenceCenter[2] {0, 0};

	T cReferenceYCenter{0};

	T cScopeTargetedShift{0};

	T bReferenceDistance{0};

	T bScopeTargetedShift[2] {0, 0};

	T bScopeAttackTimer{0};

	AttackState bScopeBeingAttacked {STATE_NEVERATTACKED};

	DigitState digitState {STATE_DIGIT1};

	T automoveTimer{0};

	T digitDisplayTimer{0};



	bool initializeCReferenceDisplayable(T yCenter);

	bool initializeBReferenceDisplayable(T xCenter, T yCenter, T distance);

	bool initializePlaneDisplayable(T xCenter, T yCenter);

	bool initializeDigitDisplayble(T xCenter, T yCenter, char digit);

	unsigned char difficultyLevel{1};

	unsigned char score{1};

	Displayable<CR_ARRAY_SIZE, T> cReferenceDisplayable{};
	Displayable<BR_ARRAY_SIZE, T> bReferenceDisplayable{};
	Displayable<PLANE_ARRAY_SIZE, T> planeDisplayable{};
	Displayable<DIGIT_ARRAY_SIZE, T> digitDisplayable{};
	ScopeMode currentScopeMode{MODE_GAMEOVER};

	bool setCommunicator(Communicator<B, T, D, A> * communicatorPointer);
	bool flashAllDisplayable();
	bool processInputAndGenerateOutput();
	bool executeCycle();
};

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::initializeCReferenceDisplayable(T yCenter) {
	bool status{true};
	T xCenter{0};
	T drawParameters[2][4] {
		//C坐标
		{
			xCenter - CR_WIDTH * LINE_WIDTH,
			xCenter + CR_WIDTH * LINE_WIDTH,
			yCenter + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH,
			yCenter + CR_HEIGHT * LINE_WIDTH + 1
		},
		{
			xCenter - CR_WIDTH * LINE_WIDTH,
			xCenter + CR_WIDTH * LINE_WIDTH,
			yCenter - CR_HEIGHT * LINE_WIDTH - 1,
			yCenter - CR_HEIGHT * LINE_WIDTH - LINE_WIDTH

		}
	};
	status &= this->cReferenceDisplayable.initializePixelInformation(drawParameters);
	if(!status) communicatorPointer
		->buildVerboseMessage("Playable<B, T, D, A>::initializeCReferenceDisplayable() failed");
	return status;
}


template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::initializeBReferenceDisplayable(T xCenter, T yCenter, T distance) {
	bool status{true};
	if(distance < 0 || distance > BR_WIDTH * LINE_WIDTH - LINE_WIDTH) return false;
	T drawParameters[4][4] {
		//B坐标
		{
			xCenter - BR_WIDTH * LINE_WIDTH - 2 * PLANE_WIDTH + distance,
			xCenter - BR_WIDTH * LINE_WIDTH - 2 * PLANE_WIDTH + distance,
			yCenter + BR_HEIGHT * LINE_WIDTH,
			yCenter + 1
		},
		{
			xCenter + BR_WIDTH * LINE_WIDTH + 2 * PLANE_WIDTH - distance,
			xCenter + BR_WIDTH * LINE_WIDTH + 2 * PLANE_WIDTH - distance,
			yCenter + BR_HEIGHT * LINE_WIDTH,
			yCenter + 1
		},
		{
			xCenter - BR_WIDTH * LINE_WIDTH - 2 * PLANE_WIDTH,
			xCenter - 2 * PLANE_WIDTH - 1,
			yCenter - 1,
			yCenter - 1
		},
		{
			xCenter + 2 * PLANE_WIDTH + 1,
			xCenter + BR_WIDTH * LINE_WIDTH + 2 * PLANE_WIDTH,
			yCenter - 1,
			yCenter - 1
		}
	};
	status &= this->bReferenceDisplayable.initializePixelInformation(drawParameters);
//	this->bReferenceDistance = status ? distance : this->bReferenceDistance ;
	if(!status) communicatorPointer
		->buildVerboseMessage("Playable<B, T, D, A>::initializeBReferenceDisplayable() failed");
	return status;
}

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::initializePlaneDisplayable(T xCenter, T yCenter) {
	bool status{true};
	T drawParameters[3][4] {
		//飞机坐标
		{
			xCenter,
			xCenter,
			yCenter + PLANE_WIDTH,
			yCenter - PLANE_WIDTH
		},
		{
			xCenter - PLANE_WIDTH,
			xCenter - 1,
			yCenter,
			yCenter
		},
		{
			xCenter + 1,
			xCenter + PLANE_WIDTH,
			yCenter,
			yCenter
		}
	};
	status &= this->planeDisplayable.initializePixelInformation(drawParameters);
//	this->planeCenter[0] = status ? xCenter : this->planeCenter[0];
//	this->planeCenter[1] = status ? yCenter : this->planeCenter[1];
	if(!status) communicatorPointer
		->buildVerboseMessage("Playable<B, T, D, A>::initializePlaneDisplayable() failed");
	return status;
}

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::initializeDigitDisplayble(T xCenter, T yCenter, const char digit) {
	bool status{true};
	short abcdefg{0};
	switch(digit) {
	case '0':
		abcdefg = 0b1111110;
		break;
	case '1':
		abcdefg = 0b0110000;
		break;
	case '2':
		abcdefg = 0b1101101;
		break;
	case '3':
		abcdefg = 0b1111001;
		break;
	case '4':
		abcdefg = 0b0110011;
		break;
	case '5':
		abcdefg = 0b1011011;
		break;
	case '6':
		abcdefg = 0b1011111;
		break;
	case '7':
		abcdefg = 0b1110000;
		break;
	case '8':
		abcdefg = 0b1111111;
		break;
	case '9':
		abcdefg = 0b1110011;
		break;
	case 'd':
		abcdefg = 0b1001111;
		break;
	case 'i':
		abcdefg = 0b1110110;
		break;
	case 'e':
		abcdefg = 0b0111101;
		break;
	}
	bool bitmask[7] = {
		(abcdefg & 0b1000000) > 0,
		(abcdefg & 0b0100000) > 0,
		(abcdefg & 0b0010000) > 0,
		(abcdefg & 0b0001000) > 0,
		(abcdefg & 0b0000100) > 0,
		(abcdefg & 0b0000010) > 0,
		(abcdefg & 0b0000001) > 0
	};
	T drawParameters[10][4] {
		//digitj坐标
		{
			//a
			bitmask[0] ? xCenter - DIGIT_WIDTH : DIGIT_PH_0,
			bitmask[0] ? xCenter - 1 : DIGIT_PH_H1,
			bitmask[0] ? yCenter + 2 * DIGIT_WIDTH + 1 : DIGIT_PH_2,
			bitmask[0] ? yCenter + 2 * DIGIT_WIDTH + 1 : DIGIT_PH_3
		},
		{
			//a
			bitmask[0] ? xCenter + 1 : DIGIT_PH_0,
			bitmask[0] ? xCenter + DIGIT_WIDTH : DIGIT_PH_H1,
			bitmask[0] ? yCenter + 2 * DIGIT_WIDTH + 1 : DIGIT_PH_2,
			bitmask[0] ? yCenter + 2 * DIGIT_WIDTH + 1 : DIGIT_PH_3
		},
		{
			//g
			bitmask[6] ? xCenter - DIGIT_WIDTH : DIGIT_PH_0,
			bitmask[6] ? xCenter - 1 : DIGIT_PH_H1,
			bitmask[6] ? yCenter : DIGIT_PH_2,
			bitmask[6] ? yCenter : DIGIT_PH_3
		},
		{
			//g
			bitmask[6] ? xCenter + 1 : DIGIT_PH_0,
			bitmask[6] ? xCenter + DIGIT_WIDTH : DIGIT_PH_H1,
			bitmask[6] ? yCenter : DIGIT_PH_2,
			bitmask[6] ? yCenter : DIGIT_PH_3
		}, {//d
			bitmask[3] ? xCenter - DIGIT_WIDTH : DIGIT_PH_0,
			bitmask[3] ? xCenter - 1 : DIGIT_PH_H1,
			bitmask[3] ? yCenter - 2 * DIGIT_WIDTH - 1 : DIGIT_PH_2,
			bitmask[3] ? yCenter - 2 * DIGIT_WIDTH - 1 : DIGIT_PH_3
		},
		{
			//d
			bitmask[3] ? xCenter + 1 : DIGIT_PH_0,
			bitmask[3] ? xCenter + DIGIT_WIDTH : DIGIT_PH_H1,
			bitmask[3] ? yCenter - 2 * DIGIT_WIDTH - 1 : DIGIT_PH_2,
			bitmask[3] ? yCenter - 2 * DIGIT_WIDTH - 1 : DIGIT_PH_3
		},
		{
			//f
			bitmask[5] ? xCenter - DIGIT_WIDTH - 1 : DIGIT_PH_0,
			bitmask[5] ? xCenter - DIGIT_WIDTH - 1 : DIGIT_PH_V1,
			bitmask[5] ? yCenter + 2 * DIGIT_WIDTH : DIGIT_PH_2,
			bitmask[5] ? yCenter + 1 : DIGIT_PH_3
		},
		{
			//b
			bitmask[1] ? xCenter + DIGIT_WIDTH + 1 : DIGIT_PH_0,
			bitmask[1] ? xCenter + DIGIT_WIDTH + 1 : DIGIT_PH_V1,
			bitmask[1] ? yCenter + 2 * DIGIT_WIDTH : DIGIT_PH_2,
			bitmask[1] ? yCenter + 1 : DIGIT_PH_3
		},
		{
			//e
			bitmask[4] ? xCenter - DIGIT_WIDTH - 1 : DIGIT_PH_0,
			bitmask[4] ? xCenter - DIGIT_WIDTH - 1 : DIGIT_PH_V1,
			bitmask[4] ? yCenter - 1 : DIGIT_PH_2,
			bitmask[4] ? yCenter - 2 * DIGIT_WIDTH : DIGIT_PH_3
		},
		{
			//c
			bitmask[2] ? xCenter + DIGIT_WIDTH + 1 : DIGIT_PH_0,
			bitmask[2] ? xCenter + DIGIT_WIDTH + 1 : DIGIT_PH_V1,
			bitmask[2] ? yCenter - 1 : DIGIT_PH_2,
			bitmask[2] ? yCenter - 2 * DIGIT_WIDTH : DIGIT_PH_3
		},

	};
	status &= this->digitDisplayable.initializePixelInformation(drawParameters);
	if(!status) communicatorPointer
		->buildVerboseMessage("Playable<B, T, D, A>::initializeDigitDisplayble() failed");
	return status;
}

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::setCommunicator(Communicator<B, T, D, A> *communicatorPointer) {
	if(communicatorPointer == nullptr) return false;
	this->communicatorPointer = communicatorPointer;
	return true;
}

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::flashAllDisplayable() {
	if(this->communicatorPointer == nullptr) return false;
	bool status{true};
	status &= communicatorPointer->platformSpecificClearScreen();
	if(planeDisplayable.visibility)
		for(struct Displayable<PLANE_ARRAY_SIZE, T>::Pixel& pixelIt : planeDisplayable.pixelArray) {
			status &= communicatorPointer->platformSpecificFlashPixelToScreen(pixelIt.x, pixelIt.y);
		}
	if(bReferenceDisplayable.visibility)
		for(struct Displayable<BR_ARRAY_SIZE, T>::Pixel& pixelIt : bReferenceDisplayable.pixelArray) {
			status &= communicatorPointer->platformSpecificFlashPixelToScreen(pixelIt.x, pixelIt.y);
		}
	if(cReferenceDisplayable.visibility)
		for(struct Displayable<CR_ARRAY_SIZE, T>::Pixel& pixelIt : cReferenceDisplayable.pixelArray) {
			status &= communicatorPointer->platformSpecificFlashPixelToScreen(pixelIt.x, pixelIt.y);
		}
	if(digitDisplayable.visibility)
		for(struct Displayable<DIGIT_ARRAY_SIZE, T>::Pixel& pixelIt : digitDisplayable.pixelArray) {
			status &= communicatorPointer->platformSpecificFlashPixelToScreen(pixelIt.x, pixelIt.y);
		}
	if(!status) communicatorPointer
		->buildVerboseMessage("Playable<B, T, D, A>::flashAllDisplayable() failed");
	return status;
}

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::processInputAndGenerateOutput() {
	bool status{true};
	switch(currentScopeMode) {
	case MODE_NOTSTARTED:
		status &= initializeCReferenceDisplayable(-50/*-MAXABS_COORDINATE + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH*/);
		status &= initializePlaneDisplayable(0, 50);
		cReferenceYCenter = -50/*-MAXABS_COORDINATE + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH*/;
		planeCenter[0] = 0;
		planeCenter[1] = 50/*MAXABS_COORDINATE - PLANE_WIDTH*/;
		communicatorPointer->digitalOutputRwr1LedBuffer
			= communicatorPointer->digitalOutputRwr2LedBuffer
			  = communicatorPointer->digitalOutputBuzzerBuffer
				= false;
		currentScopeMode = MODE_CSCOPE;//状态机下一状态切换
		break;
	case MODE_CSCOPE:
		planeDisplayable.visibility = true;
		cReferenceDisplayable.visibility = true;
		bReferenceDisplayable.visibility = false;
		digitDisplayable.visibility = false;
		if(communicatorPointer->digitalInputLaunchButtonBuffer) {//如果用户尝试锁定飞机
			if((planeCenter[0] <= 0 + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH)
					&& (planeCenter[0] >= 0 - CR_HEIGHT * LINE_WIDTH - LINE_WIDTH)
					&& (planeCenter[1] <= cReferenceYCenter + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH)
					&& (planeCenter[1] >= cReferenceYCenter - CR_HEIGHT * LINE_WIDTH - LINE_WIDTH)
			  ) { //飞机的位置在锁定范围内
				currentScopeMode = MODE_BSCOPE;//状态机下一状态切换
				status &= initializePlaneDisplayable(0, -1);
				planeCenter[0] = 0;
				planeCenter[1] = -1;
				status &= initializeBReferenceDisplayable(0, 0, 0);
				bReferenceDistance = bScopeTargetedShift[0] = bReferenceCenter[0]
									 = bScopeTargetedShift[1] = bReferenceCenter[1] = 0;
				bScopeBeingAttacked = STATE_NEVERATTACKED;
				bScopeAttackTimer = 0;
			} /*else { //飞机的位置不在在锁定范围内
				currentScopeMode = MODE_GAMEOVER;//状态机下一状态切换
				planeDisplayable.visibility = false;
				cReferenceDisplayable.visibility = false;
				bReferenceDisplayable.visibility = false;
			}*/
		}
		if(planeCenter[1] <= -MAXABS_COORDINATE) {//如果飞机太靠下
			currentScopeMode = MODE_GAMEOVER;//状态机下一状态切换
			score = difficultyLevel;
		}
		if(automoveTimer % (TIMER_PERIOD / PIPER_FREQ) == 0 && //如果用户尝试移动锁定器的垂直位置
				(communicatorPointer->digitalInputPiperUpButtonBuffer
				 || communicatorPointer->digitalInputPiperDownButtonBuffer) ) {
			status &= initializeCReferenceDisplayable(cReferenceYCenter//c参考器上下移动
					  + (cReferenceYCenter <= 0)
					  * static_cast<T>(communicatorPointer->digitalInputPiperUpButtonBuffer)
					  - (cReferenceYCenter >= (-MAXABS_COORDINATE + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH))
					  * static_cast<T>(communicatorPointer->digitalInputPiperDownButtonBuffer));
			cReferenceYCenter += status
								 ? ((cReferenceYCenter <= 0)
									* static_cast<T>(communicatorPointer->digitalInputPiperUpButtonBuffer)
									- (cReferenceYCenter >= (-MAXABS_COORDINATE + CR_HEIGHT * LINE_WIDTH + LINE_WIDTH))
									* static_cast<T>(communicatorPointer->digitalInputPiperDownButtonBuffer))
								 : 0;
		}
		if(automoveTimer % (TIMER_PERIOD / REFERENCE_FREQ) == 0 && //如果用户尝试左右移动飞机
				(communicatorPointer->analogInputStickXAxisBuffer - 2.5f != 0)) {
			T newAnalogInputStickXAxisBuffer = (communicatorPointer->analogInputStickXAxisBuffer - 2.5f);
			status &= initializePlaneDisplayable(planeCenter[0] - newAnalogInputStickXAxisBuffer, planeCenter[1]);
			planeCenter[0] -= status ? newAnalogInputStickXAxisBuffer : 0;
		}
		if(automoveTimer % (AUTOMOVE_FREQ / difficultyLevel) == 0) { //如果到电脑随机移动B坐标和飞机
			if(planeCenter[0] == cScopeTargetedShift) {
				cScopeTargetedShift = communicatorPointer
										 ->platformSpecificRandomGenerator(-MAXABS_COORDINATE, MAXABS_COORDINATE);
			}
			bool shouldAdd = planeCenter[0] < cScopeTargetedShift;
			T randomShiftAmount = communicatorPointer->platformSpecificRandomGenerator(0, 3) - (shouldAdd ? 0 : 2);
			status &= initializePlaneDisplayable(planeCenter[0] + randomShiftAmount, planeCenter[1]);
			planeCenter[0] += status ? randomShiftAmount : 0;
		}
		if(automoveTimer % (AUTOMOVE_FREQ / difficultyLevel) == 0) {//如果飞机到时间该往下移动一个单位
			status &= initializePlaneDisplayable( planeCenter[0], planeCenter[1] - difficultyLevel); //飞机上下移动
			planeCenter[1] -= status ? difficultyLevel : 0;
		}
		if(automoveTimer % (TIMER_PERIOD / BLINK_FREQ)  == 0) { //如果灯和蜂鸣器该改变状态了
			communicatorPointer->digitalOutputRwr1LedBuffer = !communicatorPointer->digitalOutputRwr1LedBuffer;
			communicatorPointer->digitalOutputBuzzerBuffer = !communicatorPointer->digitalOutputBuzzerBuffer;
		}
		break;
	case MODE_BSCOPE:
		planeDisplayable.visibility = true;
		cReferenceDisplayable.visibility = false;
		bReferenceDisplayable.visibility = true;
		if(communicatorPointer->digitalInputLaunchButtonBuffer) {//如果用户尝试锁定发射导弹
			if(bReferenceDistance > BR_WIDTH * LINE_WIDTH * 0.25
					&& bReferenceDistance < BR_WIDTH * LINE_WIDTH * 0.75) { //飞机的位置在锁定范围内
				++difficultyLevel;
				currentScopeMode = MODE_GAMEWIN;//状态机下一状态切换
			}
		}
		if(automoveTimer % (TIMER_PERIOD / REFERENCE_FREQ) == 0 && //如果用户尝试上下左右移动B坐标和飞机
				(communicatorPointer->analogInputStickXAxisBuffer - 2.5f != 0
				 || communicatorPointer->analogInputStickYAxisBuffer - 2.5f != 0)) { //用户尝试移动B坐标和飞机
			T newAnalogInputStickXYAxisBuffer[2] = {static_cast<int>(communicatorPointer->analogInputStickXAxisBuffer - 2.5f),
													static_cast<int>(communicatorPointer->analogInputStickYAxisBuffer - 2.5f)
												   };
			status &= initializeBReferenceDisplayable(bReferenceCenter[0] - newAnalogInputStickXYAxisBuffer[0]
					  , bReferenceCenter[1] + newAnalogInputStickXYAxisBuffer[1]
					  , bReferenceDistance);
			bReferenceCenter[0] -= status ? newAnalogInputStickXYAxisBuffer[0] : 0;
			bReferenceCenter[1] += status ? newAnalogInputStickXYAxisBuffer[1] : 0;
			status &= initializePlaneDisplayable(planeCenter[0] - newAnalogInputStickXYAxisBuffer[0]
												 , planeCenter[1] + newAnalogInputStickXYAxisBuffer[1]);
			planeCenter[0] -= status ? newAnalogInputStickXYAxisBuffer[0] : 0;
			planeCenter[1] += status ? newAnalogInputStickXYAxisBuffer[1] : 0;
			bScopeTargetedShift[0] -= status ? newAnalogInputStickXYAxisBuffer[0] : 0;
			bScopeTargetedShift[1] += status ? newAnalogInputStickXYAxisBuffer[1] : 0;
		}
		if(automoveTimer % (AUTOMOVE_FREQ / difficultyLevel) == 0) { //如果到电脑随机移动B坐标和飞机
			if(planeCenter[0] == bScopeTargetedShift[0]) {
				bScopeTargetedShift[0] = communicatorPointer
										 ->platformSpecificRandomGenerator(-MAXABS_COORDINATE, MAXABS_COORDINATE);
			}
			if(planeCenter[1] == bScopeTargetedShift[1]) {
				bScopeTargetedShift[1] = communicatorPointer
										 ->platformSpecificRandomGenerator(-MAXABS_COORDINATE, MAXABS_COORDINATE);
			}
			bool shouldAdd[2] = {planeCenter[0] < bScopeTargetedShift[0], planeCenter[1] < bScopeTargetedShift[1]};
			T randomShiftAmount[3] = {communicatorPointer->platformSpecificRandomGenerator(0, 3) - (shouldAdd[0] ? 0 : 2),
									  communicatorPointer->platformSpecificRandomGenerator(0, 3) - (shouldAdd[1] ? 0 : 2),
									  (communicatorPointer->platformSpecificRandomGenerator(0, 101) < DISTANCE_FREQ) ? 1 : 0
									 };
			status &= initializeBReferenceDisplayable(bReferenceCenter[0] + randomShiftAmount[0]
					  , bReferenceCenter[1] + randomShiftAmount[1]
					  , bReferenceDistance +  randomShiftAmount[2]);
			bReferenceCenter[0] += status ? randomShiftAmount[0] : 0;
			bReferenceCenter[1] += status ? randomShiftAmount[1] : 0;
			bReferenceDistance += status ? randomShiftAmount[2] : 0 ;
			status &= initializePlaneDisplayable(planeCenter[0] + randomShiftAmount[0], planeCenter[1] + randomShiftAmount[1]);
			planeCenter[0] += status ? randomShiftAmount[0] : 0;
			planeCenter[1] += status ? randomShiftAmount[1] : 0;
		}
		if(bReferenceDistance >= BR_WIDTH * LINE_WIDTH - LINE_WIDTH//如果B坐标和飞机已经在屏幕外了
				|| planeCenter[0] <= -MAXABS_COORDINATE
				|| planeCenter[0] >= MAXABS_COORDINATE
				|| planeCenter[1] <= -MAXABS_COORDINATE
				|| planeCenter[1] >= MAXABS_COORDINATE) {
			currentScopeMode = MODE_GAMEOVER;//状态机下一状态切换
			score = difficultyLevel;
		}
		switch(bScopeBeingAttacked) {
		case STATE_NEVERATTACKED:
			if(automoveTimer % (TIMER_PERIOD / BLINK_FREQ)  == 0) { //如果灯和蜂鸣器该改变状态了
				communicatorPointer->digitalOutputRwr1LedBuffer = !communicatorPointer->digitalOutputRwr1LedBuffer;
				communicatorPointer->digitalOutputBuzzerBuffer = !communicatorPointer->digitalOutputBuzzerBuffer;
				if(communicatorPointer->platformSpecificRandomGenerator(0, 101) < ATTACK_PROBABLITY) {
					bScopeBeingAttacked = STATE_BEINGATTACKED;
					communicatorPointer->digitalOutputRwr1LedBuffer
						= communicatorPointer->digitalOutputRwr2LedBuffer
						  = communicatorPointer->digitalOutputBuzzerBuffer
							= true;
				}
			}
			break;
		case STATE_BEINGATTACKED:
			if(communicatorPointer->digitalInputCounterMeasureBuffer) { //如果用户及时反制
				bScopeBeingAttacked = STATE_HAVEBEENATTACKED;
			}
			if(automoveTimer % (TIMER_PERIOD / BLINK_FREQ)  == 0) { //如果灯和蜂鸣器该改变状态了
				++bScopeAttackTimer;
				if(bScopeAttackTimer >= 10) {//如果用户没有及时反制
					currentScopeMode = MODE_GAMEOVER;//状态机下一状态切换
					score = difficultyLevel;
				}
			}
			break;
		case STATE_HAVEBEENATTACKED:
			break;
		}
		break;
	case MODE_GAMEOVER:
		difficultyLevel = 1;
		communicatorPointer->digitalOutputRwr1LedBuffer
			= communicatorPointer->digitalOutputRwr2LedBuffer
			  = communicatorPointer->digitalOutputBuzzerBuffer
				= false;
		planeDisplayable.visibility = false;
		cReferenceDisplayable.visibility = false;
		bReferenceDisplayable.visibility = false;
		digitDisplayable.visibility = true;
		switch(digitState) {
		case STATE_DIGIT1:
			initializeDigitDisplayble(-5 * DIGIT_WIDTH, 0, 'd');
			digitState = STATE_DIGIT2;
			break;
		case STATE_DIGIT2:
			initializeDigitDisplayble(0, 0, 'i');
			digitState = STATE_DIGIT3;
			break;
		case STATE_DIGIT3:
			initializeDigitDisplayble(5 * DIGIT_WIDTH, 0, 'e');
			digitState = STATE_DIGIT1;
			break;
		}
		if(automoveTimer == TIMER_PERIOD/3) {
			++digitDisplayTimer;
//			qDebug() << digitDisplayTimer;
		}
		if(digitDisplayTimer == DIGIT_DISPLAYTIME) {
			digitDisplayTimer = 0;
			currentScopeMode = MODE_SHOWSCORE;
		}
		break;
	case MODE_GAMEWIN:
		planeDisplayable.visibility = false;
		cReferenceDisplayable.visibility = false;
		bReferenceDisplayable.visibility = false;
		digitDisplayable.visibility = true;
		switch(digitState) {
		case STATE_DIGIT1:
			initializeDigitDisplayble(-5 * DIGIT_WIDTH, 0, difficultyLevel / 100 + '0');
			digitState = STATE_DIGIT2;
			break;
		case STATE_DIGIT2:
			initializeDigitDisplayble(0, 0, difficultyLevel / 10 + '0');
			digitState = STATE_DIGIT3;
			break;
		case STATE_DIGIT3:
			initializeDigitDisplayble(5 * DIGIT_WIDTH, 0, difficultyLevel / 1 + '0');
			digitState = STATE_DIGIT1;
			break;
		}
		if(automoveTimer == TIMER_PERIOD/3)
			++digitDisplayTimer;
		if(digitDisplayTimer == DIGIT_DISPLAYTIME) {
			digitDisplayTimer = 0;
			currentScopeMode = MODE_NOTSTARTED;
		}
		break;
	case MODE_SHOWSCORE:
		planeDisplayable.visibility = false;
		cReferenceDisplayable.visibility = false;
		bReferenceDisplayable.visibility = false;
		digitDisplayable.visibility = true;
		switch(digitState) {
		case STATE_DIGIT1:
			initializeDigitDisplayble(-5 * DIGIT_WIDTH, 0, score / 100 + '0');
			digitState = STATE_DIGIT2;
			break;
		case STATE_DIGIT2:
			initializeDigitDisplayble(0, 0, score / 10 + '0');
			digitState = STATE_DIGIT3;
			break;
		case STATE_DIGIT3:
			initializeDigitDisplayble(5 * DIGIT_WIDTH, 0, score / 1 + '0');
			digitState = STATE_DIGIT1;
			break;
		}
//		if(automoveTimer == TIMER_PERIOD/3)
//			++digitDisplayTimer;
//		if(digitDisplayTimer==DIGIT_DISPLAYTIME*4){
//			digitDisplayTimer=0;
//			currentScopeMode = MODE_NOTSTARTED;
//		}
		break;
	}
	automoveTimer = (automoveTimer == TIMER_PERIOD) ? 0 : automoveTimer + 1;
	return status;
}

template<unsigned char B, typename T, typename D, typename A>
inline bool Playable<B, T, D, A>::executeCycle() {
	if(this->communicatorPointer == nullptr) return false;
	bool status{true};
	status &= communicatorPointer->platformSpecificUpdatePinsToBuffer(); //获取输入
//	status &= this->flashAllDisplayable(); //更新显示
	status &= this->processInputAndGenerateOutput(); //游戏处理
//	status &= this->flashAllDisplayable(); //更新显示
	status &= communicatorPointer->platformSpecificUpdateBufferToPins(); //推送输出
	status &= this->flashAllDisplayable(); //更新显示
	return true;
}



#endif // GAMEPLAY_H

/**
 * 线宽和线高定义
 *
 * 【C镜下的坐标】
 *
 * =========================================== -
 * =========================================== → line width = 3
 * =========================================== -
 *	                     -
 *                       → cr height * line width = 1*3 = 3
 *                       -
 *                       C
 *
 *
 *
 * ===========================================
 * ===========================================
 * ===========================================
 * |--------↓----------|
 * cr width * line width = 7*3 = 21
 *
 * 【B镜下的坐标】
 *
 *        line width = 3
 *        |-↑-|
 *        ===== -                  ===== -
 *        ===== → line width = 3  ===== → br height * line width = 2*3 = 6
 *        ===== -                  ===== |
 *        =====                    ===== |
 *        =====                    ===== |
 *        =====                    ===== -
 *					     C
 * ===================== ===================== -
 * ===================== ===================== → line width = 3
 * ===================== ===================== -
 * |---------↓--------|
 * br width * line width = 2*3 = 6
 *
 *
 * 【飞机坐标】
 *    plane width * line width = 1*3 = 3
 *    -↑-
 *     =
 *     =
 *     =
 *  ===C=== → plane width = 3
 *     =
 *     =
 *     =
 *
 *
 *   === ===
 *  =       =
 *  =       =
 *  =       =
 *  =       =
 *  =       =
 *  =       =
 *   ===C=== -> digitWidth = 3
 *  =       =
 *  =       =
 *  =       =
 *  =       =
 *  =       =
 *  =       =
 *   === ===
 */
