/*���ڶ�ͼ�����Ԥ����*/
/*��һ������Ϊ��ȡ��֡���ڶ������������ֱ�Ϊ����ͨ����ֵ�ָ���*/
# pragma warning (disable:4819)
#include "objectDetect.h"

void preprocess(Mat &img_scene, Mat &Seg_img_red, Mat &Seg_img_blue)
{
	Mat HSV;
	cv::cvtColor(img_scene, HSV, CV_BGR2HSV);        // ��RGB�ռ�ת��ΪHSV�ռ�

	vector<Mat> channels;
	split(img_scene, channels);     // ����ɫ��ͨ��, ��һ��3ͨ��ͼ��ת����3����ͨ��ͼ��
	Mat img_scene_BlueChannel = channels.at(0);   // ��ͨ��
	Mat img_scene_GreenChannel = channels.at(1);  // ��ͨ��
	Mat img_scene_RedChannel = channels.at(2);    // ��ͨ��

	//vector<Mat> channels_HSV;
	//split(HSV, channels_HSV);     // ����ɫ��ͨ��, ��һ��3ͨ��ͼ��ת����3����ͨ��ͼ��
	//Mat img_HSV_VChannel = channels_HSV.at(0);    //  Vͨ��
	//Mat img_HSV_SChannel = channels_HSV.at(1);    // Sͨ��
	//Mat img_HSV_HChannel = channels_HSV.at(2);    // Hͨ��


	/*ͼ������ת��Ϊ32λ�ĸ�����*/
	Mat red_float;
	Mat green_float;
	Mat blue_float;
	img_scene_RedChannel.convertTo(red_float, CV_32F);
	img_scene_GreenChannel.convertTo(green_float, CV_32F);
	img_scene_BlueChannel.convertTo(blue_float, CV_32F);


	/*���ɫͨ������������ͨ����Ӧλ������ֵ�ñ�ֵ*/
	Mat red_green;
	Mat red_blue;
	Mat blue_red;
	Mat blue_green;
	divide(red_float, green_float + 0.00001, red_green, 1, -1);  // ע��Ԥ��������Ϊ0
	divide(red_float, blue_float + 0.00001, red_blue, 1, -1);
	divide(blue_float, red_float + 0.00001, blue_red, 1, -1);
	divide(blue_float, green_float + 0.00001, blue_green, 1, -1);

	/*�Ժ������ʱ���������������ž�Ϊ����ʱ������ų�*/
	Mat red_green_new;
	Mat red_blue_new;
	Mat blue_red_new;
	Mat blue_green_new;
	threshold(red_green - 1, red_green_new, 0, 0, CV_THRESH_TOZERO);       //  ��ͼ��Ԫ��ֵ > 0 ʱ����ȡԭֵ������Ϊ0
	threshold(red_blue - 1, red_blue_new, 0, 0, CV_THRESH_TOZERO);         //  ��ͼ��Ԫ��ֵ > 0 ʱ����ȡԭֵ������Ϊ0
	threshold(blue_red - 1, blue_red_new, 0, 0, CV_THRESH_TOZERO);         //  ��ͼ��Ԫ��ֵ > 0 ʱ����ȡԭֵ������Ϊ0
	threshold(blue_green - 1, blue_green_new, 0, 0, CV_THRESH_TOZERO);     //  ��ͼ��Ԫ��ֵ > 0 ʱ����ȡԭֵ������Ϊ0


	/*����ֵ����ϲ�������ɫͨ������ɫͨ����ֵ�Ľ���ͺ�ɫͨ������ɫͨ����ֵ�Ľ����ˣ���ɫͨ����ͬ���Ĵ���*/
	Mat red_multi;
	Mat blue_multi;
	multiply(red_green_new, red_blue_new, red_multi);
	multiply(blue_red_new, blue_green_new, blue_multi);

	/*�Գ˻����ȡ��ֵ�����ɶ�ֵͼ��*/
	threshold(red_multi, Seg_img_red, threshold_value_red, 1, THRESH_BINARY);         // ��ɫͨ��������ֵ�ָ������ֵʱ��ȡ 1��
	threshold(blue_multi, Seg_img_blue, threshold_value_blue, 1, THRESH_BINARY);     // ��ɫͨ��������ֵ�ָС����ֵʱ��ȡ 1��
	


	///*������ʾͼ��쿴Ч��*/
	//imshow("img_scene_red_rate", Seg_img_red * 255);
	//imshow("img_scene_blue_rate", Seg_img_blue * 255);


	/////*��ɫͼ����ֵ�ָ��Ľ����������*/
	//Mat Overlap_img = img_scene.clone();
	//int nr = Overlap_img.rows;  // ����
	//int nc = Overlap_img.cols;  // ����

	//for (int j = 0; j<nr; j++) 
	//{
	//	for (int i = 0; i<nc; i++) 
	//	{
	//		if ((Seg_img_red.at<float>(j, i) == 1) && (Seg_img_blue.at<float>(j, i)) == 0)
	//		{
	//			Overlap_img.at<cv::Vec3b>(j, i)[0] = 0;   // ԭͼ�еĺ�ɫ�����ô����ʾ
	//			Overlap_img.at<cv::Vec3b>(j, i)[1] = 0;
	//			Overlap_img.at<cv::Vec3b>(j, i)[2] = 255;
	//		}
	//		else if (((Seg_img_red.at<float>(j, i) == 0) && (Seg_img_blue.at<float>(j, i)) == 1))
	//		{
	//			Overlap_img.at<cv::Vec3b>(j, i)[0] = 255;  // ԭͼ�е���ɫ�����ô�����ʾ
	//			Overlap_img.at<cv::Vec3b>(j, i)[1] = 0;
	//			Overlap_img.at<cv::Vec3b>(j, i)[2] = 0;
	//		}
	//		else if (((Seg_img_red.at<float>(j, i) == 1) && (Seg_img_blue.at<float>(j, i)) == 1))
	//		{
	//			Overlap_img.at<cv::Vec3b>(j, i)[0] = 0;   // ԭͼ�еĽ��沿���ô��̱�ʾ
	//			Overlap_img.at<cv::Vec3b>(j, i)[1] = 255;
	//			Overlap_img.at<cv::Vec3b>(j, i)[2] = 0;
	//		}
	//		else if (((Seg_img_red.at<float>(j, i) == 0) && (Seg_img_blue.at<float>(j, i)) == 0))
	//		{
	//			Overlap_img.at<cv::Vec3b>(j, i)[0] = 0;   // ԭͼ�еı��������ô��ڱ�ʾ
	//			Overlap_img.at<cv::Vec3b>(j, i)[1] = 0;
	//			Overlap_img.at<cv::Vec3b>(j, i)[2] = 0;
	//		}
	//	}               
	//}

	//imshow("Overlap_img", Overlap_img);




}