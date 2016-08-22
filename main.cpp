# pragma warning (disable:4819)
#include "objectDetect.h"

int main()
{
	VideoCapture capture( "MOV_0021.mp4" );  // ��ȡ�Ѿ�¼�Ƶ���Ƶ"MOV_0021.mp4"

	// ����Ƿ�������:�ɹ���ʱ��isOpened����ture   
	if (!capture.isOpened())  cout << "fail to open!" << endl;

	Mat img_scene;
	capture >> img_scene;

	double FrameCount = capture.get(CV_CAP_PROP_FRAME_COUNT); // ��ȡ������Ƶ�ܵ�֡��Ŀ
	double frame = 0;
	cout << "������Ƶ��" << FrameCount << "֡" << endl;

	Size s(1164, 660);
	VideoWriter writer = VideoWriter("Myvideo.avi", CV_FOURCC('X', 'V', 'I', 'D'), 25, s);  // �������������Ƶ

	if (!writer.isOpened())  // ���Դ�����Ƶ�ļ��Ĵ�
	{
		cerr << "Can not creat video file.\n" << endl;
		return -1;
	}

	bool pause = false;  // �����е���ͣ���
	
	vector<Point2i> coordinates;  // ��Ƶ��ÿһ֡�е�Ŀ������

	ofstream ofile;
	ofile.open(".\\TestResult.txt");
	while(waitKey(1)!=27)   // ��ESC����ֱ���˳�
	{
		++frame;
		cout << "Current frame:" << frame << endl;

		//if (frame >= 370)   waitKey();  // �������

		if (!capture.read(img_scene))
		{
			cout << "��ȡ��Ƶʧ��" << endl;
			return -1;
		}

		if (img_scene.rows > 400)
		{
			//resize(img_scene,img_scene,cvSize(0,0),0.2,0.2);  //��ͼ����������ʵĴ�С
			resize(img_scene, img_scene, cvSize(380, 220), 0, 0);  //��ͼ����������ʵĴ�С

		}

		namedWindow("Original Image");
		imshow("Original Image", img_scene);  // ��ʾԭʼ��ɫͼ��

		vector<Mat> co_image;
		putText(img_scene, "The Original Image", Point(1, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(img_scene);



		//// ��ԭʼͼ�������ǿ����  <����ͼ����ǿ����ı�Ҫ��>
		//Mat new_image = Mat::zeros(img_scene.size(), img_scene.type());
		//ImageEnhance(img_scene, new_image);   // ͼ�񱥺Ͷ���ǿ

		//putText(new_image, "The Original Image After Enhanced", Point(1, 15), 1, 1, Scalar(255, 255, 255), 1);
		//co_image.push_back(new_image);

		//img_scene = new_image;  // ����ǿ���ͼ����к����Ĵ������




		Mat Seg_img_red;   // ��ɫͨ����ֵ�ָ���ͼ
		Mat Seg_img_blue;  // ��ɫͨ����ֵ�ָ���ͼ
		preprocess(img_scene, Seg_img_red, Seg_img_blue); // ͨ���ָ�����һ����ȡĿ�����ɫ�ͺ�ɫ����

		// ׼������ʾ��Seg_img_blue
		Mat Seg_img_blue_disp;
		Seg_img_blue.convertTo(Seg_img_blue_disp, CV_8U);    // ת��Ϊ��λ�޷�������
		cvtColor(Seg_img_blue_disp*255, Seg_img_blue_disp, CV_GRAY2BGR);
		putText(Seg_img_blue_disp, "The Seg_img_blue Image", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(Seg_img_blue_disp);

		// ׼������ʾ��Seg_img_red
		Mat Seg_img_red_disp;
		Seg_img_red.convertTo(Seg_img_red_disp, CV_8U);    // ת��Ϊ��λ�޷�������
		cvtColor(Seg_img_red_disp*255, Seg_img_red_disp, CV_GRAY2BGR);
		Seg_img_red_disp.convertTo(Seg_img_red_disp, CV_8U);    // ת��Ϊ��λ�޷�������
		putText(Seg_img_red_disp, "The Seg_img_red Image", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(Seg_img_red_disp);

//		Mat Object_img;
//		Mat Object_img_float;
//		bitwise_xor(Seg_img_red, Seg_img_blue, Object_img_float);   // ������ͼ��Ľ�����ȡĿ���Ǳ����ɫ����
//		Object_img_float.convertTo(Object_img, CV_8U);

		Mat Seg_img_blue_cc;
		Seg_img_blue.convertTo(Seg_img_blue_cc, CV_8U);    // ת��Ϊ��λ�޷�������

		dilate(Seg_img_blue_cc, Seg_img_blue_cc, Mat(), Point(-1, -1), dilate_size);  // ��ͼ�����ͽ��жϿ�����ճ��
		//erode(Seg_img_blue_cc, Seg_img_blue_cc, Mat(), Point(-1, -1), 2);   // ��ͼ���е����������޳�

		vector<vector<Point> > contours;
		findContours(Seg_img_blue_cc, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);   // �ҳ���ͨ�������������������
		
		// ����ͼ�������е���ͨ��������
		Mat result(Seg_img_blue_cc.size(), CV_8U, Scalar(0));
		drawContours(result, contours,      //��������
			-1, // �������е�����
			Scalar(255), // �ð��߻���
			1); // �����ߵĴ�ϸΪ1


		putText(result,"contours", Point(3,15), 1, 1, Scalar(255), 1);
		
		cvtColor(result, result, CV_GRAY2BGR);
		co_image.push_back(result);

		namedWindow("Contours");
		imshow("Contours", result);  // ��ʾͼ�������е���ͨ������
		
		// ������ͨ�������ߵĳ��ȴ�С��������ͨ�������
		Mat original(Seg_img_blue_cc.size(), CV_8U, Scalar(0));
		if (contours.size() > 3)
		{
			cnsr(contours);			// ����������С��ͨ�������
		}
		else
		{
			cnsr_mini(contours);	// ����̫С��С��ͨ�������
		}
			
		
		drawContours(original, contours,
			-1, // �����µ����е�����
			Scalar(255), // �ð��߻���
			1); // �����ߵĴֶ�Ϊ1
		namedWindow("Contours after noise reduced");
		imshow("Contours after noise reduced", original);				// ����ȥ����ͨ�����������ͨ��
		
		putText(original, "After Contours Noise Reduced", Point(3, 15), 1, 1, Scalar(255), 1);

		cvtColor(original, original, CV_GRAY2BGR);
		co_image.push_back(original);

		// ������Ӿ��εĳ����ֵ��С����ȥ�봦�����
		if (contours.size() > 2)
		{
			cout << "before rnsr:" << contours.size() << endl;
			rnsr(contours);			// ������Ӿ��γ���Ȱ�����ȥ��
		}
		else
		{
			cout << "without rnsr process:" << contours.size() << endl;
			rnsr_mini(contours);		// ������Ӿ��γ���Ȱ���Сֵȥ��
		}
	
		// �������������ͨ����Ӿ���ȥ����Ŀ��ͼ��
		Mat obj_rec_thr = Mat::zeros(Seg_img_red.size(), CV_8UC3);

		Rect *r = new Rect[contours.size()];         // ������Ӿ�������
		for (unsigned int i = 0; i < contours.size(); i++)
		{
			r[i] = boundingRect(Mat(contours[i]));                // ��ȡ��ǰ��ͨ����������Ӿ���;
			rectangle(obj_rec_thr, r[i], Scalar(0, 0, 255), 1);   // �ú�ɫ���ο����Ӿ��α�ʾ����
		}

		delete[] r;

		imshow("obj_rec_thr", obj_rec_thr);
		putText(obj_rec_thr, "After Rectange Ratio Noise Reduced", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(obj_rec_thr);

		// ����ͬһ����ͨ���ں������������ı�ֵ��С����ȥ�봦�����
		cout << "before pxsnsr:"<< contours.size() << endl;
		pxsnsr(contours, Seg_img_red, Seg_img_blue);			//��Ȥ�����غͱ�ֵȥ��		

		// �������������ͨ��������������ı�ֵȥ����Ŀ��ͼ��
		Mat obj_px_thr = Mat::zeros(Seg_img_red.size(), CV_8UC3);

		Rect *rc = new Rect[contours.size()];         // ������Ӿ�������
		for (unsigned int i = 0; i < contours.size(); i++)
		{
			rc[i] = boundingRect(Mat(contours[i]));          // ��ȡ��ǰ��ͨ����������Ӿ���;
			rectangle(obj_px_thr, rc[i], Scalar(10, 128, 255), 1);   // �ú�ɫ���ο����Ӿ��α�ʾ����
		}

		delete[] rc;

		imshow("obj_px_thr", obj_px_thr);
		
		putText(obj_px_thr, "After Red-Blue Ratio Noise Reduced", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(obj_px_thr);
					
		// ���괦�����,������ͨ����������ȡĿ�����ĵ�
		ExtraCoordinates(contours, coordinates);

		Mat img_scene_2 = img_scene.clone();   //clone��ֵ��ʽ
		if (coordinates.empty())
			ofile << endl;
		else
		{
			Point2i point = coordinates.back();
			ofile << point.x << "\t" << point.y << endl;
			circle(img_scene_2, Point(point.x, point.y), 2, Scalar(255, 0, 255), 2);
			putText(img_scene_2, intToString(point.x) + "," + intToString(point.y), Point(point.x + 5, point.y), 1, 1, Scalar(0, 255, 0), 2);
		}
		putText(img_scene_2, "Detection Result", Point(3, 33), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(img_scene_2);
		
		// �м䴦�������ĺϲ���ʾ
		Mat Image_com;
		Image_com = CombineMultiImages(co_image, 3, 3, 3, img_scene.cols, img_scene.rows);
		imshow("ProjectCarTopLanding_ColorBased", Image_com);
		resize(Image_com, Image_com, s);
		writer << Image_com;

		// ����������ʱ ��ͣ-����-�ն˲���
		if (waitKey(1) == 112)  // ���°��� p ʱ������ͣ <112Ϊp��ASCii��ֵ>
			pause = true;
		while (pause)
		{
			if (waitKey(1) == 112)
				pause = false;
			if (!pause)
				break;
			if (waitKey(1) == 27)
				return 0;
		}			
		cout << endl;		
	}
	return 0;
}




