/*���ź����������ж�ͼ������Ԫ�ط��ŵ�����*/

# pragma warning (disable:4819)
#include "objectDetect.h"

Mat sign(Mat &img)
{
	Mat img_result_1;
	Mat img_result_2;
	threshold(img, img_result_1, 0, 1, THRESH_BINARY);    // ͨ��������ֵ�ָ������ֵ0ʱ��ȡ 1������Ϊ0��
	threshold(img, img_result_2, 0, -1, THRESH_BINARY_INV);    // ͨ��������ֵ�ָ������ֵ0ʱ��ȡ 0������Ϊ-1��

	Mat img_result;
	img_result = img_result_1 + img_result_2;  // ע�⣺��ǰimg��ֵΪ0��Ԫ�ز���������ȡֵΪ-1

	return img_result;


}