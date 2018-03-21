#include <QDialog>
#include <QMouseEvent>
#include "display.h"
#include "retrieval_video.h"
#include "openCV2/highgui/highgui.hpp"
namespace Ui {
	class Target_select;
}

class Target_select : public QDialog
{
	Q_OBJECT

public:
	explicit Target_select(Classifier* c, QWidget *parent = 0);
	~Target_select();
	void setVideo(std::string video_name);
	std::string getTarget();
private slots:
	void on_start_clicked();
	void on_stop_clicked();
	void on_finish_clicked();
	void on_Recognize_clicked();
signals:
	void finish_signal();
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
private:
	Ui::Target_select *ui;
	display *display_thread;
	cv::Rect roi;
	std::string videoname;
	Classifier* classification;
	std::string target;

};

