// MatchingShape.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MatchingShape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
using namespace cv;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: code your application's behavior here.
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}

int fnMatchingShape()
{

	return 0;
}

bool CMatchingShape::parameter_initialize(int morph_size_, 
	int morph_iter1_, int morph_iter2_, int blur_iter_, int threshold_, int threshold_mode_)
{
	morph_size = morph_size_;
	morph_iter1= morph_iter1_;
	morph_iter2= morph_iter2_;
	blur_iter = blur_iter_;
	threshold_v = threshold_;
	threshold_mode = threshold_mode_;
	return true;
}

Mat CMatchingShape::image_process(Mat in_image, int morph_size_,
	int morph_iter1_, int morph_iter2_, int blur_iter_, int threshold_, int threshold_mode_)
{
	Mat result = in_image.clone();
	if (in_image.channels() > 1)
	{
		cvtColor(in_image.clone(), result, CV_BGR2GRAY);
	}
	threshold(result.clone(), result, threshold_v, 255, threshold_mode);
	Mat element = getStructuringElement(MorphShapes::MORPH_RECT,
		Size(morph_size, morph_size), Point(-1, -1));
	morphologyEx(result.clone(), result, MorphTypes::MORPH_OPEN,
		element, Point(-1, -1), morph_iter1);//MORPH_OPEN MORPH_CLOSE
	morphologyEx(result.clone(), result, MorphTypes::MORPH_CLOSE,
		element, Point(-1, -1), morph_iter2);//MORPH_OPEN MORPH_CLOSE
	medianBlur(result.clone(), result, blur_iter);
	return result;
}

Mat CMatchingShape::pattern_load(char* bar_code, int mode_)
{
	Mat result;
	vector<vector<Point>> in_contour;
	Mat original_img = imread(bar_code, CV_LOAD_IMAGE_UNCHANGED);
	if (!original_img.data)
	{
		result = Mat(480, 640, CV_8UC3, Scalar::all(0));
		return result;
	}
	//
	result = image_process(original_img, 
		morph_size, morph_iter1, morph_iter2, blur_iter, threshold_v, threshold_mode);
	//
	findContours(result, in_contour, CV_RETR_LIST,
		CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL CV_RETR_LIST
	if (mode_)
	{
		ostringstream buffer;
		for (int i = 0; i < in_contour.size(); i++)
		{
			Scalar color_ = Scalar(rand() % 255, rand() % 255, rand() % 255);
			cv::drawContours(original_img, in_contour, i, color_, 2, 8);
			buffer << i;
			string file_name = buffer.str();
			buffer.str("");
			putText(original_img, file_name, in_contour[i][0],
				HersheyFonts::FONT_HERSHEY_PLAIN, 1, color_, 1);
		}
		pattern_length = arcLength(in_contour[pattern_index], true);
		imwrite("pattern_.bmp", original_img);
	}
	temp_contour = in_contour[pattern_index];
	//
	return result;
}

bool CMatchingShape::pattern_match(Mat& in_image, double threshold_val)
{
	bool res = false;
	Mat original = in_image.clone();
	Mat temp = image_process(in_image,
		morph_size, morph_iter1, morph_iter2, blur_iter, threshold_v, threshold_mode);
	vector<double> threshold;
	vector<vector<Point>> out_contour;
	findContours(temp, out_contour, CV_RETR_LIST,
		CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL
	vector<vector<Point>>::iterator itc = out_contour.begin();
	while (itc != out_contour.end())
	{
		double g_dConLength = arcLength(*itc, true);
		if (g_dConLength <  pattern_length * 0.5 ||
			g_dConLength > pattern_length * 3)
		{
			itc = out_contour.erase(itc);
		}
		else
		{
			double threshold_ = cv::matchShapes(temp_contour, *itc,
				CV_CONTOURS_MATCH_I2, 0);
			if (threshold_ < threshold_val)
			{
				threshold.push_back(threshold_);
				Scalar color_ = Scalar(rand() % 255, rand() % 255, rand() % 255);
				cv::drawContours(original, out_contour, itc - out_contour.begin(), color_, 4, 8);
				ostringstream buffer;
				buffer << "id: " << itc - out_contour.begin();
				string file_name = buffer.str();
				buffer.str("");
				putText(original, file_name, out_contour[itc - out_contour.begin()][0],
				HersheyFonts::FONT_HERSHEY_PLAIN, 1, color_, 2);
			}
			++itc;
		}
	}
	if (threshold.size() > 0)
	{
		res = true;
		time_t nowtime;
		nowtime = time(NULL);
		char currentdate[100];
		strftime(currentdate, sizeof(currentdate),
			"%H%M%S", localtime(&nowtime));
		ostringstream buffer;
		buffer << currentdate << "_result.bmp";
		string file_name = buffer.str();
		buffer.str("");
		imwrite(file_name, original);
	}
	return res;
}