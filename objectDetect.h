#include <cv.h>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#define threshold_value_red 0.1				// ��ɫͨ����ֵ
#define threshold_value_blue 0.3			// ��ɫͨ����ֵ

#define dilate_size 3		                // ͼ������ɫ���ֵ�����ճ���ߴ�

#define threshold_value_sum 0.3				//��ͨ��߽������ܺ���ֵ��(���������ձ���ȥ����ȡֵ��Χ0-1��
#define threshold_value_rate 0.5			//��Ӿ��γ������ֵ��ȡֵ��Χ0-1��

#define threshold_value_sum_minimum 40		//��ͨ��߽������ܺ����ֵ(��̫С������ȥ��)
#define threshold_value_rate_minimum 0.3	 //��Ӿ��γ������ֵ���ֵ(������� <0.3 ̫�������ȥ��)
#define threshold_value_rate_maximum 0.6	 //��Ӿ��γ������ֵ���ֵ(����� >0.7 ������������ȫ������)

#define threshold_value_pixel_rate 0.03		//��ͨ���ں������ر�ֵ��ֵ


extern bool ficoo;
double rate(double, double);														//���������ı�ֵ(С�������ϴ����)
string intToString(int number);
void preprocess(Mat &img_scene, Mat &Seg_img_red, Mat &Seg_img_blue);				//ͼ��Ԥ����
void cnsr(vector<vector<Point>> &contours);											//��ͨ�򰴱�����Сȥ��
void cnsr_mini(vector<vector<Point>> &contours);                                     //��ͨ����С��ֵȥ��
void rnsr(vector<vector<Point>> &contours);						    //��Ӿ��ΰ�����Ȱ�������ȥ��
void rnsr_mini(vector<vector<Point>> &contours);					//��Ӿ��ΰ�����Ȱ���Сֵȥ��
void pxsnsr(vector<vector<Point>> &contours, Mat &Seg_img_red, Mat &Seg_img_blue);	//��Ȥ�����غͱ�ֵȥ��

void ImageEnhance(Mat &img_scene, Mat &new_image);     //  ��ԭʼͼ�������ǿ����

Mat  sign(Mat &img);    // ���ź���

Mat CombineMultiImages(const std::vector<cv::Mat>& Images,
	const int NumberOfRows,
	const int NumberOfCols,
	const int Distance,
	const int ImageWidth,
	const int ImageHeight);

void ExtraCoordinates(vector<vector<Point>> &contours, vector<Point2i> &coordinates);

