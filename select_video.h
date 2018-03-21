#ifndef SELECT_VIDEO_H
#define SELECT_VIDEO_H

#include <QDialog>
#include "display.h"
namespace Ui {
	class select_video;
}

class select_video : public QDialog
{
	Q_OBJECT

public:
	explicit select_video(QWidget *parent = 0);
	~select_video();
private slots:
	void on_start_clicked();
	void on_stop_clicked();
	void on_finish_clicked();
signals:
	void finish_signal();
private:
	Ui::select_video *ui;
	display *display_thread;
};

#endif // SELECT_VIDEO_H
