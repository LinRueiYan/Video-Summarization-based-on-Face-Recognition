#include "facetrack.h"
#include "facedetection.h"
#include "facedata.h"
#include <time.h>
#include <iomanip>
#include <dirent.h>
#include <iostream>

#define BLURTHRESHOLD 50

using namespace std;

faceTrack::faceTrack()
{
//    idInit = 0;
    faceObjLock = false;
    outputFolder = "D://Qt Project/outputData";
    dbFormat = ".pgm";
}

faceTrack::faceTrack(string s)
{
//    idInit = 0;
    faceObjLock = false;
    outputFolder = s;
    dbFormat = ".pgm";
}
void faceTrack::setOutputFolder(string s){
    outputFolder = s;
}

void faceTrack::setPicRate(Point pR){
    picRate = pR;
}

faceTrack::~faceTrack()
{

}
wstring faceTrack::s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

vector<faceData>* faceTrack::getFaceObj(){
    while (faceObjLock) {
    }
    return &faceObj;
}

void faceTrack::onTrack(Mat frame,vector<Rect> faces, Mat movskin){
    faceObjLock = true;
    clock_t now;
//    if(faces.size() > 0 || faceObj.size() > 0){
        int mapthresh = 10;
        vector<int> delList;

        //mapping face & record :: multimap <key,value>
        multimap<int,int> face2ObjList;           //mogROI:objList
        multimap<int,int> objList2Face;    //objList:mogROI
        for(int i = 0 ; i < faces.size() ; ++i){
            for(int j = 0 ; j < faceObj.size() ; ++j){
                Rect overlap = faces.at(i)&faceObj.at(j).getLoc();
                if(overlap.width*overlap.height > mapthresh){ //have overlap
                    face2ObjList.insert(make_pair(i,j));
                    objList2Face.insert(make_pair(j,i));
                }
            }
        }
        //end of mapping face & record



        //search for each faces
        for(int facesItem = 0 ; facesItem < faces.size() ; ++facesItem){
            if(face2ObjList.count(facesItem) == 0 ){
                //new object
//                cout << "new "  << endl;
                faceData newFace;
                newFace.setId("");
//                ++idInit;
                newFace.setLoc(faces.at(facesItem));
                now = clock();
                newFace.setLastUpdateTime(now);
                faceObj.push_back(newFace);
            }else{

                int objItem = face2ObjList.find(facesItem)->second;

                if(face2ObjList.count(facesItem) == 1 && objList2Face.count(objItem) == 1){
                    //keep tracking & get Mat
                    faceObj.at(objItem).setLoc(faces.at(facesItem));
                    now = clock();
                    if((now - faceObj.at(objItem).getLastUpdateTime())/(double)(CLOCKS_PER_SEC) > 0.1){
                        Mat aface(Size(frame.cols,frame.rows),CV_8UC3);
                        frame.copyTo(aface);
                        Mat tempROI(aface,(faces.at(facesItem)));
                        Mat check,dst;
                        tempROI.copyTo(check);
                        GaussianBlur( check, check, Size(3,3), 0, 0, BORDER_DEFAULT );
                        Laplacian( check, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT );
                        convertScaleAbs( dst, dst );
                        cv::threshold( dst, dst, BLURTHRESHOLD, 255, cv::THRESH_TOZERO);
                        if(sum(dst)[0] > 100000){
                        now = clock();
                        faceObj.at(objItem).setLastUpdateTime(now);
                        faceObj.at(objItem).faceImg.push_back(tempROI);
                        cout << " IMG SIZE "  << faceObj.at(objItem).faceImg.size() << endl;
                        }
                    }
                }
                if(face2ObjList.count(facesItem) > 1 && objList2Face.count(objItem) == 1){
                    //special case 1 face:objList = 1 : N
                    cout << "special case 1 " << endl;
                }
                if(objList2Face.count(objItem) > 1){
                    //special case 2 face:objList = N : 1 / N : N
                    cout << "special case 2 " << endl;
                }
            }
        }
//        cout << "faceObj size "  << faceObj.size() << endl;

        //check movSkin to choose tracking or disappear
        int minObjSize = 100;
        vector<Rect> msObj = FaceDetection::connectComponent(movskin,minObjSize);

        multimap<int,int> ms2ObjList;    //mogROI:objList
        multimap<int,int> objList2Ms;    //objList:mogROI
        for(int i = 0 ; i < msObj.size() ; ++i){
            for(int j = 0 ; j < faceObj.size() ; ++j){
                Rect overlap = msObj.at(i)&faceObj.at(j).getLoc();
                if(overlap.width*overlap.height > mapthresh){ //have overlap
                    ms2ObjList.insert(make_pair(i,j));
                    objList2Ms.insert(make_pair(j,i));
                }
            }
        }


        //search for each faceObject
        for(int objItem = 0 ; objItem < faceObj.size() ; ++objItem){
            if(objList2Face.count(objItem) == 0 ){
                //check movSkin to choose tracking or disappear
                if(objList2Ms.count(objItem) == 0){
                    //obj disp save all Mat & throw from list

//                    cout << "disp end  " << endl;
                    now = clock();
                    if((now - faceObj.at(objItem).getStopTime())/(double)(CLOCKS_PER_SEC) > 5 ){
                        delList.push_back(objItem);
                    }else if(faceObj.at(objItem).getLoc().x < frame.cols/50 ||
                             faceObj.at(objItem).getLoc().y < frame.rows/50 ||
                             faceObj.at(objItem).getLoc().x + faceObj.at(objItem).getLoc().width > frame.cols - frame.cols/50 ||
                             faceObj.at(objItem).getLoc().y + faceObj.at(objItem).getLoc().height > frame.rows - frame.rows/50){
                        delList.push_back(objItem);
                    }else{
                        faceObj.at(objItem).setStopTime(clock());
                    }
//                    cout << "disp end  " << endl;
                }
                else{
                    int msItem = objList2Ms.find(objItem)->second;

                    if(objList2Ms.count(objItem) == 1 && ms2ObjList.count(msItem) == 1){
                        if(msObj.at(msItem).width*msObj.at(msItem).height > faceObj.at(objItem).getLoc().width*faceObj.at(objItem).getLoc().height*2){
                             delList.push_back(objItem);
                        }else{
                        faceObj.at(objItem).setLoc(msObj.at(msItem));
                        }
                    }
                    if(objList2Ms.count(objItem) == 1 && ms2ObjList.count((msItem)) > 1){
                        //special case 3 objList:movSkin = 1 : N
                        cout << "special case 3 " << endl;
                    }
                    if(ms2ObjList.count((msItem)) > 1){
                        //special case 4 objList:movSkin = N : 1 / N : N
//                        if(){

//                        }

                        cout << "special case 4 " << endl;
                    }
                }



           }


        }  //end of search for each faceObject

        //deal with delete
        if(delList.size() > 0){
        sort(delList.begin(),delList.end());
        reverse(delList.begin(),delList.end());
        DIR *dir;
        dirent *entry;
        char *subfolder;
        int subLenght;
        for(int i = 0; i < delList.size() ; ++i ){
            //when delete, find each folder and save face image
            stringstream ss;
            ss << outputFolder << "/" << faceObj.at(delList.at(i)).getId();
            subfolder = (char*) malloc(ss.str().size());
            strcpy(subfolder, ss.str().c_str());

            //try to open folder
            try {
                dir=opendir(ss.str().c_str());

            } catch (cv::Exception& e) {
                cerr << "Error opening output folder \"" << ss.str() << "\". Reason: " << e.msg << endl;
                exit(1);
            }

            //if no folder, create one
            if(dir == 0){
               LPCWSTR subs;
               wstring stemp = s2ws(ss.str());
               subs =  stemp.c_str();
               CreateDirectory(subs,NULL);
               dir=opendir(ss.str().c_str());
            }

            //find menber of file in folder
            int talImg = 0;
            char *StrPointer;
            while(entry=readdir(dir)){
                if((StrPointer=strstr(entry->d_name,dbFormat.c_str()))){
                    ++talImg;
                }
            }

            for(int j = 0 ; j < faceObj.at(delList.at(i)).faceImg.size() ; ++j){
               if(faceObj.at(delList.at(i)).getId().length() > 0){
                   ostringstream outFileName;
                   outFileName << ss.str() << "/" << talImg + j  << dbFormat;
                   ostringstream lookFileName;
                   lookFileName << ss.str() << "/" << talImg + j  << ".jpg";
                   if(picRate.x !=0 && picRate.y != 0){
                        resize(faceObj.at(delList.at(i)).faceImg.at(j),faceObj.at(delList.at(i)).faceImg.at(j),Size(picRate.x,picRate.y));
                   }
                   cvtColor(faceObj.at(delList.at(i)).faceImg.at(j), faceObj.at(delList.at(i)).faceImg.at(j), CV_BGR2GRAY );
                   equalizeHist(faceObj.at(delList.at(i)).faceImg.at(j),faceObj.at(delList.at(i)).faceImg.at(j));
                   imwrite(lookFileName.str(),faceObj.at(delList.at(i)).faceImg.at(j));
                   imwrite(outFileName.str(),faceObj.at(delList.at(i)).faceImg.at(j));
               }
            }
            faceObj.erase(faceObj.begin() + delList.at(i));
        }
        }
        //    }
    faceObjLock = false;
    return;
}
