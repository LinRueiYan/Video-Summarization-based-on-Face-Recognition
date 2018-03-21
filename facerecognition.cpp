#include "facerecognition.h"
#include "layer_registed.h"
FaceRecognition::FaceRecognition(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	/* record video name */
	video_list = read_video_info();
	/* set ui thumbnail */
	set_Thumbnail();
	/* face recognition object */
	classifier = new Classifier();
	/* video retrieval object */
	retrieval = new Retrieval_Video(classifier);
	/* select target object */
	Target = new Target_select(classifier);
	/* video summarization object */
	summarization = new Summarization();

	QObject::connect(this, SIGNAL(drawBorder()), this, SLOT(drawBox()));
	QObject::connect(this, SIGNAL(clearBorder()), this, SLOT(clearBox()));
	connect(retrieval, SIGNAL(finish_rate()), this, SLOT(getProcess()));
	connect(retrieval, SIGNAL(processfinish()), this, SLOT(finish_retrieval()));
	connect(retrieval, SIGNAL(processfinish()), retrieval, SLOT(deleteLater()));
	connect(retrieval, SIGNAL(processfinish()), this, SLOT(re_start()));

}

FaceRecognition::~FaceRecognition()
{
}
void FaceRecognition::closeEvent(QCloseEvent *event){
	emit finish_signal();
}
void FaceRecognition::mousePressEvent(QMouseEvent *e){

}
/*************************************
* user select the video ( Red box )
* Left : select video 
* Right : clear video 
*************************************/
void FaceRecognition::mouseReleaseEvent(QMouseEvent *e){
	if (e->button() == Qt::LeftButton){
		emit drawBorder();
	}
	if (e->button() == Qt::RightButton){
		emit clearBorder();
	}
}
/*************************************
* read video information 
* setting/video_info.txt : record the videonames of user select 
* 
*************************************/
std::vector<std::string>  FaceRecognition::read_video_info(){
	std::ifstream in("Setting/video_info.txt");
	std::vector<std::string> result;
	std::string videoname;
	while (std::getline(in, videoname)){
		result.push_back(videoname);
	}
	in.close();
	return result;
}
/************************************
* set ui thumbnail
* set first frame of video to thumbnail
*************************************/
void FaceRecognition::set_Thumbnail(){
	cv::Mat frame;
	{
		/* initial video name of ui */
		frame = cv::Scalar(0, 0, 0);
		ui.label->setText(" ");
		ui.label_2->setText(" ");
		ui.label_3->setText(" ");
		ui.label_4->setText(" ");
		ui.label_5->setText(" ");
		ui.label_6->setText(" ");
		ui.label_7->setText(" ");
		ui.label_8->setText(" ");
		ui.label_9->setText(" ");
	}
	for (int i = 0;i < video_list.size(); i++){
		/* read video */
		cv::VideoCapture video = VideoCapture(video_list[i]);
		switch (i){
		case 0:
			if (!video.isOpened()){
				/* if the video is Null, set ui display RGB(0,0,0)*/
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				/* else set first frame of video to ui display */
				video >> frame;
			}
			/* resize frame size to ui display size */
			cv::resize(frame, frame, Size(ui.framelabel->size().width(), ui.framelabel->size().height()));
			/* set frame to ui display */
			ui.framelabel->setPixmap(
				/* Mat to QImage */
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			/* set video name of ui */
			ui.label->setText(QString::fromStdString(video_list[i]));
			break;
			
		case 1:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_2->size().width(), ui.framelabel_2->size().height()));
			ui.framelabel_2->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_2->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_2->setText(QString::fromStdString(video_list[i]));
			break;
			
		case 2:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_3->size().width(), ui.framelabel_3->size().height()));
			ui.framelabel_3->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_3->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_3->setText(QString::fromStdString(video_list[i]));
			break;
			
		case 3:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_4->size().width(), ui.framelabel_4->size().height()));
			ui.framelabel_4->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_4->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_4->setText(QString::fromStdString(video_list[i]));
			break;
		case 4:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_5->size().width(), ui.framelabel_5->size().height()));
			ui.framelabel_5->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_5->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_5->setText(QString::fromStdString(video_list[i]));
			break;

		case 5:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_6->size().width(), ui.framelabel_6->size().height()));
			ui.framelabel_6->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_6->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_6->setText(QString::fromStdString(video_list[i]));
			break;

		case 6:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_7->size().width(), ui.framelabel_7->size().height()));
			ui.framelabel_7->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_7->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_7->setText(QString::fromStdString(video_list[i]));
			break;

		case 7:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_8->size().width(), ui.framelabel_8->size().height()));
			ui.framelabel_8->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_8->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_8->setText(QString::fromStdString(video_list[i]));
			break;

		case 8:
			if (!video.isOpened()){
				frame = cv::Scalar(0, 0, 0);
			}
			else{
				video >> frame;
			}
			cv::resize(frame, frame, Size(ui.framelabel_9->size().width(), ui.framelabel_9->size().height()));
			ui.framelabel_9->setPixmap(
				QPixmap::fromImage(Mat2QImage(frame)).scaled(ui.framelabel_9->size(),
				Qt::KeepAspectRatio, Qt::FastTransformation));
			ui.label_9->setText(QString::fromStdString(video_list[i]));
			break;

		}

	}

}
/************************************
* select the video and show red box
*************************************/
void FaceRecognition::drawBox(){
	if (ui.framelabel->underMouse()){
		ui.background->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label->text().toStdString());
	}
	if (ui.framelabel_2->underMouse()){
		ui.background_2->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_2->text().toStdString());
	}
	if (ui.framelabel_3->underMouse()){
		ui.background_3->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_3->text().toStdString());
	}
	if (ui.framelabel_4->underMouse()){
		ui.background_4->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_4->text().toStdString());
	}
	if (ui.framelabel_5->underMouse()){
		ui.background_5->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_5->text().toStdString());
	}
	if (ui.framelabel_6->underMouse()){
		ui.background_6->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_6->text().toStdString());
	}
	if (ui.framelabel_7->underMouse()){
		ui.background_7->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_7->text().toStdString());
	}
	if (ui.framelabel_8->underMouse()){
		ui.background_8->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_8->text().toStdString());
	}
	if (ui.framelabel_9->underMouse()){
		ui.background_9->setStyleSheet("border: 5px solid red");
		target_video.push_back(ui.label_9->text().toStdString());
	}
}
/************************************
* clear the select
************************************/
void FaceRecognition::clearBox(){
	ui.background->setStyleSheet("");
	ui.background_2->setStyleSheet("");
	ui.background_3->setStyleSheet("");
	ui.background_4->setStyleSheet("");
	ui.background_5->setStyleSheet("");
	ui.background_6->setStyleSheet("");
	ui.background_7->setStyleSheet("");
	ui.background_8->setStyleSheet("");
	ui.background_9->setStyleSheet("");
	target_video.clear();
}
/************************************
* select target button
* if user select only one video, run select target object
* else show error message
*************************************/
void FaceRecognition::on_Target_clicked(){
	if (target_video.size() == 1){
		Target->setVideo(target_video[0]);
		Target->show();
	}
	else if (target_video.size() ==0){
		ui.Result->append("Please select video!!");
	}
	else{
		ui.Result->append("Please select only one video!!");
	}

}
/************************************
* select video button
* user only slect video format
*************************************/
void FaceRecognition::on_select_video_clicked(){
	QStringList filename = QFileDialog::getOpenFileNames(this,
		tr("Open Video"),
		"./",
		"ALL files (*.*);;Vdieo files (*.mp4;*.avi;*.wmv)");
	if (filename.size() == 0)
		return;
	/* write the video information on Setting/video_info.txt */
	std::ofstream List("Setting/video_info.txt");
	for (int i = 0; i < filename.size(); i++){
		List << filename[i].toStdString() <<std:: endl;
	}
	/* reset video information and ui thimbnail */
	video_list = read_video_info();
	set_Thumbnail();
}
/************************************
* dislpay the video summarization 
*************************************/
void FaceRecognition::on_display_video_clicked(){
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Video"),
		"./",
		"ALL files (*.*);;Vdieo files (*.mp4;*.avi;*.wmv)");
	summarization->setVideo(filename.toStdString());
	summarization->show();
}

/************************************
* retrieval video buttom 
* if user select target person, run retrieval video object 
* else show error message 
************************************/
void FaceRecognition::on_retrieval_video_clicked(){
	target_person = Target->getTarget();
	ui.Result->append("**************************");
	ui.Result->append("Target : ");
	ui.Result->append(QString::fromStdString(target_person));
	ui.Result->append("**************************");
	for (int i = 0; i < video_list.size(); i++){
		retrieval->readfile(video_list[i]);
	}
	if (target_person.empty()){
		ui.Result->append("Target is not exist!!");
		return;
	}
	retrieval->setResult(target_person);
	retrieval->show();
	ui.Result->append("--------------------------");

}
/************************************
* get the process of video retrieval 
************************************/
void  FaceRecognition::getProcess(){
	string temp;
	temp = retrieval->getfinish_rate();
	ui.Result->append(QString::fromStdString(temp));
}
/************************************
* if the video retrieval is finish,show finish message
************************************/
void  FaceRecognition::finish_retrieval(){
	string temp;
	ui.Result->append("Process finish!");
	ui.Result->append("--------------------------");
	retrieval->close();

}
/************************************
* Mat to QImage
************************************/
QImage FaceRecognition::Mat2QImage(Mat cvImg)
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
/************************************
* restart video retrieval 
************************************/
void FaceRecognition::re_start(){
	retrieval = new Retrieval_Video(classifier);
}