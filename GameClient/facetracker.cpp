#include "facetracker.h"

faceTracker::faceTracker() {
    mCascadeFile = "";
    mCascade = NULL;
    flags = 0;

    mStorage = cvCreateMemStorage(0);
}

faceTracker::~faceTracker() {
    if(mCascade) cvReleaseHaarClassifierCascade(&mCascade);
    mCascadeFile = "";
    mCascade = NULL;
    flags = 0;
}

void faceTracker:: setCascadeFile(QString cascadeFile) {
    mCascadeFile = cascadeFile;
    if(mCascade) cvReleaseHaarClassifierCascade(&mCascade);
    mCascade = (CvHaarClassifierCascade *) cvLoad(mCascadeFile.toUtf8());
}

QString faceTracker:: cascadeFile() const {
    return mCascadeFile;
}

void faceTracker:: setFlags(int iFlags) {
    flags = iFlags;
}

QVector<QRect> faceTracker:: detect(IplImage *cvImage) {
    QVector<QRect> listRect;
    CvRect *rect = NULL;

    double scale = 1.3;

    // gray scale
    IplImage *grayImage = cvCreateImage(cvSize(cvImage -> width, cvImage -> height), cvImage->depth, CV_8UC1);
    IplImage *smallImage = cvCreateImage(cvSize(cvRound(cvImage -> width / scale), cvRound(cvImage -> height / scale)), cvImage -> depth, CV_8UC1);

    cvCvtColor(cvImage, grayImage, CV_BGR2GRAY);
    cvResize(grayImage, smallImage);
    cvEqualizeHist(smallImage, smallImage);
    cvClearMemStorage(mStorage);

    // face detecting
    if(mCascade) {
        CvSeq *faces = cvHaarDetectObjects(smallImage, mCascade, mStorage, 1.2, 4, flags, cvSize(64, 64));

        for(int i = 0; i < faces -> total; ++ i) {
            rect = (CvRect*) cvGetSeqElem(faces, i);
            listRect.append(QRect(rect -> x * scale, rect -> y * scale, rect -> width * scale, rect -> height * scale));
        }
    }

    cvReleaseImage(&grayImage);
    cvReleaseImage(&smallImage);

    return listRect;
}
