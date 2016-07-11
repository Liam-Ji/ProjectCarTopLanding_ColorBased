/*���������γ����ȥ������*/
/*��һ����������ͨ�򣬵ڶ��������Ǻ�ɫͨ����ֵ�������������ڻ�ȡͼ���С����ͼ��������ȥ����*/
# pragma warning (disable:4819)

#include "objectDetect.h"

void rnsr(vector<vector<Point>> &contours)
{
	Rect *r = new Rect[contours.size()];         // ������Ӿ�������
	double *ra = new double[contours.size()];	 // ������Ӿ��γ��������
	double *rb = new double[contours.size()];


	for (unsigned int i = 0; i < contours.size(); i++)
	{
		r[i] = boundingRect(Mat(contours[i]));          // ��ȡ��ǰ��ͨ����������Ӿ���;
		rb[i] = ra[i] = rate(r[i].width, r[i].height);	// ���㳤���
	}

	sort(ra, ra + contours.size());		// ����Ӿ��γ���Ȱ���С��������

	int k = (int)((double)contours.size() * threshold_value_rate);
	if ((k < 2) && (contours.size() > 2))  k = 2;  // ���մ��� <contours.size() �Ѿ��Ǵ��ڵ��� 2>
	if (k > 6)  k = 3;  // ��ౣ�� 3 ��Ǳ��Ŀ��

	double threshold_rate = (ra[contours.size() - k - 1] + ra[contours.size() - k])/2;	//������Ӿ��γ������ֵ
	
	if (threshold_rate > threshold_value_rate_maximum)  // ���������������������ȫ������
		threshold_rate = threshold_value_rate_maximum;

	// ����ͨ���������ֵ���ƺ����Ӿ���
	vector<vector<Point>>::const_iterator itContours = contours.begin();
	for (unsigned int i = 0; i < contours.size(), itContours != contours.end(); ++i)
	{
		if ((rb[i] > threshold_rate) && ((rb[i] > threshold_value_rate_minimum)))
		{
			++itContours;
		}	
		else
			itContours = contours.erase(itContours);   // ɾ����ǰ��ͨ������

	}
	
	delete[] r;
	delete[] ra;
	delete[] rb;

}