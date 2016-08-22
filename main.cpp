# pragma warning (disable:4819)
#include "objectDetect.h"

int main()
{
	VideoCapture capture( "MOV_0021.mp4" );  // 读取已经录制的视频"MOV_0021.mp4"

	// 检测是否正常打开:成功打开时，isOpened返回ture   
	if (!capture.isOpened())  cout << "fail to open!" << endl;

	Mat img_scene;
	capture >> img_scene;

	double FrameCount = capture.get(CV_CAP_PROP_FRAME_COUNT); // 获取整个视频总的帧数目
	double frame = 0;
	cout << "整个视频共" << FrameCount << "帧" << endl;

	Size s(1164, 660);
	VideoWriter writer = VideoWriter("Myvideo.avi", CV_FOURCC('X', 'V', 'I', 'D'), 25, s);  // 创建待保存的视频

	if (!writer.isOpened())  // 尝试创建视频文件的打开
	{
		cerr << "Can not creat video file.\n" << endl;
		return -1;
	}

	bool pause = false;  // 运行中的暂停标记
	
	vector<Point2i> coordinates;  // 视频中每一帧中的目标坐标

	ofstream ofile;
	ofile.open(".\\TestResult.txt");
	while(waitKey(1)!=27)   // 按ESC键后直接退出
	{
		++frame;
		cout << "Current frame:" << frame << endl;

		//if (frame >= 370)   waitKey();  // 方便调试

		if (!capture.read(img_scene))
		{
			cout << "读取视频失败" << endl;
			return -1;
		}

		if (img_scene.rows > 400)
		{
			//resize(img_scene,img_scene,cvSize(0,0),0.2,0.2);  //把图像调整到合适的大小
			resize(img_scene, img_scene, cvSize(380, 220), 0, 0);  //把图像调整到合适的大小

		}

		namedWindow("Original Image");
		imshow("Original Image", img_scene);  // 显示原始彩色图像

		vector<Mat> co_image;
		putText(img_scene, "The Original Image", Point(1, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(img_scene);



		//// 对原始图像进行增强处理  <对于图像增强处理的必要性>
		//Mat new_image = Mat::zeros(img_scene.size(), img_scene.type());
		//ImageEnhance(img_scene, new_image);   // 图像饱和度增强

		//putText(new_image, "The Original Image After Enhanced", Point(1, 15), 1, 1, Scalar(255, 255, 255), 1);
		//co_image.push_back(new_image);

		//img_scene = new_image;  // 用增强后的图像进行后续的处理操作




		Mat Seg_img_red;   // 红色通道阈值分割结果图
		Mat Seg_img_blue;  // 蓝色通道阈值分割结果图
		preprocess(img_scene, Seg_img_red, Seg_img_blue); // 通道分割来进一步获取目标的蓝色和红色部分

		// 准备待显示的Seg_img_blue
		Mat Seg_img_blue_disp;
		Seg_img_blue.convertTo(Seg_img_blue_disp, CV_8U);    // 转化为八位无符号整形
		cvtColor(Seg_img_blue_disp*255, Seg_img_blue_disp, CV_GRAY2BGR);
		putText(Seg_img_blue_disp, "The Seg_img_blue Image", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(Seg_img_blue_disp);

		// 准备待显示的Seg_img_red
		Mat Seg_img_red_disp;
		Seg_img_red.convertTo(Seg_img_red_disp, CV_8U);    // 转化为八位无符号整形
		cvtColor(Seg_img_red_disp*255, Seg_img_red_disp, CV_GRAY2BGR);
		Seg_img_red_disp.convertTo(Seg_img_red_disp, CV_8U);    // 转化为八位无符号整形
		putText(Seg_img_red_disp, "The Seg_img_red Image", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(Seg_img_red_disp);

//		Mat Object_img;
//		Mat Object_img_float;
//		bitwise_xor(Seg_img_red, Seg_img_blue, Object_img_float);   // 求两个图像的交集获取目标的潜在颜色区域
//		Object_img_float.convertTo(Object_img, CV_8U);

		Mat Seg_img_blue_cc;
		Seg_img_blue.convertTo(Seg_img_blue_cc, CV_8U);    // 转化为八位无符号整形

		dilate(Seg_img_blue_cc, Seg_img_blue_cc, Mat(), Point(-1, -1), dilate_size);  // 对图像膨胀进行断开部分粘连
		//erode(Seg_img_blue_cc, Seg_img_blue_cc, Mat(), Point(-1, -1), 2);   // 对图像中的噪声进行剔除

		vector<vector<Point> > contours;
		findContours(Seg_img_blue_cc, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);   // 找出连通域轮廓（检测外轮廓）
		
		// 画出图像中所有的连通域轮廓线
		Mat result(Seg_img_blue_cc.size(), CV_8U, Scalar(0));
		drawContours(result, contours,      //画出轮廓
			-1, // 画出所有的轮廓
			Scalar(255), // 用白线画出
			1); // 轮廓线的粗细为1


		putText(result,"contours", Point(3,15), 1, 1, Scalar(255), 1);
		
		cvtColor(result, result, CV_GRAY2BGR);
		co_image.push_back(result);

		namedWindow("Contours");
		imshow("Contours", result);  // 显示图像中所有的连通域轮廓
		
		// 利用连通域轮廓线的长度大小来消除连通域的噪声
		Mat original(Seg_img_blue_cc.size(), CV_8U, Scalar(0));
		if (contours.size() > 3)
		{
			cnsr(contours);			// 按比例消除小连通域的噪声
		}
		else
		{
			cnsr_mini(contours);	// 消除太小的小连通域的噪声
		}
			
		
		drawContours(original, contours,
			-1, // 画出新的所有的轮廓
			Scalar(255), // 用白线画出
			1); // 轮廓线的粗度为1
		namedWindow("Contours after noise reduced");
		imshow("Contours after noise reduced", original);				// 画出去掉连通域噪声后的连通域
		
		putText(original, "After Contours Noise Reduced", Point(3, 15), 1, 1, Scalar(255), 1);

		cvtColor(original, original, CV_GRAY2BGR);
		co_image.push_back(original);

		// 利用外接矩形的长宽比值大小进行去噪处理操作
		if (contours.size() > 2)
		{
			cout << "before rnsr:" << contours.size() << endl;
			rnsr(contours);			// 利用外接矩形长宽比按比例去噪
		}
		else
		{
			cout << "without rnsr process:" << contours.size() << endl;
			rnsr_mini(contours);		// 利用外接矩形长宽比按最小值去噪
		}
	
		// 绘出经过利用连通域外接矩形去噪后的目标图像
		Mat obj_rec_thr = Mat::zeros(Seg_img_red.size(), CV_8UC3);

		Rect *r = new Rect[contours.size()];         // 定义外接矩形数组
		for (unsigned int i = 0; i < contours.size(); i++)
		{
			r[i] = boundingRect(Mat(contours[i]));                // 获取当前连通域轮廓的外接矩形;
			rectangle(obj_rec_thr, r[i], Scalar(0, 0, 255), 1);   // 用红色矩形框把外接矩形表示出来
		}

		delete[] r;

		imshow("obj_rec_thr", obj_rec_thr);
		putText(obj_rec_thr, "After Rectange Ratio Noise Reduced", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(obj_rec_thr);

		// 利用同一个连通域内红蓝像素数量的比值大小进行去噪处理操作
		cout << "before pxsnsr:"<< contours.size() << endl;
		pxsnsr(contours, Seg_img_red, Seg_img_blue);			//兴趣区像素和比值去噪		

		// 绘出经过利用连通域红蓝像素数量的比值去噪后的目标图像
		Mat obj_px_thr = Mat::zeros(Seg_img_red.size(), CV_8UC3);

		Rect *rc = new Rect[contours.size()];         // 定义外接矩形数组
		for (unsigned int i = 0; i < contours.size(); i++)
		{
			rc[i] = boundingRect(Mat(contours[i]));          // 获取当前连通域轮廓的外接矩形;
			rectangle(obj_px_thr, rc[i], Scalar(10, 128, 255), 1);   // 用红色矩形框把外接矩形表示出来
		}

		delete[] rc;

		imshow("obj_px_thr", obj_px_thr);
		
		putText(obj_px_thr, "After Red-Blue Ratio Noise Reduced", Point(3, 15), 1, 1, Scalar(255, 255, 255), 1);
		co_image.push_back(obj_px_thr);
					
		// 坐标处理操作,利用连通域轮廓来获取目标中心点
		ExtraCoordinates(contours, coordinates);

		Mat img_scene_2 = img_scene.clone();   //clone赋值方式
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
		
		// 中间处理处理结果的合并显示
		Mat Image_com;
		Image_com = CombineMultiImages(co_image, 3, 3, 3, img_scene.cols, img_scene.rows);
		imshow("ProjectCarTopLanding_ColorBased", Image_com);
		resize(Image_com, Image_com, s);
		writer << Image_com;

		// 程序运行中时 暂停-继续-终端操作
		if (waitKey(1) == 112)  // 按下按键 p 时程序暂停 <112为p的ASCii码值>
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




