#include "Summarization.h"
#include "ui_Summarization.h"

Summarization::Summarization(QWidget *parent) :
QDialog(parent), ui(new Ui::Summarization)
{
	ui->setupUi(this);
	display_thread = new display(ui->framelabel, ui->framelabel, ui->timeslider, &roi);
	connect(this, SIGNAL(finish_signal()), display_thread, SLOT(video_finish()));
}

Summarization::~Summarization()
{
	delete ui;
}
void Summarization::on_start_clicked(){
	display_thread->Player_signal = 1;
	if (!display_thread->isRunning()){
		display_thread->readfile(videoname);
		display_thread->start(QThread::NormalPriority);
	}
}
void Summarization::on_stop_clicked(){
	display_thread->Player_signal = 32;
}
void Summarization::on_finish_clicked(){
	emit finish_signal();
}
void Summarization::setVideo(std::string video_name){
	videoname = video_name;
}