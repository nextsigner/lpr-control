#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "definiciones.h"

#include <QLibraryInfo>

#include <QMediaService>
#include <QMediaRecorder>
#include <QtMultimediaWidgets/QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaMetaData>

#include <QMessageBox>
#include <QPalette>

//#include "capturebuffer.h"

Q_DECLARE_METATYPE(QCameraInfo)

void MainWindow::setCamera(const QCameraInfo &cameraInfo)
{
    camera = new QCamera(cameraInfo);
    connect(camera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(updateCameraState(QCamera::State)));
    connect(camera, SIGNAL(error(QCamera::Error)), this, SLOT(displayCameraError()));

    //videoWidget->videoSurface()->se
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->setViewfinder(videoWidget->videoSurface());
    //camera->set

    //captureBuffer = new CaptureBuffer;
    //connect(captureBuffer, SIGNAL(enviandoFotogramaDeVideo(QImage)), this, SLOT(getFotogramaDeVideo(QImage)));
    //camera->setViewfinder(captureBuffer);

    //updateCameraState(camera->state());
    //updateLockStatus(camera->lockStatus(), QCamera::UserRequest);
    camera->start();  
}

void MainWindow::displayRecorderError()
{
    QMessageBox::warning(this, tr("Error de Captura"), mediaRecorder->errorString());
}

void MainWindow::displayCameraError()
{
    QMessageBox::warning(this, tr("Error en la Camara : "), camera->errorString());
}

void MainWindow::updateCameraDevice(QAction *action)
{
    setCamera(qvariant_cast<QCameraInfo>(action->data()));
}

void MainWindow::updateCameraState(QCamera::State  state)
{
    switch (state) {
    case QCamera::ActiveState:
        //ui->actionStartCamera->setEnabled(false);
        //ui->actionStopCamera->setEnabled(true);
        //ui->captureWidget->setEnabled(true);
        //ui->actionSettings->setEnabled(true);
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        //ui->actionStartCamera->setEnabled(true);
        //ui->actionStopCamera->setEnabled(false);
        //ui->captureWidget->setEnabled(false);
        //ui->actionSettings->setEnabled(false);
            ;
    }
}

void MainWindow::updateCaptureMode()
{
    /*int tabIndex = ui->captureWidget->currentIndex();
    QCamera::CaptureModes captureMode = tabIndex == 0 ? QCamera::CaptureStillImage : QCamera::CaptureVideo;

    if (camera->isCaptureModeSupported(captureMode))
        camera->setCaptureMode(captureMode);*/
}

void MainWindow::updateRecorderState(QMediaRecorder::State state)
{
    switch (state) {
    case QMediaRecorder::StoppedState:
        //ui->recordButton->setEnabled(true);
        //ui->pauseButton->setEnabled(true);
        //ui->stopButton->setEnabled(false);
        break;
    case QMediaRecorder::PausedState:
        //ui->recordButton->setEnabled(true);
        //ui->pauseButton->setEnabled(false);
        //ui->stopButton->setEnabled(true);
        break;
    case QMediaRecorder::RecordingState:
        //ui->recordButton->setEnabled(false);
        //ui->pauseButton->setEnabled(true);
        //ui->stopButton->setEnabled(true);
        break;
    }
}

void MainWindow::updateLockStatus(QCamera::LockStatus status, QCamera::LockChangeReason)
{
    QColor indicationColor = Qt::black;

    switch (status) {
    case QCamera::Searching:
        indicationColor = Qt::yellow;
        //ui->statusbar->showMessage(tr("Focusing..."));
        //ui->lockButton->setText(tr("Focusing..."));
        break;
    case QCamera::Locked:
       // indicationColor = Qt::darkGreen;
        //ui->lockButton->setText(tr("Unlock"));
        //ui->statusbar->showMessage(tr("Focused"), 2000);
        break;
    case QCamera::Unlocked:
       // indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
        //ui->lockButton->setText(tr("Focus"));
        //if (reason == QCamera::LockFailed)
            //ui->statusbar->showMessage(tr("Focus Failed"), 2000);
        ;
    }

    //QPalette palette = ui->lockButton->palette();
    //palette.setColor(QPalette::ButtonText, indicationColor);
    //ui->lockButton->setPalette(palette);
}

void MainWindow::updateRecordTime()
{
    //QString str = QString("Recorded %1 sec").arg(mediaRecorder->duration()/1000);
    //ui->statusBar->showMessage(str);
}



/*void MainWindow::processCapturedImage(int requestId, const QImage &img)
{
    Q_UNUSED(requestId);

    IplImage *imgHeaderCaptura = cvCreateImageHeader( cvSize(img.width(), img.height()), IPL_DEPTH_8U, 4);
    imgHeaderCaptura->imageData = (char*) img.bits();



    cvSetImageROI(imgHeaderCaptura, cvRect(reg_px, reg_py, reg_an, reg_al));

    recorte3 = cvCreateImage(cvGetSize(imgHeaderCaptura), imgHeaderCaptura->depth, imgHeaderCaptura->nChannels);

    cvCopy(imgHeaderCaptura, recorte3, NULL);
    cvResetImageROI(imgHeaderCaptura);



    cvReleaseImageHeader(&imgHeaderCaptura);



}*/

void MainWindow::readyForCapture(bool ready)
{
    //ui->pushButtonCapturar->setEnabled(ready);
}

void MainWindow::imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    Q_UNUSED(fileName);

    //qDebug()<<"File id:"<<id;
    //qDebug()<<"Filename:"<<fileName;

    isCapturingImage = false;
    /*if (applicationExiting)
        close();*/
}

//void MainWindow::procesarVideoFrameBuffer(int n, QVideoFrame frame)
//{
    //qDebug()<<"Procesando buffer: -----------";
    //if( vf.map(QAbstractVideoBuffer::ReadOnly) ){

       /*QImage uf(vf.width(),vf.height(),QImage::Format_ARGB32);
       memcpy(uf.bits(), vf.bits(),vf.mappedBytes() - 32);
       QByteArray ddd;
       ddd.append(getChapaPorBuffer(uf));

       ui->tableWidgetProcesadas->insertRow(0);
       ui->tableWidgetProcesadas->setItem(0,0, new QTableWidgetItem(ddd.constData()));*/
   //}


    /*int len = vf.mappedBytes();
    QImage qim;
    qim.loadFromData((const uchar *)vf.bits(), len, (const char *)"JPEG");

    salidaQMotion->setPixmap(QPixmap::fromImage(qim));*/


    /*frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage frameAsImage = QImage(frame.bits(), frame.width(),
    frame.height(), frame.bytesPerLine(),
    QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

    IplImage *imgHeaderCaptura = cvCreateImageHeader( cvSize(frameAsImage.width(), frameAsImage.height()), IPL_DEPTH_8U, 4);
    imgHeaderCaptura->imageData = (char*) frameAsImage.bits();



    cvSetImageROI(imgHeaderCaptura, cvRect(reg_px, reg_py, reg_an, reg_al));

    recorte3 = cvCreateImage(cvGetSize(imgHeaderCaptura), imgHeaderCaptura->depth, imgHeaderCaptura->nChannels);

    cvCopy(imgHeaderCaptura, recorte3, NULL);
    cvResetImageROI(imgHeaderCaptura);

    frame.unmap();

    //motionDetector_.input_qt(*recorte3);


    cvShowImage( "result", recorte3);

    cvReleaseImageHeader(&imgHeaderCaptura);*/




//}
