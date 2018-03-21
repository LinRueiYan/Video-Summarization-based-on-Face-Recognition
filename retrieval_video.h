#ifndef RETRIEVAL_VIDEO_H
#define RETRIEVAL_VIDEO_H
#include <QDialog>
#include <ctime>
#include <fstream>
#include <openCV2/opencv.hpp>
#include <time.h>
#include <algorithm>
#include <ctime>
#include "openCV2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "classification.h"
#include "facedetection.h"
#include "FaceAlignment.h"
using namespace cv;
namespace Ui {
	class retrieval_video;
}
class Retrieval_Video : public QDialog
{
	Q_OBJECT
public:
	explicit Retrieval_Video(Classifier* c, QWidget *parent = 0);
	~Retrieval_Video();
	/******** function **********/
	void readfile(string filename);
	void setResult(string r);
	std::string getfinish_rate();
	void retrieval();
public slots:
	void on_start_clicked();
signals:
	void finish_rate();
	void processfinish();
private:
	Ui::retrieval_video *ui;
	VideoCapture video;
	FaceAlignment alignment;
	Mat frame;
	vector<bbox_t> detector_info;
	map<int, std::vector<int> >tracker; // tacker_ID,frame_No.
	map<int, vector<vector<Prediction> > >recognition_weight;
	vector<int> ThisID, LastID;//record this time id ,record last time id
	cv::Rect roi_resize(cv::Rect roi,cv::Mat frame);
	vector<int> record_trackerid;
	map<int,cv::Rect>record_roi;
	string target;
	int target_tracking_id=0;
	int fps,pos;
	void RecordLog(string logname, string context);
	Classifier* classification;
	FaceDetection detector;
	vector<string>video_list;
	void saveTarget(Mat img,cv::Rect roi);
	void saveImg(Mat img);
	void savePredict_Img(Mat img, vector<Prediction> p );
	void create_video();
	cv::VideoWriter outputVideo;
	cv::VideoWriter summerization, detection_video;
	void setfinish_rate(int rate);
	int rate;
	std::string Calculation_weight(vector<vector<Prediction> > weight, int tracker_id);
	clock_t s, e;
	QImage Mat2QImage(Mat cvImg);

};

#endif // RETRIEVAL_VIDEO_H
