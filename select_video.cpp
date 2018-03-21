#include "select_video.h"
#include "ui_select_video.h"

select_video::select_video(QWidget *parent) :
QDialog(parent),
ui(new Ui::select_video)
{
	ui->setupUi(this);

//	display_thread = new display(ui->framelabel, ui->roilabel, ui->timeslider, &roi);
	connect(this, SIGNAL(finish_signal()), display_thread, SLOT(video_finish()));
}

select_video::~select_video()
{
	delete ui;
}
void select_video::on_start_clicked()
{/*
	display_thread->Player_signal = 1;
	if (!display_thread->isRunning()){
		display_thread->readfile(videoname);
		display_thread->start(QThread::NormalPriority);
	}
	*/
}
void select_video::on_stop_clicked()
{
	//display_thread->Player_signal = 32;
}
void select_video::on_finish_clicked()
{
//	emit finish_signal();
}