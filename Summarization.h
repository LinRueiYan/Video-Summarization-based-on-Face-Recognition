#include <QDialog>
#include <QMouseEvent>
#include "display.h"
#include "openCV2/highgui/highgui.hpp"
namespace Ui {
	class Summarization;
}

class Summarization : public QDialog
{
	Q_OBJECT

public:
	explicit Summarization(QWidget *parent = 0);
	~Summarization();
	void setVideo(std::string video_name);
	std::string getTarget();
private slots:
	void on_start_clicked();
	void on_stop_clicked();
	void on_finish_clicked();
signals:
	void finish_signal();
private:
	Ui::Summarization *ui;
	display *display_thread;
	cv::Rect roi;
	std::string videoname;

};

