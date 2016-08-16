/*������Ȥ�����������ܺϵı�ֵ��ȥ����*/
/*����Ĳ�������Ϊ����ͨ�򣬺�ɫͨ����ֵ�ָ�������ɫͨ����ֵ�ָ���*/
# pragma warning (disable:4819)

#include "objectDetect.h"

void pxsnsr(vector<vector<Point>> &contours, Mat &Seg_img_red, Mat &Seg_img_blue)
{
	Rect *r = new Rect[contours.size()];   // ������Ӿ�������
	Mat obj_rec_thr = Mat::zeros(Seg_img_red.size(), CV_8UC3);

	for (unsigned int i = 0; i < contours.size(); i++)
	{
		r[i] = boundingRect(Mat(contours[i]));// boundingRect��ȡ�����Ӿ���
	}


	/// �����������͹��
	Mat Seg_img_blue_hull(Seg_img_blue.size(), CV_8U, Scalar(0));
	vector<vector<Point> >hull(contours.size());
	for (unsigned int i = 0; i< contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
		fillConvexPoly(Seg_img_blue_hull, &hull[i][0], hull[i].size(), Scalar(1));
	}

	erode(Seg_img_blue_hull, Seg_img_blue_hull, Mat(), Point(-1, -1), dilate_size);   // ��ͼ�����֮ǰ�����ͻָ�


	Seg_img_blue_hull.convertTo(Seg_img_blue_hull, CV_32F);

	imshow("Seg_img_blue _hull", Seg_img_blue_hull);


	double Evaluation = 0;      // ��Ǳ��Ŀ��Ĵ��ֵ��ʼ��
	double Evaluation_max = 0;   // ��Ǳ��Ŀ��Ĵ�����ֵ��ʼ��
	int index_best = -1;         // ����������� <ע��˴���ʼֵӦΪ-1>
	int index = 0;               // �������

	cout << contours.size() << endl;

	vector<vector<Point>>::const_iterator itContours = contours.begin();
	//for (; itContours != contours.end(); ++itContours)
	while (itContours != contours.end())
	{
		Mat imageROI_red = Seg_img_red(cv::Rect(r[index].x, r[index].y, r[index].width, r[index].height));
		Mat imageROI_blue = Seg_img_blue_hull(cv::Rect(r[index].x, r[index].y, r[index].width, r[index].height));

		Mat imageROI_red_blue = imageROI_red + imageROI_blue;
		threshold(imageROI_red_blue, imageROI_red_blue, 1, 1, THRESH_BINARY);   // ������ֵ�ָ������ֵ1����2ʱ��ȡ 1��


		Scalar s1 = sum(imageROI_red_blue);
		Scalar s2 = sum(imageROI_blue);
		double sum_red = s1.val[0];    // ��ȡͼ����������ɫ�еĺ�ɫ��������
		double sum_blue = s2.val[0];    // ��ȡͼ�������ɫ��������

		double pixel_sum_rate = rate(sum_red, sum_blue);  // ����ͼ����к������صı���

		cout << "sum_red:" << sum_red << "\t" << "," << "sum_blue:" << sum_blue << "\t";
		cout << "pixel_sum_rate:" << pixel_sum_rate << endl;


	    // ���������ر�̫�͵���ͨ������ɾ��
		if ((pixel_sum_rate < threshold_value_pixel_rate) || (sum_red < 12))  // �統ǰ��������һ���ĺ�ɫ����
		{
			itContours = contours.erase(itContours);  // ɾ����ǰ���� <�ر�ע�⣺ɾ����ǰ������������Զ�ָ���һ��>
			index++;
			continue;
		}
		else     // �統ǰ������һ���ĺ�ɫ����
		{
			int aaa = contours.size();  // �������������ϵ�

			Evaluation = sum_red;       // Ŀ������������ֵ����
			if (Evaluation > Evaluation_max)
			{
				Evaluation_max = Evaluation;
				index_best++;

				cout << "index_best - " << index_best  << endl;
			}

			index++;
			itContours++;  // ����������һ������ <�˴�����Ҫ����ָ��������ת���һ������>
		}

	
	}



	int ttt = contours.size();    // �������������ϵ�


   // �����Ȼ�д���һ��Ǳ��Ŀ����ѡȡ���ŵ�һ��
	if (ttt > 1) 
	{
		int index = 0;
		vector<vector<Point>>::const_iterator itContours_2 = contours.begin();
		while(itContours_2 != contours.end())
		{
			if (index != index_best)
				itContours_2 = contours.erase(itContours_2);
			else
				++itContours_2; 

			index++;
		}

	}



	delete[] r;



}