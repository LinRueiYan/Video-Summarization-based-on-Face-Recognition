#include "facedetection.h"


FaceDetection::FaceDetection()
{
	detector = new Detector("D:/YOLO/Data/yolo-face.cfg", "D:/YOLO/Data/yolo-face_final_1.weights");
}

FaceDetection::~FaceDetection()
{

}
std::vector<bbox_t> FaceDetection::getinfo(cv::Mat frame){
	/* face detection  >> detect(image, threshold) */
	std::vector<bbox_t> result_vec = detector->detect(frame, 0.15);
	/* update tracking number */
	result_vec = detector->tracking(result_vec);
	return result_vec;
}