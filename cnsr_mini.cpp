/*用于去除连通域的部分噪声利用最低阈值*/
# pragma warning (disable:4819)
#include "objectDetect.h"
void cnsr_mini(vector<vector<Point>> &contours)
{
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < threshold_value_sum_minimum)
			itc = contours.erase(itc);   // 删除当前连通域轮廓
		else
			++itc;
	}
}
