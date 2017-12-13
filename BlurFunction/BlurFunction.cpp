// test.cpp : 定義主控台應用程式的進入點。
//

#include <iostream>
#include <opencv2/opencv.hpp>  
#include <cmath>

using namespace std;
using namespace cv;

//-----------------------------------【全域變數聲明部分】--------------------------------------  
//            描述：全域變數聲明  
//-----------------------------------------------------------------------------------------------  
Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4;
int g_nMeanBlurValue = 10;  //均值濾波內核值  
int g_nGaussianBlurValue = 10;  //高斯濾波內核值  
int g_nMedianBlurValue = 10;  //中值濾波參數值  
int g_nBilateralFilterValue = 10;  //雙邊濾波參數值  


								   //-----------------------------------【全域函式宣告部分】--------------------------------------  
								   //            描述：全域函式宣告  
								   //-----------------------------------------------------------------------------------------------  
								   //軌跡條回呼函數  
static void on_MeanBlur(int, void *);           //均值塊濾波器  
static void on_GaussianBlur(int, void *);                    //高斯濾波器  
static void on_MedianBlur(int, void *);               //中值濾波器  
static void on_BilateralFilter(int, void*);                    //雙邊濾波器  

void LayerMix(InputArray _grayImage, InputArray _blurImage, OutputArray _mixImage);
//-----------------------------------【main( )函數】--------------------------------------------  
//            描述：控制台應用程式的入口函數，我們的程式從這裡開始  
//-----------------------------------------------------------------------------------------------  
int main()
{
	std::cout << "Please enter image path : ";
	string infile;
	std::cin >> infile;

	//載入原圖  
	g_srcImage = imread(infile, 0);
	if (!g_srcImage.data) { printf("Oh，no，讀取圖像錯誤！\n"); return 1; }

	//克隆原圖到四個Mat類型中  
	g_dstImage1 = g_srcImage.clone();
	g_dstImage2 = g_srcImage.clone();
	g_dstImage3 = g_srcImage.clone();
	g_dstImage4 = g_srcImage.clone();


	//顯示原圖  
	namedWindow("【<0>原圖窗口】", 1);
	imshow("【<0>原圖窗口】", g_srcImage);

	//=================【<2>均值濾波】==========================  
	//創建窗口  
	namedWindow("【<2>均值濾波】", 1);
	//創建軌跡條  
	createTrackbar("內核值：", "【<2>均值濾波】", &g_nMeanBlurValue, 100, on_MeanBlur);
	on_MeanBlur(g_nMeanBlurValue, 0);
	//======================================================  


	//=================【<3>高斯濾波】===========================  
	//創建窗口  
	namedWindow("【<3>高斯濾波】", 1);
	//創建軌跡條  
	createTrackbar("內核值：", "【<3>高斯濾波】", &g_nGaussianBlurValue, 100, on_GaussianBlur);
	on_GaussianBlur(g_nGaussianBlurValue, 0);
	//=======================================================  


	//=================【<4>中值濾波】===========================  
	//創建窗口  
	namedWindow("【<4>中值濾波】", 1);
	//創建軌跡條  
	createTrackbar("參數值：", "【<4>中值濾波】", &g_nMedianBlurValue, 100, on_MedianBlur);
	on_MedianBlur(g_nMedianBlurValue, 0);
	//=======================================================  


	//=================【<5>雙邊濾波】===========================  
	//創建窗口  
	namedWindow("【<5>雙邊濾波】", 1);
	//創建軌跡條  
	createTrackbar("參數值：", "【<5>雙邊濾波】", &g_nBilateralFilterValue, 100, on_BilateralFilter);
	on_BilateralFilter(g_nBilateralFilterValue, 0);
	//=======================================================  


	//輸出一些説明資訊  
	cout << "\t請調整捲軸觀察圖像效果" << endl;
	cout << "\t按下   q  鍵，程式退出" << endl;
	while (char(waitKey(1)) != 'q') {}

	return 0;
}

//-----------------------------【on_MeanBlur( )函數】------------------------------------  
//            描述：均值濾波操作的回呼函數  
//-----------------------------------------------------------------------------------------------  
static void on_MeanBlur(int, void *)
{
	blur(g_srcImage, g_dstImage1, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1), Point(-1, -1));
	//LayerMix(g_srcImage, g_dstImage1, g_dstImage1);
	imshow("【<2>均值濾波】", g_dstImage1);

}

//-----------------------------【on_GaussianBlur( )函數】------------------------------------  
//            描述：高斯濾波操作的回呼函數  
//-----------------------------------------------------------------------------------------------  
static void on_GaussianBlur(int, void *)
{
	GaussianBlur(g_srcImage, g_dstImage2, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);
	//LayerMix(g_srcImage, g_dstImage2, g_dstImage2);
	imshow("【<3>高斯濾波】", g_dstImage2);
}


//-----------------------------【on_MedianBlur( )函數】------------------------------------  
//            描述：中值濾波操作的回呼函數  
//-----------------------------------------------------------------------------------------------  
static void on_MedianBlur(int, void *)
{
	medianBlur(g_srcImage, g_dstImage3, g_nMedianBlurValue * 2 + 1);
	//LayerMix(g_srcImage, g_dstImage3, g_dstImage3);
	imshow("【<4>中值濾波】", g_dstImage3);
}


//-----------------------------【on_BilateralFilter( )函數】------------------------------------  
//            描述：雙邊濾波操作的回呼函數  
//-----------------------------------------------------------------------------------------------  
static void on_BilateralFilter(int, void *)
{
	bilateralFilter(g_srcImage, g_dstImage4, g_nBilateralFilterValue, g_nBilateralFilterValue * 2, g_nBilateralFilterValue / 2);
	//LayerMix(g_srcImage, g_dstImage4, g_dstImage4);
	imshow("【<5>雙邊濾波】", g_dstImage4);
}

void LayerMix(InputArray _grayImage, InputArray _blurImage, OutputArray _mixImage)
{
	Mat grayImage = _grayImage.getMat();
	CV_Assert(grayImage.type() == CV_8UC1);

	Mat blurImage = _blurImage.getMat();
	CV_Assert(blurImage.type() == CV_8UC1);

	_mixImage.create(grayImage.size(), CV_8UC1);
	Mat mixImage = _mixImage.getMat();

	double divide = 0;
	for (int i = 0; i < grayImage.rows; ++i)
	{
		for (int j = 0; j < grayImage.cols; ++j)
		{
			divide = (double)grayImage.at<uchar>(i, j) / (double)blurImage.at<uchar>(i, j) > 1 ? 255 : ((double)grayImage.at<uchar>(i, j) / (double)blurImage.at<uchar>(i, j)) * 255.0;
			mixImage.at<uchar>(i, j) = divide + (double)blurImage.at<uchar>(i, j) < 255.0 ? 0 : 255;
		}
	}
}


