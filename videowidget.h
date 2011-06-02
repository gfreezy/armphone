#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include "videodevice.h"

class VideoWidget
{
public:
    VideoWidget(QLabel *label);
    ~VideoWidget();
    void updateImage(uchar* buf, quint64 len);

private:
    QLabel* label;
    QImage *img;

};

#endif // VIDEOWIDGET_H
