#ifndef PIXELENTITY_H
#define PIXELENTITY_H

/**
 * 可显类——可以在显示屏上显示的物体
 * @brief The Displayable class
 * @tparam P 像素数组的长度
 * @tparam T 存储像素坐标的基本数据类型
 */
template <unsigned int P, typename T>
class Displayable {
  public:
	struct Pixel {
		T x;//x 水平坐标；左负右正
		T y;//y 垂直坐标；下负上正
	};

	// 属性——代表物体的高电平像素坐标数组
	Pixel pixelArray[P];

	// 属性——物体是否可见(这只是一个用于判断的属性，具体是否可见需要在Display中实现)
	bool visibility{false};

	/**
	 * 获取——代表此物体的像素矩阵中元素的数量
	 * 注意！像素矩阵中元素的数量是一个固定值，运行过程中不应该被修改
	 * @brief Displayable::size
	 * @return 代表此物体的像素矩阵中元素的数量
	 */
	unsigned char size() {
		return P;
	}


	/**
	 * 在像素矩阵中更新一个带厚度的水平线段
	 * 注意！此线段是以左上角而不是左中心为基准的
	 * @brief initializePixelInformation
	 * @param xStart 水平方向起始坐标
	 * @param xEnd 水平方向终止坐标
	 * @param y 垂直坐标
	 * @param iterationCount 已使用的索引值总数
	 * @param lineWidth 线段厚度
	 */
	template<unsigned int G>
	bool initializePixelInformation(const T (&xYStartEndTupleArray)[G][4]);

};

template<unsigned int P, typename T>
template<unsigned int G>
inline bool Displayable<P, T>::initializePixelInformation(const T (&xYStartEndTupleArray)[G][4]) {
	unsigned short iterationCount{0};
	for(T tupleInd = 0; tupleInd < G; ++tupleInd) {
		if(xYStartEndTupleArray[tupleInd][0] > xYStartEndTupleArray[tupleInd][1] || xYStartEndTupleArray[tupleInd][2] < xYStartEndTupleArray[tupleInd][3])
			return false;
		for(T xIt = xYStartEndTupleArray[tupleInd][0]; xIt <= xYStartEndTupleArray[tupleInd][1]; ++xIt) {
			for(T yIt = xYStartEndTupleArray[tupleInd][2]; yIt >= xYStartEndTupleArray[tupleInd][3]; --yIt) {
				if(iterationCount > P)
					return false;
				this->pixelArray[iterationCount].x = xIt;
				this->pixelArray[iterationCount].y = yIt;
				++iterationCount;
			}
		}
	}
	return iterationCount == P;
}



#endif // PIXELENTITY_H
