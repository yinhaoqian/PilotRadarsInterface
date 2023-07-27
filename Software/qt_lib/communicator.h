#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H


/**
 * 接口——交流器
 * @brief The Communicator class
 * @tparam B 数字信号位宽
 * @tparam D 用于代表数字信号的数据类型
 * @tparam A 用于代表模拟信号的数据类型
 * @tparam T 存储像素坐标的基本数据类型
 */
template <unsigned char B, typename T, typename D, typename A>
class Communicator {
  private:
	///**
	// * 接口
	// * 利用平台特定的方式显示消息到屏幕
	// * @brief Communicator::platformSpecificPrint
	// * @param messageText
	// */
	virtual void platformSpecificPrint(char* messageText) = 0;

	///**
	// * 接口
	// * 利用平台特定的方式结束程序运行
	// * @brief Communicator::platformSpecificExit
	// */
	virtual void platformSpecificExit() = 0;


  public:

	/**
	 * 显示消息到屏幕
	 * @brief Communicator::verbose
	 * @param messageText 消息
	 */
	void buildVerboseMessage(char* messageText);

	/**
	 * 显示消息并且结束程序运行
	 * @brief Communicator::critical
	 * @param messageText 消息
	 */
	void buildCriticalMessage(char* messageText);

	/**
	 * 属性——刚刚从针脚读出来的数据缓冲区
	 * @brief Communicator::digitalInput[*]Buffer
	 */
	bool digitalInputLaunchButtonBuffer{true};
	bool digitalInputCounterMeasureBuffer{true};
	bool digitalInputPiperUpButtonBuffer{true};
	bool digitalInputPiperDownButtonBuffer{true};
	A analogInputStickXAxisBuffer{2.5};
	A analogInputStickYAxisBuffer{2.5};

	/**
	 * 属性——即将要发送到针脚的数字信号电平缓冲区
	 * @brief Communicator::digitalOutput[*]Buffer
	 */
	bool digitalOutputBuzzerBuffer{false};
	bool digitalOutputRwr1LedBuffer{false};
	bool digitalOutputRwr2LedBuffer{false};
	D analogOutputVectorGraphicsXAxisVoltageBuffer {0};
	D analogOutputStickYAxisSliderVoltageBuffer {0};

	/**
	 * 平台差异化接口
	 * 将缓冲器中的输出针脚电平更新到针脚上
	 * @brief platformSpecificUpdateBufferToPins
	 */
	virtual bool platformSpecificUpdateBufferToPins() = 0;

	/**
	 * 平台差异化接口
	 * 将针脚上的电平更新到缓冲器中
	 * @brief platformSpecificUpdateBufferToPins
	 */
	virtual bool platformSpecificUpdatePinsToBuffer() = 0;

	/**
	 * 平台差异化接口
	 * 将某个像素点亮
	 * @brief platformSpecificFlashPixelToScreen
	 */
	virtual bool platformSpecificFlashPixelToScreen(const T x, const T y) = 0;

	/**
	 * 平台差异化接口
	 * 将屏幕清空
	 * @brief platformSpecificClearScreen
	 */
	virtual bool platformSpecificClearScreen() = 0;

	/**
	 * 平台差异化接口
	 * 随机取一个区间数字
	 * @brief platformRandomGenerator
	 * @param lowerLimit 最低值
	 * @param upperLimit 最高值
	 * @param generatedNumber 结果保存的参考索引
	 */
	virtual T platformSpecificRandomGenerator(const T lowerLimit, const T upperLimit) = 0;

};

template <unsigned char B, typename T, typename D, typename A>
inline void Communicator<B, T, D, A>::buildVerboseMessage(char *messageText) {
	platformSpecificPrint(messageText);
}

template <unsigned char B, typename T, typename D, typename A>
inline void Communicator<B, T, D, A>::buildCriticalMessage(char *messageText) {
	this->buildVerboseMessage(messageText);
	platformSpecificExit();
}



#endif // COMMUNICATOR_H
