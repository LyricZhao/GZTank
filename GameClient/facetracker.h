#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <QString>
#include <QVector>
#include <QRect>

#include "cv.h"

class faceTracker
{
public:
    faceTracker();
    ~faceTracker();

    void setCascadeFile(QString cascadeFile);
    QString cascadeFile() const;
    void setFlags(int flags);
    QVector<QRect> detect(IplImage *cvImage);

private:
    CvHaarClassifierCascade *mCascade;
    CvMemStorage *mStorage;

    QString mCascadeFile;
    int flags;
};

#endif // FACETRACKER_H
