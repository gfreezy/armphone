#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include <QLabel>
#include <QPixmap>
#include "videodevice.h"

class VideoWidget
{
public:
    VideoWidget(QLabel *label);
    ~VideoWidget();
    void updateImage(char* buf, quint64 len);

private:
    QLabel* label;
    QPixmap* img;


};

#endif // VIDEOWIDGET_H
