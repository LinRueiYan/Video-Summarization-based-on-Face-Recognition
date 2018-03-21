#include "FaceAlignment.h"
FaceAlignment::FaceAlignment(){
	clm_parameters.validate_detections = false;
	clm_model = CLMTracker::CLM(clm_parameters.model_location);
	classifier = cv::CascadeClassifier(clm_parameters.face_detector_location);
	/* face detector by hog */
	face_detector_hog = dlib::get_frontal_face_detector();
}
FaceAlignment::~FaceAlignment(){

}
cv::Mat FaceAlignment::Alignment(Mat img,int& flag){
	// Loading depth file if exists
	Mat_<float> depth_image;
	// Making sure the image is in uchar grayscale
	Mat_<uchar> grayscale_image;
	convert_to_grayscale(img, grayscale_image);
		
	// if no pose defined we just use a face detector
	if (bounding_boxes.empty())
	{

		// Detect faces in an image
		vector<Rect_<double> > face_detections;

		/* select face detector  */
		if (clm_parameters.curr_face_detector == CLMTracker::CLMParameters::HOG_SVM_DETECTOR)
		{
			vector<double> confidences;
			CLMTracker::DetectFacesHOG(face_detections, grayscale_image, face_detector_hog, confidences);
		}
		else
		{
			CLMTracker::DetectFaces(face_detections, grayscale_image, classifier);
		}

		// perform landmark detection for every face detected
		for (size_t face = 0; face < face_detections.size(); ++face)
		{
			// if there are multiple detections go through them
			bool success = CLMTracker::DetectLandmarksInImage(grayscale_image, depth_image, face_detections[face], clm_model, clm_parameters);
			/* if face alignment success,flag = 1 */
			if (success)
				flag = 1;
			img = CLMTracker::getAffineImg(img, clm_model);
		}
	}
	return img;
}
/***********************************
* convert img to grayscale
***********************************/
void FaceAlignment::convert_to_grayscale(const Mat& in, Mat& out)
{
	if (in.channels() == 3)
	{
		// Make sure it's in a correct format
		if (in.depth() != CV_8U)
		{
			if (in.depth() == CV_16U)
			{
				Mat tmp = in / 256;
				tmp.convertTo(tmp, CV_8U);
				cvtColor(tmp, out, CV_BGR2GRAY);
			}
		}
		else
		{
			cvtColor(in, out, CV_BGR2GRAY);
		}
	}
	else if (in.channels() == 4)
	{
		cvtColor(in, out, CV_BGRA2GRAY);
	}
	else
	{
		if (in.depth() == CV_16U)
		{
			Mat tmp = in / 256;
			out = tmp.clone();
		}
		else if (in.depth() != CV_8U)
		{
			in.convertTo(out, CV_8U);
		}
		else
		{
			out = in.clone();
		}
	}
}