/*����ȥ����ͨ��Ĳ����������������ֵ*/
# pragma warning (disable:4819)
#include "objectDetect.h"

void cnsr_mini(vector<vector<Point>> &contours)
{
	//int *ca = new int[contours.size()];	 // ������ͨ��߽糤����������

	//vector<vector<Point>>::const_iterator itContours = contours.begin();
	//for (unsigned int i = 0; i < contours.size(), itContours != contours.end(); ++i, ++itContours)
	//{
	//	ca[i] = itContours->size();     // ��ȡ��ͨ��߽糤����Ϣ
	//}
	//sort(ca, ca + contours.size());	    // ����ͨ��߽����صĶ�������

	//int s = (int)((double)contours.size() * threshold_value_sum);

	//if ((s < 2) && (contours.size() > 2))  s = 3;  // ���մ��� <contours.size() �Ѿ��Ǵ��� 3>
	//if (s > 6)  s = 6;  // ��ౣ�� 6 ��Ǳ��Ŀ��

	//unsigned int threshhold_con = (ca[contours.size() - s - 1] + ca[contours.size() - s]) / 2;

	vector<vector<Point>>::const_iterator itc = contours.begin();
	while (itc != contours.end())
	{
		if (itc->size() < threshold_value_sum_minimum)
			itc = contours.erase(itc);   // ɾ����ǰ��ͨ������
		else
			++itc;
	}

	//delete[] ca;
}
