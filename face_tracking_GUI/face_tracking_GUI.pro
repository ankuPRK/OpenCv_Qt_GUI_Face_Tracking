#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T01:36:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CannyStillWithQtGUI1
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp

HEADERS  += frmmain.h

FORMS    += frmmain.ui

########################################################################
# add these to the end of your .pro file, this is so Qt knows about the location of the include and lib directories
# in Qt .pro files, begin a line with a pound character '#' to enter a comment
# note that for the double backslashes, the second is an escape character so the first is seen by Qt as a backslash
# the single backslashes at the end of each line (except for the last line) are line continuation characters
INCLUDEPATH += C:\\opencv\\build\\include
#INCLUDEPATH += C:\\opencv\\sources
LIBS += -LC:\\opencv\\mybuild\\lib\\Debug \
-lopencv_imgcodecs300d \
-lopencv_imgproc300d \
-lopencv_ml300d \
-lopencv_objdetect300d \
-lopencv_photo300d \
-lopencv_shape300d \
-lopencv_stitching300d \
-lopencv_superres300d \
-lopencv_ts300d \
-lopencv_video300d \
-lopencv_videoio300d \
-lopencv_videostab300d \
-lopencv_calib3d300d \
-lopencv_core300d \
-lopencv_features2d300d \
-lopencv_flann300d \
-lopencv_hal300d \
-lopencv_highgui300d \
# Note: it is recommended to add a blank line at the end of your .pro file ############################
