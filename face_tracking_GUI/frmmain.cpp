#include "frmmain.h"
#include "ui_frmmain.h"

#include<QFileDialog>
#include<QtCore>
#include<QMessageBox>


// constructor ////////////////////////////////////////////////////////////////////////////////////
frmmain::frmmain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmmain)
{
    ui->setupUi(this);
    qtimer = new QTimer(this);                                                      // instantiate timer
}

// destructor /////////////////////////////////////////////////////////////////////////////////////
frmmain::~frmmain()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void frmmain::exitProgram() {
    if(qtimer->isActive()) qtimer->stop();          // if timer is running, stop timer
    QApplication::quit();                           // and exit program
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void frmmain::processFrameAndUpdateGUI() {

    /*    The loop does the following:
        ->	It detects the rectangles for each face and in each rectangle
            it detects good features using GoodFeaturesToTrack function.
        ->	Then it just tracks those points by calculating their Optical Flow
            by using KLT algorithm via calcOpticalFlowPyrLK function.
            -	To draw rectangles on tracked points, we use two assumptions:
                a. Centroid of the updated points of each face will be the centroid of the updated rectangle.
                b. Ratio of the dimentions of the new rectangle to the old rectangle will be the ratio of the
                    variance of the points of new rectangle to the variance of the points of the old rectangle
        ->	After 50 frames, it refreshes and starts from again.
        ->	The loop terminates if the user presses the Esc key.
    */

    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
    Size subPixWinSize(10, 10);
    Size winSize(31, 31);

    bool blnFrameReadSuccessfully = cap.read(frame);                    // get next frame from the webcam
    if (!blnFrameReadSuccessfully || frame.empty()) {                            // if we did not get a frame
        QMessageBox::information(this, "", "unable to read from webcam \n\n exiting program\n");        // show error via message box
        exitProgram();                                                                              // and exit program
        return;                                                                                     //
    }
    cvtColor(frame, gray, CV_BGR2GRAY);		// convert to grayscale
    frame.copyTo(image);


    //This part of the loop detects the faces using Viola Jones Algorithm
        if (needToDetect) {
            // Detect faces
            faces.clear();
            face_cascade.detectMultiScale(gray, faces, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30));
            nFaces = faces.size();	//no of faces

            //preparing the vectors
            points.clear();
            centroid.resize(nFaces);
            dim.resize(nFaces);
            /////detectING the good features in all the rectangles and storing those features in points vector////
            for (i = 0; i < nFaces; i++) {

                //making the mask for ith rectangle
                mask = Scalar(0,0,0);
                mask(faces[i]) = Scalar(255);

                //store good features of ith rectangle in tempPoints vector
                tempPoints.clear();
                goodFeaturesToTrack(gray, tempPoints, MAX_COUNT, 0.01, 10, mask, 3, 0, 0.04);
                points.push_back(tempPoints);	//push tempPoints to points
                cornerSubPix(gray, points[i], subPixWinSize, Size(-1, -1), termcrit);


                if (points[i].empty()) {
                    faces.erase(faces.begin() + i);
                    points.erase(points.begin() + i);
                    nFaces = faces.size();
                    i--;
                    centroid.resize(nFaces);
                    dim.resize(nFaces);
                }
                else {
                    //calculate the centroid
                    centroid[i].x = faces[i].x + faces[i].width / 2.0;
                    centroid[i].y = faces[i].y + faces[i].height / 2.0;

                    //get the dimensions and centroids of all the rectangles
                    dim[i].x = faces[i].width;
                    dim[i].y = faces[i].height;

                    //draw the points on the image
                    if(bDisplayDots){
                        for (j = 0; j < points[i].size(); j++) {
                            cv::circle(image, points[i][j], 1, Scalar(255, 255, 255), -1, 8);
                        }
                    }
                }
            }
            needToDetect = false;
        }
        else
        {
            ////do the tracking////
            status.clear();
            err.clear();
            for (i = 0; i < nFaces; i++) {
                tempStatus.clear();
                tempErr.clear();
                //KLT tracking function
                calcOpticalFlowPyrLK(prevGray, gray, pointsPrev[i], points[i], tempStatus, tempErr, winSize,
                    3, termcrit, 0, 0.001);
                status.push_back(tempStatus);
                err.push_back(tempErr);
            }

            for (i = 0; i < nFaces; i++) {

                //update the points
                for (j = 0, k = 0; j < points[i].size(); j++)
                {
                    if (!status[i][j])
                        continue;
                    points[i][k++] = points[i][j];
                    if(bDisplayDots)
                    cv::circle(image, points[i][j], 1, Scalar(255, 255, 255), -1, 8);
                }
                points[i].resize(k);
                if (points[i].empty()) {
                    faces.erase(faces.begin() + i);
                    points.erase(points.begin() + i);
                    status.erase(status.begin() + i);
                    err.erase(err.begin() + i);
                    dim.erase(dim.begin() + i);
                    nFaces = faces.size();
                    i--;
                    centroid.resize(nFaces);
                }
                else {
                    //calculate centroid
                    centroid[i] = frmmain::get_centroid(&(points[i]));

                    //draw the centriod in red color
                    if(bDisplayDots)
                    cv::circle(image, centroid[i], 2, Scalar(0, 0, 255), -1, 8);
                }
            }
        }

        //
        //draw rectangles around the faces
        for (i = 0; i < nFaces; i++) {

            //binding the rectangle into the image
            topleft.x = (centroid[i].x - dim[i].x / 2 > 0 ? centroid[i].x - dim[i].x / 2 : 0);
            topleft.y = (centroid[i].y - dim[i].y / 2 > 0 ? centroid[i].y - dim[i].y / 2 : 0);
            Botright.x = (centroid[i].x + dim[i].x / 2 < dWidth ? centroid[i].x + dim[i].x / 2 : dWidth);
            Botright.y = (centroid[i].y + dim[i].y / 2 < dHeight ? centroid[i].y + dim[i].y / 2 : dHeight);

//            cout << "topleft: " << topleft << "botright: " << Botright << "\n";
            Cropper = Rect{(int)topleft.x,(int)topleft.y,(int)Botright.x - (int)topleft.x, (int)Botright.y - (int)topleft.y};

            rectangle(image, Cropper , Scalar(255, 0, 0), 1, 8, 0);
            str = frmmain::get_string_id(minutecount, i, secondcount, 0, "data\\");
//            cout << "cropping rectangle: " << Cropper << "\n";
            frame(Cropper).copyTo(croppedimg);
            imwrite(str, croppedimg);
//            cout << "id: " << str << "\n";
        }

        pointsPrev.clear();
        pointsPrev = points;		//update pointsPrev
        gray.copyTo(prevGray);		//update the previous gray frame
        secondcount++;				//increment the frame counter

        if (secondcount == min_to_sec) {
            //start afresh
            pointsPrev.clear();
            needToDetect = true;
            secondcount = 0;
            minutecount++;
        }

    QImage qimgOriginal = convertOpenCVMatToQtQImage(frame);             // convert from OpenCV Mat to Qt QImage
    QImage qimgFinal = convertOpenCVMatToQtQImage(image);                   //

    ui->lblOriginal->setPixmap(QPixmap::fromImage(qimgOriginal));       // show images on form labels
    ui->lblCanny->setPixmap(QPixmap::fromImage(qimgFinal));             //
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void frmmain::on_btnOpenFile_clicked()
{
    qtimer->stop();

    needToDetect = true;
    minutecount = 0;
    secondcount = 0;
    nFaces = 0;
//    face_cascade.~CascadeClassifier();
//    face_cascade = new CascadeClassifier(this);


    QString strFileName = QFileDialog::getOpenFileName();       // bring up open file dialog
    if(strFileName == "") {                                     // if file was not chosen
            ui->lblChosenFile->setText("file not chosen");          // update label
            return;                                                 // and exit function
    }

    cap.open(strFileName.toStdString());              // associate the capture object to the default webcam

    if(cap.isOpened() == false) {                 // if unsuccessful
        ui->lblChosenFile->setText("error: Video not read from file");      // update lable with error message
        exitProgram();                          // and exit program
        return;                                 //
    }

    dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    face_cascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt2.xml");

    if(face_cascade.empty()){
        ui->lblChosenFile->setText("error: xml file not loaded.");      // update lable with error message
        exitProgram();                          // and exit program
        return;                                 //
    }

    cap >> frame;			// read first frame
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    gray.copyTo(mask);		//use it to give size and type to the mask

    system("md data");		//create directory names 'data'


    ui->lblChosenFile->setText(strFileName);                // update label with file name
    ui->btnPlayPause->setEnabled(true);
    ui->btnPlayPause->setText("Pause");
    ui->btnStop->setEnabled(true);
    ui->btnToggleDots->setEnabled(true);

    qtimer = new QTimer(this);                                                      // instantiate timer
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // associate timer to processFrameAndUpdateGUI
    qtimer->start(20);                  // start timer, set to cycle every 20 msec (50x per sec), it will not actually cycle this often
}

///////////////////////////////////////////////////////////////////////////////////////////////////
QImage frmmain::convertOpenCVMatToQtQImage(cv::Mat mat) {
    if(mat.channels() == 1) {                   // if grayscale image
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);     // declare and return a QImage
    } else if(mat.channels() == 3) {            // if 3 channel color image
        cvtColor(mat, mat, CV_BGR2RGB);     // invert BGR to RGB
        return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);       // declare and return a QImage
    } else {
        qDebug() << "in convertOpenCVMatToQtQImage, image was not 1 channel or 3 channel, should never get here";
    }
    return QImage();        // return a blank QImage if the above did not work

}

void frmmain::on_btnOpenWebCam_clicked()
{
    qtimer->stop();
    needToDetect = true;
    minutecount = 0;
    secondcount = 0;
    nFaces = 0;
//    face_cascade.~CascadeClassifier();
//    face_cascade = new CascadeClassifier(this);

    cap.open(0);              // associate the capture object to the default webcam
    if(cap.isOpened() == false) {                 // if unsuccessful
        ui->lblChosenFile->setText("error: Video not read from WebCam");      // update lable with error message
        exitProgram();                          // and exit program
        return;                                 //
    }

    dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    face_cascade.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt2.xml");

    if(face_cascade.empty()){
        ui->lblChosenFile->setText("error: xml file not loaded.");      // update lable with error message
        exitProgram();                          // and exit program
        return;                                 //
    }

    cap >> frame;			// read first frame
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    gray.copyTo(mask);		//use it to give size and type to the mask

    system("md data");		//create directory names 'data'


    ui->lblChosenFile->setText("WebCam");                // update label with file name
    ui->btnPlayPause->setEnabled(true);
    ui->btnPlayPause->setText("Pause");
    ui->btnStop->setEnabled(true);
    ui->btnToggleDots->setEnabled(true);
    connect(qtimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));     // associate timer to processFrameAndUpdateGUI
    qtimer->start(20);                  // start timer, set to cycle every 20 msec (50x per sec), it will not actually cycle this often
}

void frmmain::on_btnPlayPause_clicked()
{
    if(qtimer->isActive()){
        qtimer->stop();
        ui->btnPlayPause->setText("Play");
    }
    else{
        qtimer->start(20);
        ui->btnPlayPause->setText("Pause");

    }
}

void frmmain::on_btnStop_clicked()
{
    cap.release();
    qtimer->stop();
    needToDetect = true;
    minutecount = 0;
    secondcount = 0;
    nFaces = 0;
//    face_cascade.~CascadeClassifier();
//    face_cascade = new CascadeClassifier(this);

    ui->btnPlayPause->setText("Play/Pause");
    ui->btnPlayPause->setDisabled(true);
    ui->btnStop->setDisabled(true);
    ui->btnToggleDots->setDisabled(true);
    ui->lblChosenFile->setText("Video is stopped.");
}

void frmmain::on_btnToggleDots_clicked()
{
    bDisplayDots = !bDisplayDots;
}


char *frmmain::get_string_id(int minutecount, int faceid, int secondcount, int tf, char path[]) {

    int minutesize = 6;
    int secondsize = 3;
    int faceidsize = 2;
    int tfsize = 1;
    int idsize = minutesize + secondsize + faceidsize + tfsize + 4 + 1;		//4 is for extension .jpg and 1 is for '\0'
    int i = 0;
    int length = 0;
    while (path[i] != '\0') {
        length++;
        i++;
    }

    char *out = (char *)malloc(sizeof(char) * (idsize + length));

    for (i = 0; i < length; i++) {
        out[i] = path[i];
    }
    for (i = 0; i < minutesize; i++) {
        out[minutesize - i - 1 + length] = 48 + (minutecount % 10);
        minutecount = minutecount / 10;
    }
    for (i = 0; i < faceidsize; i++) {
        out[minutesize + faceidsize - i - 1 + length] = 48 + (faceid % 10);
        faceid = faceid / 10;
    }
    for (i = 0; i < secondsize; i++) {
        out[minutesize + secondsize + faceidsize - i - 1 + length] = 48 + (secondcount % 10);
        secondcount = secondcount / 10;
    }
    for (i = 0; i < tfsize; i++) {
        out[tfsize + minutesize + secondsize + faceidsize - i - 1 + length] = 48 + (tf % 10);
        tf = tf / 10;
    }
    out[length + idsize - 1] = '\0';
    out[length + idsize - 2] = 'g';
    out[length + idsize - 3] = 'p';
    out[length + idsize - 4] = 'j';
    out[length + idsize - 5] = '.';

    return out;
}

//function to calculate the variance of a Point2f vector, having a Point2f centroid
Point2f frmmain::get_variance(vector<Point2f> *ppoints,Point2f centroid) {
    int j = 0;
    Point2f variance;
    variance.x = 0;
    variance.y = 0;
    if ((*ppoints).size() == 0) {
        return (Point2f)NULL;
    }
    for (j = 0; j < (*ppoints).size(); j++) {
        variance.x = variance.x + ((*ppoints)[j].x - centroid.x)*((*ppoints)[j].x - centroid.x);
        variance.y = variance.y + ((*ppoints)[j].y - centroid.y)*((*ppoints)[j].y - centroid.y);
    }
    variance.x = variance.x / (*ppoints).size();
    variance.y = variance.y / (*ppoints).size();
    return variance;
}

//function to calculate the centroid of a Point2f vector
Point2f frmmain::get_centroid(vector<Point2f> *ppoints) {
    int p = 0;
    Point2f centroid;
    centroid.x = 0;
    centroid.y = 0;

    if ((*ppoints).size() == 0) {
        return (Point2f)NULL;
    }

    for (p = 0; p < (*ppoints).size(); p++) {
        centroid.x = centroid.x + (*ppoints)[p].x;
        centroid.y = centroid.y + (*ppoints)[p].y;
    }
    centroid.x = centroid.x / (*ppoints).size(); //Mean cordinates of points[i]
    centroid.y = centroid.y / (*ppoints).size(); // Mean cordinates of points[i]

    return centroid;
}

