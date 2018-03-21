#ifndef FACETRACK_H
#define FACETRACK_H
#include <windows.h>
#include <afxres.h>
#include "facedata.h"


class faceTrack
{
public:
    faceTrack();
    faceTrack(string s);
    ~faceTrack();

    void onTrack(Mat frame, vector<Rect> faces, Mat movskin);
    vector<faceData> *getFaceObj();
    void setOutputFolder(string s);
    void setPicRate(Point pR);


private:
    vector<faceData> faceObj;
    string outputFolder;
    string dbFormat;
//    string idInit;
    bool faceObjLock;
    Point picRate;
    wstring s2ws(const std::string& s);
};

#endif // FACETRACK_H
