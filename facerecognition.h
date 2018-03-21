#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <QtWidgets/QMainWindow>
#include "ui_facerecognition.h"
#include <qmessagebox.h>
#include <QMouseEvent>
#include <qstring.h>
#include <qfiledialog.h>
#include "openCV2/highgui/highgui.hpp"
#include "display.h"
#include "retrieval_video.h"
#include "Summarization.h"
#include "Target_select.h"
using namespace cv;

class FaceRecognition : public QMainWindow
{
	Q_OBJECT

public:
	FaceRecognition(QWidget *parent = 0);
	~FaceRecognition();
private slots:
	void on_Target_clicked();
	void on_select_video_clicked();
	void on_display_video_clicked();
	void on_retrieval_video_clicked();
	void closeEvent(QCloseEvent *event);
	void drawBox();
	void clearBox();
	void getProcess();
	void finish_retrieval();
signals:
	void finish_signal();
	void drawBorder();
	void clearBorder();
	void start_retrieval();
public slots:
	void re_start();
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
private:
	Ui::FaceRecognitionClass ui;
	std::vector<std::string> target_video;
	Target_select *Target;
	Summarization *summarization;
	Rect roi;
	display *display_thread;
	Retrieval_Video* retrieval;
	Classifier* classifier;
	string target_person;
	std::vector<std::string> read_video_info();
	void set_Thumbnail();
	QImage Mat2QImage(cv::Mat cvImg);
	std::vector<std::string> video_list;
};

#endif // FACERECOGNITION_H
