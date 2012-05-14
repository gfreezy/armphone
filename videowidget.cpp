#include "videowidget.h"

VideoWidget::VideoWidget(QLabel *label)
{
    this->label = label;
    this->img = new QImage(320, 240, QImage::Format_RGB888);
}

void VideoWidget::updateImage(uchar *buf, quint64 len)
{
    img->loadFromData((uchar*)buf, len);
    label->setPixmap(QPixmap::fromImage(*img));
    label->update();

}

VideoWidget::~VideoWidget()
{
    delete img;
}



