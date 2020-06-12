#include "videowidgetsurface.h"

#include <QtWidgets>
#include <qabstractvideosurface.h>
#include <qvideosurfaceformat.h>

#include <QDebug>
VideoWidgetSurface::VideoWidgetSurface(QWidget *widget, QObject *parent)
    : QAbstractVideoSurface(parent)
    , widget(widget)
    , imageFormat(QImage::Format_Invalid)
{
}

//! [0]
QList<QVideoFrame::PixelFormat> VideoWidgetSurface::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32;
                //<< QVideoFrame::Format_ARGB32
                //<< QVideoFrame::Format_ARGB32_Premultiplied
                //<< QVideoFrame::Format_RGB565
                //<< QVideoFrame::Format_RGB555;

    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
//! [0]

//! [1]
bool VideoWidgetSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
            && !size.isEmpty()
            && format.handleType() == QAbstractVideoBuffer::NoHandle;
}
//! [1]

//! [2]
bool VideoWidgetSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();
    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->imageFormat = imageFormat;
        imageSize = size;
        sourceRect = format.viewport();

        QAbstractVideoSurface::start(format);

        widget->updateGeometry();
        updateVideoRect();

        return true;
    } else {
        return false;
    }
}
//! [2]

//! [3]
void VideoWidgetSurface::stop()
{
    currentFrame = QVideoFrame();
    targetRect = QRect();
    QAbstractVideoSurface::stop();
    widget->update();
}
//! [3]

//! [4]
bool VideoWidgetSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        stop();
        return false;
    } else {
        currentFrame = frame;


        /*currentFrame.map(QAbstractVideoBuffer::ReadOnly);
        QImage frameAsImage = QImage(currentFrame.bits(), currentFrame.width(),
        currentFrame.height(), currentFrame.bytesPerLine(),
        QVideoFrame::imageFormatFromPixelFormat(currentFrame.pixelFormat()));



        //salida-> = QPixmap(QPixmap::fromImage(frameAsImage));
        //QPainter painter(salida);
        //painter.drawPixmap(0,0,frameAsImage.width(), frameAsImage.height(), QPixmap::fromImage(frameAsImage));


        emit enviandoFotogramaDeVideo(frameAsImage);

        currentFrame.unmap();*/

        //emit enviandoFotogramaDeVideo(frame);

        QVideoFrame frame2 = currentFrame;
        frame2.map(QAbstractVideoBuffer::ReadOnly);
                QImage frameAsImage = QImage(frame2.bits(), frame2.width(),
                frame2.height(), frame2.bytesPerLine(),
                QVideoFrame::imageFormatFromPixelFormat(frame2.pixelFormat()));

        //emit enviandoFotogramaDeVideo(frameAsImage);
                //qDebug()<<"Enviando señal de Fotograma";
                //frameAsImage.save("D:/prueba.jpg");
                cf=frameAsImage;
        frame2.unmap();
        widget->repaint(targetRect);
        return true;
    }
}
//! [4]

//! [5]
void VideoWidgetSurface::updateVideoRect()
{
    QSize size = surfaceFormat().sizeHint();
    size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);

    targetRect = QRect(QPoint(0, 0), size);
    targetRect.moveCenter(widget->rect().center());
}
//! [5]

//! [6]
void VideoWidgetSurface::paint(QPainter *painter)
{
    if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform oldTransform = painter->transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
            painter->scale(1, -1);
            painter->translate(0, -widget->height());
        }
        QImage image(
                    currentFrame.bits(),
                    currentFrame.width(),
                    currentFrame.height(),
                    currentFrame.bytesPerLine(),
                    imageFormat);

        painter->drawImage(targetRect, image, sourceRect);
        painter->setTransform(oldTransform);
        //emit enviandoFotogramaDeVideo(image);
        currentFrame.unmap();
    }
}
//! [6]
