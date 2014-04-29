#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QFile>

#include "noisegen.h"
#include "usbaspdetector.h"

#include <libusb.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_dial_valueChanged(int value);

    void on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;

    QAudioDeviceInfo    m_device;
    QAudioOutput*       m_audioOutput;
    QIODevice*          m_output; // not owned
    QAudioFormat        m_format;
    NoiseGen*           n_generator;

    QTimer tmrAutoDetect;
    USBAspDetector BabyGnusbDetector;

    int connectedPort;

private slots:
    void SlotCheckNewPort();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_dial_3_valueChanged(int value);
};

#endif // DIALOG_H
