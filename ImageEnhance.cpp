/*��ԭʼ����Ƶͼ�������ǿ*/

# pragma warning (disable:4819)

#include "objectDetect.h"

void IntensityEnhance(Mat &img_scene, Mat &new_image);   // �Ӻ����ĺ�������
void IntensityEnhance_fast(Mat &img_scene, Mat &new_image);   // �Ӻ����ĺ�������

void ImageEnhance(Mat &img_scene, Mat &new_image)
{
	Mat HSV;
	cv::cvtColor(img_scene, HSV, CV_BGR2HSV);    // ��RGB�ռ�ת��ΪHSV�ռ�
	vector<Mat> splitBGR(HSV.channels());        //�����洢��ͨ��ͼƬ������
	split(HSV, splitBGR);   //�ָ�ͨ�����洢��splitBGR��

	IntensityEnhance_fast(splitBGR[1], splitBGR[1]);        //�Ա��Ͷ�ͨ���ֱ������ǿ

	merge(splitBGR, new_image);  //�ϲ�ͨ��
	cv::cvtColor(new_image, new_image, CV_HSV2BGR);    // ��RGB�ռ�ת��ΪHSV�ռ�

	IntensityEnhance_fast(new_image, new_image);    // �������������ǿ


}




// �ڲ��Ӻ���
void IntensityEnhance(Mat &img_scene, Mat &new_image)
{
	double alpha = 1.23;
	double beta = 14;

	if (img_scene.channels() == 3)    // ������ͼ��Ϊ��ɫͼ��ʱ��
	{
		/// ִ������ new_image(i,j) = alpha*image(i,j) + beta
		for (int y = 0; y < img_scene.rows; y++)
		{
			for (int x = 0; x < img_scene.cols; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					new_image.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(img_scene.at<Vec3b>(y, x)[c]) + beta);
				}
			}
		}
	}

	else if (img_scene.channels() == 1)    // ������ͼ��Ϊ�Ҷ�ͼ��ʱ��
	{
		for (int y = 0; y < img_scene.rows; y++)
		{
			for (int x = 0; x < img_scene.cols; x++)
			{
				new_image.at<uchar>(y, x) = saturate_cast<uchar>(alpha*(img_scene.at<uchar>(y, x)) + beta);
			}
		}
	}

	
}


// �ڲ��Ӻ���
void IntensityEnhance_fast(Mat &img_scene, Mat &new_image)
{
	double alpha = 1.23;
	double beta = 14;

	new_image = alpha*img_scene + beta;


}