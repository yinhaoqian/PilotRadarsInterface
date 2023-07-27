#ifndef TEST_H
#define TEST_H
#include "communicator.h"
#define UNO_TEMPALTE_PARAM 8,int,int,int
class UnoCommunicator: Communicator<UNO_TEMPALTE_PARAM> {


	// Communicator interface
private:
	void platformSpecificPrint(char *messageText) override final
	{
	}
	void platformSpecificExit() override final
	{
	}

public:
	void platformSpecificUpdateBufferToPins() override final
	{
	}
	void platformSpecificUpdatePinsToBuffer() override final
	{
	}
	void platformSpecificFlashPixelToScreen(const int x, const int y) override final
	{
	}
};





#endif // TEST_H

