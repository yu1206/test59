#include <iostream>
#include <vector>
#include<stdlib.h>
#include <time.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "fstream"
#include <direct.h>
#include <io.h>
#include <Windows.h>
#include <direct.h> 
using namespace std;
using namespace cv;

extern int readDir(char *dirName, vector<string> &filesName);

struct MatTxt
{
	string name;
	Mat img;
	vector<Point>pts;
	vector<Point2f>pts2f;
};
int merge_my1(Mat src1,Mat src2,Mat out)
{
	for (int h = 0; h < out.rows;h++)
	{
	    uchar* outData = out.ptr<uchar>(h);
		const uchar* src2Data = src2.ptr<uchar>(h);
		

		for (int w = 0; w < out.cols;w++)
		{
			for (int c = 0; c < 3;c++)
			{
				int mask = src2Data[ /*h*out.cols + */w * 3 + c];
				if (mask!=0)
				{
					outData[/*h*out.cols +*/ w * 3 + c] = mask;
				}
			}
		}
	}

	int jjjj = 0;


	return 0;
}

int merge_my2(Mat src1, Mat src2, Mat out)
{

	for (int i = 0; i < out.rows; i++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			/*if (src2.at<Vec3b>(i, j)[0]==0)
			{
				continue;
			}*/

			if (src2.at<Vec3b>(i, j)[0] != 0)
			{
				out.at<Vec3b>(i, j)[0] = src2.at<Vec3b>(i, j)[0];
				out.at<Vec3b>(i, j)[1] = src2.at<Vec3b>(i, j)[1];
				out.at<Vec3b>(i, j)[2] = src2.at<Vec3b>(i, j)[2];
			}

			
		}
	}
	
	int jjjj = 0;


	return 0;
}

int test49(int argc, char *argv[])
{
	vector<MatTxt> mMP;
	string imgpath = "E:/new_污损模糊 - 副本/统计污损/2";
	vector<string> v_img_;
	readDir((char*)imgpath.c_str(), v_img_);
	for (int i = 0; i < v_img_.size(); i++)
	{
		string imagename = v_img_[i];
		int npos = imagename.find_last_of('/');
		int npos2 = imagename.find_last_of('.');

		string name1 = imagename.substr(npos + 1, npos2 - npos - 1);
		Mat src = imread(imagename.c_str());

		cout << imagename.c_str() << endl;

		if (src.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}
		int width = src.cols;
		int height = src.rows;
		string txtpath = imgpath + "/" + name1 + ".txt";
		fstream finRead;
		finRead.open(txtpath, ios::in);

		if (!finRead.is_open())
		{
			cout << "finRead 文件数据打开错误！" << endl;
			system("pause");
			return false;
		}

		string line; int num1 = 0;
		vector<Point> vP; vector<Point2f> vP2f;
		while (getline(finRead, line))
		{

			if (line.empty())
			{
				break;
			}

			num1++;

			float x, y;
			stringstream str_s(line);
			str_s >> x >> y;
			Point pt; pt.x = x*width; pt.y = y*height;
			vP.push_back(pt);
			Point2f pt2f;
			pt2f.x = x; pt2f.y = y;
			vP2f.push_back(pt2f);


		}
		finRead.close();

		MatTxt mt;
		mt.name = name1;
		mt.img = src;
		mt.pts = vP;
		mt.pts2f = vP2f;
		mMP.push_back(mt);
	}

	srand((unsigned)time(NULL));

	string imgpath1 = "E:/清晰车牌-wm/val-污损制作2";
	string savepath = "E:/清晰车牌-wm/val-污损2";

	int nmask = 1;//一个原图像，一个mask图像，对mask在原图像的位置随机几次
	vector<string> v1_img_;
	readDir((char*)imgpath1.c_str(), v1_img_);

	for (int i = 0; i < v1_img_.size(); i++)
	{
		
		string imagename = v1_img_[i];

		//string imagename = "E:/清晰车牌-wm/trian-污损制作2/100076-苏E566VJ.jpg";

		int npos = imagename.find_last_of('/');
		int npos2 = imagename.find_last_of('.');

		string name1 = imagename.substr(npos + 1, npos2 - npos - 1);
		Mat src1 = imread(imagename.c_str());

		cout << imagename.c_str() << endl;

		if (src1.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}


		

		int num = 0;
		//for (int j = 0; j < mMP.size(); j++)

		int j = rand() % mMP.size();
		{


			//j = 1;



			MatTxt mt = mMP[j];
			Mat stdres1 = mt.img;
			vector<Point> pts1 = mt.pts;
			vector<Point2f> pts2f = mt.pts2f;
			string namestd = mt.name;

			Rect boundRect1 = boundingRect(Mat(pts1));
			Mat roires1 = stdres1(boundRect1);
			int  imgH = src1.rows; int imgW = src1.cols;
			vector<Point> ptssrc;
			for (int v = 0; v < pts2f.size(); v++)
			{
				Point pt; pts2f[v];
				pt.x = pts2f[v].x *imgW;
				pt.y = pts2f[v].y *imgH;
				ptssrc.push_back(pt);
			}
			
			Rect boundRect = boundingRect(Mat(ptssrc));
			Mat roires;// = stdres(boundRect);
			resize(roires1, roires, Size(boundRect.width, boundRect.height));
			int stdH = roires.rows; int stdW = roires.cols;


			/*if (imgW <= stdW || imgH << stdH)
			{
				continue;
			}*/
			

			int x = 0; int y = 0;

			for (int k = 0; k < nmask; k++)
			{


				if (stdres1.data == NULL)
				{
					printf("std.data = NULL！\n");
					system("pause");
					continue;
				}

				num++;

				
				if (imgW - stdW - 1 == 0)
				{
					x = 0;
				}
				else
				{
					x = rand() % (imgW - stdW - 1);
				}

				if (imgH - stdH - 1 == 0)
				{
					y = 0;
				}
				else
				{
					y = rand() % (imgH - stdH - 1);
				}



				//x = 75; y = 0;
				printf("j=%d,x=%d,y=%d\n", j, x, y);

				Mat imgzero = src1.clone();
				imgzero.setTo(cv::Scalar(0, 0, 0));
				Mat roi = imgzero(Rect(x, y, stdW, stdH));
				roires.copyTo(roi);

				/*vector<Point> ptsroi;
				for (int v = 0; v < ptssrc.size(); v++)
				{
					Point pt = ptssrc[v];
					pt.x = pt.x - boundRect.x + x;
					pt.y = pt.y - boundRect.y + y;
					ptsroi.push_back(pt);
				}*/


				/*imshow("1", imgzero);
				imshow("2", src1);*/
				
				Mat res = src1.clone();
				//res.setTo(cv::Scalar(0, 0, 0));
				//Mat imgmask2 = Mat(res.size(), CV_8UC3);
				//imgmask2.setTo(cv::Scalar(255, 255, 255));

				//int npt[] = { ptsroi.size() };
				//const Point * ppt[1] = { &ptsroi[0] };
				//fillPoly(imgmask2, ppt, npt, 1, cv::Scalar(0, 0, 0));

				//imshow("3", imgmask2);
				//imshow("4", res);

				
				//bitwise_and(img, imgmask, img1);
				//bitwise_or(src1, imgmask, res, imgmask2);
				//  src1.copyTo(res, imgmask2);
				 // src1.copyTo(imgzero, imgmask2);
				//src1.setTo(imgmask, imgmask2);

				
				//imshow("4_4", src1);

				 // merge_my1(src1,imgzero,res);

				merge_my2(src1, imgzero, res);

				 // imshow("4_3", src1);

				 // imshow("4_2", imgzero);

				//  int jjjj224 = 0;

				//imshow("4_1", res);

				//int jjjj22= 0;

				


				//waitKey(0);

				

				char file[1024];

				sprintf(file, "%s/%s-%s-%d.jpg", savepath.c_str(), name1.c_str(), namestd.c_str(), num);
				imwrite(file, res);

				int jjjj = 0;

			}
		}

	}
	return 0;
}
int test493(int argc, char *argv[])
{
	// 1. 读取坐标和对应的txt

	vector<MatTxt> mMP;
	string imgpath = "E:/new_污损模糊 - 副本/统计污损/2";
	
	vector<string> v_img_;
	readDir((char*)imgpath.c_str(), v_img_);
	for (int i = 0; i < v_img_.size(); i++)
	{
		string imagename = v_img_[i];
		int npos = imagename.find_last_of('/');
		int npos2 = imagename.find_last_of('.');

		string name1 = imagename.substr(npos + 1, npos2 - npos - 1);
		Mat src = imread(imagename.c_str());

		cout << imagename.c_str() << endl;

		if (src.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}
		int width = src.cols;
		int height = src.rows;
		string txtpath = imgpath +"/"+ name1 + ".txt";
		fstream finRead;
		finRead.open(txtpath, ios::in);

		if (!finRead.is_open())
		{
			cout << "finRead 文件数据打开错误！" << endl;
			system("pause");
			return false;
		}

		string line; int num1 = 0;
		vector<Point> vP;
		while (getline(finRead, line))
		{

			if (line.empty())
			{
				break;
			}

			num1++;

			float x, y;
			stringstream str_s(line);
			str_s >> x>> y ;
			Point pt; pt.x = x*width; pt.y = y*height;
			vP.push_back(pt);



		}
		finRead.close();

		MatTxt mt;
		mt.name = name1;
		mt.img = src;
		mt.pts = vP;
		mMP.push_back(mt);
	}

	srand((unsigned)time(NULL));
	
	string imgpath1 = "E:/yu1-plate-data/4_正常车牌-制作污损";
	string savepath = "E:/yu1-plate-data/4_正常车牌-污损";

	int nmask = 1;//一个原图像，一个mask图像，对mask在原图像的位置随机几次

	vector<string> v1_img_;
	readDir((char*)imgpath1.c_str(), v1_img_);

	for (int i = 0; i<v1_img_.size(); i++)
	{
		//string imagename = v1_img_[i];

		string imagename = "E:/yu1-plate-data/4_正常车牌-制作污损/1-苏E8C93Q.jpg";

		int npos = imagename.find_last_of('/');
		int npos2 = imagename.find_last_of('.');

		string name1 = imagename.substr(npos + 1, npos2 - npos - 1);
		Mat src1 = imread(imagename.c_str());

		cout << imagename.c_str() << endl;

		if (src1.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}


		cout << imagename.c_str() << endl;

		 int num = 0;
		for (int j = 0; j< mMP.size();j++)
		{
			MatTxt mt = mMP[j];
			Mat stdmask = mt.img;
			vector<Point> pts =mt.pts;
			string namestd = mt.name;
			for (int k = 0; k< nmask; k++)
			{

				
				if (stdmask.data == NULL)
				{
					printf("std.data = NULL！\n");
					system("pause");
					continue;
				}

				num++;


				
				Rect boundRect = boundingRect(Mat(pts));
				Mat roimask = stdmask(boundRect);

				

				Mat imgmask = src1.clone();
				imgmask.setTo(cv::Scalar(0, 0, 0));

				int  imgH = src1.rows; int imgW = src1.cols;
				int stdH = roimask.rows; int stdW = roimask.cols;
				int x = 0; int y = 0;
				if (imgW - stdW - 1 == 0)
				{
					x = 0;
				}
				else
				{
					x = rand() % (imgW - stdW - 1);
				}

				if (imgH - stdH - 1 == 0)
				{
					y = 0;
				}
				else
				{
					y = rand() % (imgH - stdH - 1);
				}


				Mat roi = imgmask(Rect(x, y, stdW, stdH));
				roimask.copyTo(roi);

				vector<Point> ptsroi;
				for (int v = 0; v < pts.size(); v++)
				{
					Point pt = pts[v];
					pt.x = pt.x - boundRect.x+x;
					pt.y = pt.y - boundRect.y+y;
					ptsroi.push_back(pt);
				}


				imshow("1", imgmask);

				
				
				
				/*int  imgH = img.rows; int imgW = img.cols;
				int stdH = roi.rows; int stdW = roi.cols;

				if (imgH<stdH)
				{
					continue;
				}

				if (imgW<stdW)
				{
					continue;
				}
				
				
				


				int x = 0; int y = 0;
				if (imgW - stdW - 1 == 0)
				{
					x = 0;
				}
				else
				{
					x = rand() % (imgW - stdW - 1);
				}

				if (imgH - stdH - 1 == 0)
				{
					y = 0;
				}
				else
				{
					y = rand() % (imgH - stdH - 1);
				}

				vector<Point> ptsroi;
				for (int v = 0; v < pts.size(); v++)
				{
					Point pt = pts[v];
					pt.x = pt.x - boundRect.x+x;
					pt.y = pt.y - boundRect.y+y;
					ptsroi.push_back(pt);
				}
				
				int npt[] = { ptsroi.size() };
				const Point * ppt[1] = { &ptsroi[0] };
				

				

				imshow("1",img);
				waitKey(0);*/



			
				
				Mat res = src1.clone();
				res.setTo(cv::Scalar(0, 0, 0));

				Mat imgmask2 = Mat(res.size(),CV_8UC1);
				imgmask2.setTo(cv::Scalar(255, 255, 255));
				
				
				
				
				
				int npt[] = { ptsroi.size() };
				const Point * ppt[1] = { &ptsroi[0] };
				//fillPoly(img, ppt, npt, 1, cv::Scalar(0, 0, 0));
				fillPoly(imgmask2, ppt, npt, 1, cv::Scalar(0, 0, 0));

				imshow("2", imgmask2);
				imshow("3", src1);
				//bitwise_or(img, imgmask, img1);
				//bitwise_or(src1, imgmask, res, imgmask2);
				src1.copyTo(res, imgmask2);

				src1.setTo(imgmask,imgmask2);


				imshow("4_2", src1);
				imshow("4_1", res);
				waitKey(0);
				
				

				char file[1024];

				sprintf(file, "%s/%s-%s-%d.jpg", savepath.c_str(), name1.c_str(), namestd.c_str(), num);
				imwrite(file, res);

				int jjjj = 0;
			}
		}
		






	}




	return 0;
}




int test492(int argc, char* argv[])
{
	IplImage* src, *res, *roi;

	

	src = cvLoadImage("E:/yu1-plate-data/4_正常车牌-制作污损/1-苏E8C93Q.jpg", 1);

	cvShowImage("1", src);

	res = cvCreateImage(cvGetSize(src), 8, 3);
	roi = cvCreateImage(cvGetSize(src), 8, 1);

	/* prepare the 'ROI' image */
	cvZero(roi);

	/* Note that you can use any shape for the ROI */
	cvCircle(
		roi,
		cvPoint(10, 10),
		10,
		CV_RGB(255, 255, 255),
		-1, 8, 0
		);


	cvShowImage("2", roi);

	/* extract subimage */
	cvAnd(src, src, res, roi);

	cvShowImage("3", res);
	cvShowImage("3_1", roi);
	/*
	* do the main processing with subimage here.
	* in this example, we simply invert the subimage
	*/
	cvNot(res, res);

	cvShowImage("4", res);

	/* 'restore' subimage */
	IplImage* roi_C3 = cvCreateImage(cvGetSize(src), 8, 3);
	cvMerge(roi, roi, roi, NULL, roi_C3);

	cvShowImage("5", roi_C3);


	cvAnd(res, roi_C3, res, NULL);

	cvShowImage("6", res);

	/* merge subimage with original image */
	cvNot(roi, roi);

	cvShowImage("7", roi);
	cvShowImage("8", src);
	cvAdd(src, res, res, roi);

	
	cvShowImage("8_2", roi);
	cvShowImage("8_3", res);
	/* show result */
	//cvNamedWindow("1", 1);
	//cvNamedWindow("res", 1);
	/*cvShowImage("1", src);
	cvShowImage("res", res);*/

	cvWaitKey(0);

	/* be tidy */
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&res);
	cvReleaseImage(&roi);

	return 0;
}



int test491(int argc, char *argv[])
{
	//轮廓最小外接矩形的绘制
	Mat srcImg = imread("20180831103839.jpg");
	Mat dstImg = srcImg.clone();
	cvtColor(srcImg, srcImg, CV_BGR2GRAY);
	threshold(srcImg, srcImg, 100, 255, CV_THRESH_BINARY); //二值化
	imshow("threshold", srcImg);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarcy;
	findContours(srcImg, contours, hierarcy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	vector<Rect> boundRect(contours.size());  //定义外接矩形集合
	vector<RotatedRect> box(contours.size()); //定义最小外接矩形集合
	Point2f rect[4];
	for (int i = 0; i<contours.size(); i++)
	{
		box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
		boundRect[i] = boundingRect(Mat(contours[i]));
		circle(dstImg, Point(box[i].center.x, box[i].center.y), 5, Scalar(0, 255, 0), -1, 8);  //绘制最小外接矩形的中心点
		box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
		rectangle(dstImg, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height), Scalar(0, 255, 0), 2, 8);
		for (int j = 0; j<4; j++)
		{
			line(dstImg, rect[j], rect[(j + 1) % 4], Scalar(0, 0, 255), 2, 8);  //绘制最小外接矩形每条边
		}
	}
	imshow("dst", dstImg);
	waitKey(0);


	return 0;
}