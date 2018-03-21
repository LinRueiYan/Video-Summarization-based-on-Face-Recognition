#ifndef DISPLAY_H
#define DISPLAY_H
#include <QThread>
#include <QObject>
#include <QLabel>
#include <QImage>
#include <QSlider>
#include <openCV2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace cv;
class display :public QThread
{
    Q_OBJECT
public:
	display(QLabel *fs, QLabel *rs, QSlider *sl, Rect *roi);
    ~display();
    int Player_signal=0;
    void readfile(string filename);
	Mat getROI();
protected:
    void run();
public slots:
	void changeslider(int pos);
	void video_finish();
private :
	QLabel *frameShow, *roiShow;
    QSlider* slider;
    Rect* roi;
    int fps,video_length,pos;
    Size video_size;
    VideoCapture video;
    bool isfinish=false;
    string filename;
    Mat frame;
    std::ofstream log;
    clock_t s,e;
    char c;
	Mat roiframe;
    /****** function **********/
    QImage Mat2QImage(Mat cvImg);
    void create_log();
    void record_log(string context);
    void setSlider();
    Rect setROI();
    void showROI(Rect r);
};

#endif // DISPLAY_H
