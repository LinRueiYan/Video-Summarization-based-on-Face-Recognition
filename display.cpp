#include "display.h"

display::display(QLabel *fs, QLabel *rs,QSlider *sl, Rect *roi)
	:frameShow(fs), roiShow(rs), slider(sl), roi(roi)
{
    this->roi = roi;
}

display::~display()
{
    log.close();
}
/***************************************
* read videoname and video information
***************************************/
void display::readfile(string filename){
    this->filename = filename;
    video = VideoCapture(filename);
    fps = video.get(CV_CAP_PROP_FPS);
    video_length = video.get(CV_CAP_PROP_FRAME_COUNT);
    video_size = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH)
                          ,(int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
    setSlider();
    Player_signal = 32;
    pos = 0;
    isfinish = false;
}
/***************************************
* display thread start
***************************************/
void display::run(){
	/* check video is true */
    if(!video.isOpened()){
        record_log("Read Video failed!!");
        return ;
    }
	/* pos : the frame number of video */
    while(pos < video_length && isfinish == false){
        /*********************************************/
        pos++;
        video >> frame;
        s = clock();
        if(frame.empty()){
            record_log("Frame is error!!");
            continue;
        }
        /*************** Start Event *****************/
        if(Player_signal == 1){
            slider->setValue(pos);
        }
        /**************** Stop Event *****************/
        else if(Player_signal == 32){
            pos = slider->value();
			video.set(CV_CAP_PROP_POS_FRAMES, pos);
			slider->setValue(pos);
        }
        /****************     ROI    *****************/
        Rect r = setROI();
        showROI(r);
        rectangle(frame,r,CV_RGB(255,0,0));
        /**************** Show Event *****************/
        resize(frame,frame,Size(frameShow->size().width(),frameShow->size().height()));
        frameShow->setPixmap(
                    QPixmap::fromImage(Mat2QImage(frame)).scaled(frameShow->size(),
                    Qt::KeepAspectRatio, Qt::FastTransformation));
        e=clock();
		waitKey(1);
    }
	/* video is finished and show background RGB(0,0,0) */
	Mat blackframe(frame.rows, frame.cols, CV_8UC3);
	blackframe = Scalar(0, 0, 0);
	frameShow->setPixmap(
		QPixmap::fromImage(Mat2QImage(blackframe)).scaled(frameShow->size(),
		Qt::KeepAspectRatio, Qt::FastTransformation));
	slider->setValue(0);
	return;
}
/***************************************
* Mat to QImage
***************************************/
QImage display::Mat2QImage(Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {
        cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;
}
/***************************************
* record log
***************************************/
void display::record_log(string context){
    log<<context<<std::endl;
}
/***************************************
* create log file
***************************************/
void display::create_log(){
    std::stringstream ss;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    ss << "./Log/TrainLog";
    ss << 1 + ltm->tm_mon;
    ss <<  ltm->tm_mday ;
    ss << ".txt";
    log.open(ss.str().c_str());
}
/***************************************
* set slider bar
***************************************/
void display::setSlider(){
    slider->setMinimum(0);
    slider->setMaximum(video_length);
}
/***************************************
* change slider bar
***************************************/
void display::changeslider(int pos){
	pos = slider->value();
}
/***************************************
* set bounding box
* because QLabel size and image size is not equal
***************************************/
Rect display::setROI(){
    Rect r;
    double rateX,rateY;
    rateX = (double)frame.cols/frameShow->width();
    rateY = (double)frame.rows/frameShow->height();
    r.x =roi->x*rateX;
    r.y =roi->y*rateY;
    r.width =roi->width*rateX;
    r.height =roi->height*rateY;
    return r;
}
/***************************************
* show bounding box close-up
***************************************/
void display::showROI(Rect r){
    if(r.width >0 && r.height >0){
		Mat temp;
		frame(r).copyTo(roiframe);
		resize(frame(r), temp, Size(roiShow->size().width(), roiShow->size().height()));
		roiShow->setPixmap(
			QPixmap::fromImage(Mat2QImage(temp)).scaled(roiShow->size(),
			Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}
Mat display::getROI(){
	return roiframe;
}
/***************************************
* confirm video is finished
***************************************/
void display::video_finish(){
	isfinish = true;
}