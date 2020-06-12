#ifndef VIDEOWIDGETSURFACE_H
#define VIDEOWIDGETSURFACE_H

#include <QtCore/QRect>
#include <QtGui/QImage>
#include <qabstractvideosurface.h>
#include <qvideoframe.h>
//! [0]
class VideoWidgetSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoWidgetSurface(QWidget *widget, QObject *parent = 0);
    QImage cf;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(        QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
    bool start(const QVideoSurfaceFormat &format);
    void stop();
    bool present(const QVideoFrame &frame);
    QRect videoRect() const { return targetRect; }
    void updateVideoRect();
    void paint(QPainter *painter);
signals:
    void enviandoFotogramaDeVideo(QImage img);
private:
    QWidget *widget;
    QImage::Format imageFormat;
    QRect targetRect;
    QSize imageSize;
    QRect sourceRect;
    QVideoFrame currentFrame;
    QImage fotograma;
};
//! [0]

#endif // VIDEOWIDGETSURFACE_H
