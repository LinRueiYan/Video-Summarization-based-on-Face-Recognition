#include "retrieval_video.h"
#include "ui_retrieval_video.h"
Retrieval_Video::Retrieval_Video(Classifier* c, QWidget *parent) :classification(c),
QDialog(parent), ui(new Ui::retrieval_video)
{
	ui->setupUi(this);
}

Retrieval_Video::~Retrieval_Video()
{
	delete ui;
}
void Retrieval_Video::setResult(string r){
	target = r;
}
void Retrieval_Video::readfile(string filename){
	RecordLog("Log/Retrieval.txt", filename);
	video_list.push_back(filename);
}
void Retrieval_Video::on_start_clicked(){
	retrieval();
}
void Retrieval_Video::retrieval(){
	create_video();
	/******************* Record face detect result ***********************/
	detection_video = VideoWriter("detection_video.avi", CV_FOURCC('D', 'I', 'V', '3'),30, cv::Size(1920, 1080));
	/********************* Retrieval Video Num ***************************/
	for (int m = 0; m < video_list.size(); m++){
		setfinish_rate(m);
		/* Send single of retriveal progress */
		emit finish_rate();
		/* Read video */
		video = VideoCapture(video_list[m]);
		if (!video.isOpened()){
			RecordLog("Log/Retrieval.txt", "Open video failed!!!");
			continue;
		}
		fps = video.get(CV_CAP_PROP_FPS);
		pos = 0;
		while (true){
			video >> frame;
			if (!frame.empty()){
				/* Face detection */
				detector_info = detector.getinfo(frame);
				for (int i = 0; i < detector_info.size(); i++){
					/****************** Record Tracker ************************/
					vector<int>temp;
					if (!tracker[detector_info[i].track_id].empty())
						temp = tracker[detector_info[i].track_id];
					temp.push_back(pos);
					tracker[detector_info[i].track_id] = temp;
					/******* Bounding box roi 2 roi (fromat transform) ********/
					cv::Rect roi;
					roi.x = detector_info[i].x;
					roi.y = detector_info[i].y;
					roi.width = detector_info[i].w;
					roi.height = detector_info[i].h;
					//saveImg(frame(roi));
					
					roi = roi_resize(roi, frame);
					stringstream t;
					t << "ID :" << detector_info[i].track_id << "  ";
					t << detector_info[i].prob;
					/****** if confidence > 0.2, run face recognition *********/
					if (detector_info[i].prob > 0.2){
						int flag = 0;
						/* Face detection Yellow boxes */
						cv::rectangle(frame, roi, CV_RGB(255, 255, 0), 5);  
						putText(frame, t.str(), Point2f(roi.x, roi.y - 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(255, 255, 0)); // Face Recognize Yellow ID
						/*************** Face alignment ***************************/
						Mat alignframe = alignment.Alignment(frame(roi), flag);
						/*************** Face recognize ***************************/
						vector<Prediction> predictions = classification->Classify(alignframe);
						/**********************************************************/
						if (flag==1)
							savePredict_Img(alignframe, predictions);
						stringstream r;
						r << predictions[0].first << "  ";
						//r << predictions[0].second;
						putText(frame, r.str(), Point2f(roi.x, roi.y - 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(255, 255, 0)); // Face Recognize Yellow ID
						/*********** Face recognition confidence > 0.9 ************/
						if (target == predictions[0].first && predictions[0].second > 0.9){
							/* Face detection Rex boxes */
							cv::rectangle(frame, roi, CV_RGB(255, 0, 0), 5);  
							putText(frame, predictions[0].first, Point2f(roi.x, roi.y - 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(255, 0, 0)); // Face Recognize Red ID
							record_roi[pos] = roi;
						}
						else if (target == predictions[0].first && predictions[0].second < 0.9){
							/* Face detection Orabge boxes */
							cv::rectangle(frame, roi, CV_RGB(255, 120, 0), 5);   //Face Recognize Red ROI
							putText(frame, predictions[0].first, Point2f(roi.x, roi.y - 5), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(255, 120, 0)); // Face Recognize Orange ID
							record_roi[pos] = roi;
						}
						/*********** calculation weight **************************/
						vector<vector<Prediction> > temp_weight;
						if (!recognition_weight[detector_info[i].track_id].empty())
							temp_weight = recognition_weight[detector_info[i].track_id];
						temp_weight.push_back(predictions);
						recognition_weight[detector_info[i].track_id] = temp_weight;
					}
				}
				/******************* Record face detect result *******************/
				if (detector_info.size() > 0){
					detection_video.write(frame);
				}
				/******************** Show frame *******************************/
				imshow("frame", frame);
				cv::resize(frame, frame, Size(ui->framelabel->size().width(), ui->framelabel->size().height()));
				ui->framelabel->setPixmap(
					QPixmap::fromImage(Mat2QImage(frame)).scaled(ui->framelabel->size(),
					Qt::KeepAspectRatio, Qt::FastTransformation));
				waitKey(1);
				pos++;
				/**************** Up weight *******************/
				ThisID.clear();
				for (int i = 0; i < detector_info.size(); i++){
					ThisID.push_back(detector_info[i].track_id);
				}
				for (int i = 0; i < LastID.size(); i++){
					vector<int>::iterator iter = std::find(ThisID.begin(), ThisID.end(), LastID[i]);
					if (iter == ThisID.end())
						if (target == Calculation_weight(recognition_weight[LastID[i]], LastID[i])){
							vector<int>::iterator iter = std::find(record_trackerid.begin(), record_trackerid.end(), LastID[i]);
							if (iter == record_trackerid.end())
								record_trackerid.push_back(LastID[i]);
						}
				}
				LastID.clear();
				for (int i = 0; i < detector_info.size(); i++){
					LastID.push_back(detector_info[i].track_id);
				}
				/************************************************/
			}
			else
				break;
		}
		/********** Save summarization video *************/
		for (int i = 0; i < record_trackerid.size(); i++){
			vector<int>temp = tracker[record_trackerid[i]];
			for (int j = 0; j < temp.size(); j++){
				cv::Rect roi = record_roi[temp[j]];
				video.set(CV_CAP_PROP_POS_FRAMES, temp[j]);
				video >> frame;
				putText(frame, video_list[m], Point2f(20, 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(255, 255, 0)); // Face Recognize Yellow ID
				//saveImg(frame);
				saveTarget(frame, roi);
			}
		}
		/*************** Record info log *****************/
		ostringstream strs;
		strs << "Target number " << " :" << record_trackerid.size();
		RecordLog("Log/Retrieval.txt", strs.str());
		tracker.clear();
		recognition_weight.clear();
	}
	frame.release();
	video.release();
	cv::destroyAllWindows();
	/* Send finish single */
	emit processfinish();
	return;
}
/**************************************** 
* record log 
****************************************/
void Retrieval_Video::RecordLog(string logname, string context){
	fstream file;
	stringstream t;
	time_t now = time(0);
	tm *ltm = localtime(&now);
	file.open(logname.c_str(), ios::app);
	t << 1 + ltm->tm_mon << "/";
	t << ltm->tm_mday << " ";
	t << ltm->tm_hour << ":";
	t << ltm->tm_min << ":";
	t << ltm->tm_sec << " ";
	file << t.str() << context << std::endl;
	file.close();
}
/****************************************
* save video summarization
****************************************/
void Retrieval_Video::saveTarget(Mat img,cv::Rect roi){
	/********************   putText   **************************/
	rectangle(img, roi, CV_RGB(255, 0, 0),5);
	/********************   save Img  **************************/
	cv::resize(img, img, cv::Size(640, 480));
	summerization.write(img);
}
/****************************************
* save detection img
****************************************/
void Retrieval_Video::saveImg(Mat img){
	vector<int> compression_params;
	ostringstream strs;
	string temp;
	strs << pos;
	temp.append("Record_Img/");
	temp.append(strs.str());
	temp.append(".png");
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	imwrite(temp, img, compression_params);
	cv::imshow("faces", img);
	cv::waitKey(1);
}
/****************************************
* save recogmition img
****************************************/
void Retrieval_Video::savePredict_Img(Mat img, vector<Prediction> p){
	if (p[0].second > 0.9){
		vector<int> compression_params;
		ostringstream strs;
		string temp;
		strs << pos;
		temp.append("Record_Img/");
		temp.append(p[0].first);
		temp.append("/");
		temp.append(strs.str());
		temp.append(".png");
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		compression_params.push_back(9);
		imwrite(temp, img, compression_params);
	}
}
/******************************************
* avoid roi size > frame size
******************************************/
cv::Rect  Retrieval_Video::roi_resize(cv::Rect roi, cv::Mat frame){
	int h = frame.rows;
	int w = frame.cols;
	if (roi.x + roi.width > w)
		roi.width = w - roi.x;
	if (roi.y + roi.height > h)
		roi.height = h - roi.y;
	return roi;
}
/******************************************
* create summarization vido 
******************************************/
void Retrieval_Video::create_video(){
	string temp;
	temp.append(target);
	temp.append("_Summerization.avi");
	summerization = VideoWriter(temp, CV_FOURCC('D', 'I', 'V', '3'), 12, cv::Size(640, 480));
	if (!summerization.isOpened())
		return;
	return;
}
/******************************************
* progress of retrieval
******************************************/
std::string Retrieval_Video::getfinish_rate(){
	string temp;
	stringstream t;
	temp.append("Process : ");
	t << rate << " / " << video_list.size();
	temp.append(t.str());
	return temp;

}
/******************************************
* progress of retrieval
******************************************/
void Retrieval_Video::setfinish_rate(int rate){
	this->rate = rate;
}
/******************************************
* Calculation weight 
* 1. vote
* 2. weight
******************************************/
std::string Retrieval_Video::Calculation_weight(vector<vector<Prediction> > weight, int tracker_id){
	map<std::string, float> score;
	float max =0;
	string result;
	/*************** Calculation_vote *************************
	for (int i = 0; i < weight.size(); i++){
		vector<Prediction> temp = weight[i];
		for (int j = 0; j < temp.size(); j++){
			score[temp[j].first] += (5 - j);
		}
	}
	for (auto iter = score.cbegin(); iter != score.cend(); ++iter){
		if (iter->second > max){
			max = iter->second;
			if ((float)max >= 5){
				result = iter->first;

			}
		}
	}
	/*************** Calculation_weight *************************/
	for (int i = 0; i < weight.size(); i++){
		vector<Prediction> temp = weight[i];
		for (int j = 0; j < temp.size(); j++){
			score[temp[j].first] += temp[j].second;
		}
	}
	for (auto iter = score.cbegin(); iter != score.cend();++iter){
		if (iter->second > max){
			max = iter->second;
			//ostringstream strs;
			//strs << "Weight " << " :" << (float)max / weight.size();
			//RecordLog("Log/Retrieval.txt", strs.str());
			if ((float)max / weight.size() > 0.7)
				result = iter->first;
		}
	}
	/*************************************************************/
	return result;
}
/******************************************
* Mat 2 QImage
******************************************/
QImage Retrieval_Video::Mat2QImage(Mat cvImg)
{
	QImage qImg;
	if (cvImg.channels() == 3)                             //3 channels color image
	{
		cvtColor(cvImg, cvImg, CV_BGR2RGB);
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	else if (cvImg.channels() == 1)                    //grayscale image
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_Indexed8);
	}
	else
	{
		qImg = QImage((const unsigned char*)(cvImg.data),
			cvImg.cols, cvImg.rows,
			cvImg.cols*cvImg.channels(),
			QImage::Format_RGB888);
	}
	return qImg;
}