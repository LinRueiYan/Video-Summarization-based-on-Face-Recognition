#include "CLM_core.h"
#include <fstream>
#include <opencv2/core/core.hpp>
#include "openCV2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <dlib/image_processing/frontal_face_detector.h>
#include <tbb/tbb.h>
#include <FaceAnalyser.h>
#include <GazeEstimation.h>
using namespace std;
using namespace cv;
class FaceAlignment{
public:
	FaceAlignment();
	~FaceAlignment();
	Mat Alignment(Mat img,int& flag);
private:
	CLMTracker::CLMParameters clm_parameters;
	cv::Mat img;
	void convert_to_grayscale(const Mat& in, Mat& out);
	bool visualise;
	bool cx_undefined;
	bool fx_undefined;
	float fx = 0, fy = 0, cx = 0, cy = 0;
	cv::CascadeClassifier classifier;
	CLMTracker::CLM clm_model;
	vector<Rect_<double> > bounding_boxes;
	dlib::frontal_face_detector face_detector_hog;
};