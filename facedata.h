#ifndef FACEDATA_H
#define FACEDATA_H

#include <iostream>

#include <opencv2/opencv.hpp>

#include <vector>

#include <time.h>


using namespace std;

using namespace cv;


class faceData
{
public:
    faceData(){}
    ~faceData(){}
    bool isNeedRecognize;

    void setLoc(Rect loc){
        location = loc;
        if(locBuffer.size() < 10){
            locBuffer.push_back(loc);
        }else{
            locBuffer.erase(locBuffer.begin());
            locBuffer.push_back(loc);
        }
            return; }
    Rect getLoc(){return location; }
    Rect getLocNorm(){
        Rect norm;
        for(int i = 0 ; i < locBuffer.size() ; ++i ){

        }


    }

    void setId(string sid){id = sid; return; }
    string getId(){return id; }

    void setStopTime(clock_t t){stopTime = t; return; }
    clock_t getStopTime(){return stopTime; }

    void setLastUpdateTime(clock_t t){lastUpdateTime = t; return; }
    clock_t getLastUpdateTime(){return lastUpdateTime; }

//    void pushBackImg(Mat f){faceImg.push_back(f); return; }
//    Mat popBackImg(){return faceImg.pop_back();}

    vector<Mat> faceImg;


private:
    Rect location;
    string id;
    clock_t stopTime;
    clock_t lastUpdateTime;
    vector<Rect> locBuffer;

//    vector<Mat> faceImg;
};

#endif // FACEDATA_H
