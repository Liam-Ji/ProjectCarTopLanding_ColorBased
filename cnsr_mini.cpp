/*����ȥ����ͨ��Ĳ����������������ֵ*/
# pragma warning (disable:4819)
#include "objectDetect.h"
void cnsr_mini(vector<vector<Point>> &contours)
{
	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < threshold_value_sum_minimum)
			itc = contours.erase(itc);   // ɾ����ǰ��ͨ������
		else
			++itc;
	}
}
