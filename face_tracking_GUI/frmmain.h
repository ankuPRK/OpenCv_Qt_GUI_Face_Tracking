#ifndef frmmain_H
#define frmmain_H

#include <QMainWindow>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video/tracking.hpp>
#include<opencv2/videoio/videoio.hpp>
#include<opencv2/objdetect/objdetect.hpp>

#include<iostream>
#include<vector>

///////////////////////////////////////////////////////////////////////////////////////////////////

using namespace cv;
using namespace std;

namespace Ui {
class frmmain;
}

class frmmain : public QMainWindow
{
    Q_OBJECT

public slots:
    void processFrameAndUpdateGUI();            // function prototype

public:
    explicit frmmain(QWidget *parent = 0);
    ~frmmain();

private slots:
    void on_btnOpenFile_clicked();

    void on_btnOpenWebCam_clicked();

    void on_btnPlayPause_clicked();

    void on_btnStop_clicked();

    void on_btnToggleDots_clicked();

private:
    Ui::frmmain *ui;
    VideoCapture cap;         // Capture object to use with webcam

    QTimer* qtimer;             // timer for processFrameAndUpdateGUI()

    QImage frmmain::convertOpenCVMatToQtQImage(cv::Mat mat);
    Mat image, frame, croppedimg;			//type of Mat will be CV_8U 3 channels
    Mat gray, prevGray, mask;   //type of Mat will be CV_8U 1 channel

    vector<Rect> faces;
    CascadeClassifier face_cascade;

    //Parameters of Detection of feature points on the first frame; see the definitions of functions GoodFeaturesToTrack
    //and CornerSubPix for more details//
    vector < vector<Point2f> > points;			//Contains a vector of "vector of points for each face", which are iteratively
                                                //updated after each frame
    vector < vector<Point2f> > pointsPrev;		//vector of 'vector of points for each face' for the previous frame
    vector<Point2f>  tempPoints;
    const int MAX_COUNT = 500;					//max no. of points per rectangle

    //Parameters utilised in calculating the optical flow using the KLT algorithm//
    vector < vector<uchar> > status;		//status is 1 if the updated point is valid, else its status is 0.
    vector < vector<float> > err;
    vector < vector<Point2f> > tmp;
    vector<uchar>  tempStatus;
    vector<float>  tempErr;
    vector<Point2f>  tempTmp;
    Point2f topleft;
    Point2f Botright;


//////parameters to calculate the new(updated) rectangles(faces)///////
    vector<Point2f> centroid;		//vector of centroids of each rectangle(face)
    vector<Point2f> dim;			//vector of (width, height) of the rectangles of previous frame

///////parameters for controlling loop and saving image/////
    int i, j, k;
    bool needToDetect = true;
    int minutecount = 0;
    int secondcount = 0;
    int min_to_sec = 50;
    int nFaces = 0;
    char *str;
    Rect Cropper;
    bool bDisplayDots = true;

    void frmmain::exitProgram();                    // function prototype

    ////some other functions/////
    char * frmmain::get_string_id(int minutecount, int faceid, int secondcount, int tf, char path[]);
    Point2f frmmain::get_centroid(vector<Point2f> *ppoints);
    Point2f frmmain::get_variance(vector<Point2f> *ppoints, Point2f centroid);

    double dWidth, dHeight;

    // function prototype
};

#endif // frmmain_H
