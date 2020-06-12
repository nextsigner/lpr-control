#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include "videowidgetsurface.h"
#include <QtWidgets/QWidget>


QT_BEGIN_NAMESPACE
class QAbstractVideoSurface;
QT_END_NAMESPACE
class VideoWidgetSurface;
//! [0]
class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    QAbstractVideoSurface *videoSurface() const { return surface; }
    QSize sizeHint() const;
public slots:
    void setFotogramaActual(const QImage &img);
    QImage getfotogramaActual();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    VideoWidgetSurface *surface;
    QImage fotograma;
    //IplImage cf;
};
//! [0]

#endif // VIDEOWIDGET_H
