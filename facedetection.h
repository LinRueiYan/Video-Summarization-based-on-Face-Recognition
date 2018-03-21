#ifndef FACEDETECTION_H
#define FACEDETECTION_H
#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream>
#include "yolo_v2_class.hpp"	// imported functions from DLL
#include <opencv2/opencv.hpp>	
class FaceDetection
{
public:
	FaceDetection();
	~FaceDetection();
	std::vector<bbox_t> getinfo(cv::Mat frame);
private:
	Detector *detector;
	cv::Mat img;
};

#endif // FACEDETECTION_H
