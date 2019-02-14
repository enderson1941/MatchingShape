// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MATCHINGSHAPE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MATCHINGSHAPE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MATCHINGSHAPE_EXPORTS
#define MATCHINGSHAPE_API __declspec(dllexport)
#else
#define MATCHINGSHAPE_API __declspec(dllimport)
#endif

//opencv
#include "opencv2/opencv_modules.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/aruco/charuco.hpp"
#include "opencv2/aruco.hpp"
#include "algorithm"

//soft
#include "iostream"
#include "time.h"
#include "vector"
#include "string"
#include "stdio.h"
#include "io.h"
#include "iomanip"
#include "stdlib.h"
#include "fstream"
#include "conio.h"
#include "Shlwapi.h"
#include "ctime"
#include "map"

using namespace std;
using namespace cv;

// This class is exported from the MatchingShape.dll
class MATCHINGSHAPE_API CMatchingShape 
{
public:
	// TODO: add your methods here.
	int morph_size;
	int morph_iter1;
	int morph_iter2;
	int blur_iter;
	int pattern_index = 0;
	int threshold_v;
	int threshold_mode;

	double pattern_length;

	bool parameter_initialize(int morph_size_, 
		int morph_iter1_, int morph_iter2_, int blur_iter_, int threshold_, int threshold_mode_);
	Mat pattern_load(char* bar_code, int mode_ = 0);
	bool pattern_match(Mat& in_image, double threshold_val);

private: 
	Mat image_process(Mat in_image, int morph_size_,
		int morph_iter1_, int morph_iter2_, int blur_iter_, int threshold_, int threshold_mode_);
};

vector<Point> temp_contour;

extern MATCHINGSHAPE_API int nMatchingShape;

extern MATCHINGSHAPE_API int fnMatchingShape(void);

