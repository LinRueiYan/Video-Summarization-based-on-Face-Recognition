#include "Target_select.h"
#include "ui_Target_select.h"

Target_select::Target_select(Classifier* c, QWidget *parent) : classification(c),
QDialog(parent),ui(new Ui::Target_select)
{
	ui->setupUi(this);
	/* display video object */
	display_thread = new display(ui->framelabel,ui->roilabel, ui->timeslider, &roi);
	connect(this, SIGNAL(finish_signal()), display_thread, SLOT(video_finish()));
}

Target_select::~Target_select()
{
	delete ui;
}
/************************************
* user draw the select box (press)
************************************/
void Target_select::mousePressEvent(QMouseEvent *e){

	if (e->button() == Qt::LeftButton){
		if (e->x() < ui->framelabel->x()
			|| e->y() < ui->framelabel->y()
			|| e->x() > ui->framelabel->x() + ui->framelabel->width()
			|| e->y() > ui->framelabel->y() + ui->framelabel->height()){

		}
		else{
			roi.x = e->x() - 10;
			roi.y = e->y() - 10;
			roi.width = 0;
			roi.height = 0;
		}
	}
}
/************************************
* user draw the select box (release)
************************************/
void Target_select::mouseReleaseEvent(QMouseEvent *e){
	if (e->button() == Qt::LeftButton){
		if (e->x() < ui->framelabel->x()
			|| e->y() < ui->framelabel->y()
			|| e->x() > ui->framelabel->x() + ui->framelabel->width()
			|| e->y() > ui->framelabel->y() + ui->framelabel->height()){

		}
		else{
			roi.width = e->x() - roi.x - 10;
			roi.height = e->y() - roi.y - 10;
		}
	}
}
/************************************
* start button
************************************/
void Target_select::on_start_clicked()
{
	/* start signal == 1 */
	display_thread->Player_signal = 1;
	if (!display_thread->isRunning()){
		/* set videoname to display thread */
		display_thread->readfile(videoname);
		display_thread->start(QThread::NormalPriority);
	}
}
/************************************
* stop button
************************************/
void Target_select::on_stop_clicked()
{
	display_thread->Player_signal = 32;
}
/************************************
* finish button
************************************/
void Target_select::on_finish_clicked()
{
	emit finish_signal();
}
/************************************
* read video name
************************************/
void Target_select::setVideo(std::string video_name){
	videoname = video_name;
}
/************************************
* face recognition 
* if use didn't select img, show error message
************************************/
void Target_select::on_Recognize_clicked(){
	cv::Mat img = display_thread->getROI();
	if (img.empty()){
		ui->Result->setPlainText("Face Image is not Exist!!");
		return;
	}
	/* face recognition result (ID <string> , confidence <float>) */
	std::vector<Prediction> predictions = classification->Classify(img);
	std::string result;
	for (size_t i = 0; i < predictions.size(); ++i) {
		Prediction p = predictions[i];
		ostringstream buff;
		if (p.second < 0.0001)
			p.second = 0.000;
		buff << p.second;

		result.append(p.first);
		result.append("   :  ");
		result.append(buff.str());
		result.append("\n");
	}
	/* show result in ui */
	ui->Result->setPlainText(QString::fromStdString(result));
	/* max confidence id is target */
	target = predictions[0].first;
	
}
/************************************
* return target
************************************/
std::string Target_select::getTarget(){
	return target;
}