/*���������γ����ȥ������*/
/*��һ����������ͨ�򣬵ڶ��������Ǻ�ɫͨ����ֵ�������������ڻ�ȡͼ���С����ͼ��������ȥ����*/
# pragma warning (disable:4819)

#include "objectDetect.h"

void rnsr_mini(vector<vector<Point>> &contours)
{
	Rect *r = new Rect[contours.size()];         // ������Ӿ�������
	double *ra = new double[contours.size()];	 // ������Ӿ��γ��������
	double *rb = new double[contours.size()];


	for (unsigned int i = 0; i < contours.size(); i++)
	{
		r[i] = boundingRect(Mat(contours[i]));          // ��ȡ��ǰ��ͨ����������Ӿ���;
		rb[i] = ra[i] = rate(r[i].width, r[i].height);	// ���㳤���
	}

	// ����ͨ���������ֵ���ƺ����Ӿ���
	vector<vector<Point>>::const_iterator itContours = contours.begin();
	for (unsigned int i = 0; i < contours.size(), itContours != contours.end(); ++i)
	{
		if (rb[i] > threshold_value_rate_minimum)
		{
			++itContours;
		}
		else
			itContours = contours.erase(itContours);   // ɾ����ǰ̫�����ͨ������

	}

	delete[] r;
	delete[] ra;
	delete[] rb;



}