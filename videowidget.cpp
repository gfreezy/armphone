#include "videowidget.h"

VideoWidget::VideoWidget(QLabel *label)
{
    this->label = label;
    this->img = new QPixmap(320, 240);
}

void VideoWidget::updateImage(char *buf, quint64 len)
{
    img->loadFromData((uchar*)buf, len);
    label->setPixmap(*img);
    label->update();

}

VideoWidget::~VideoWidget()
{
    delete img;
}



