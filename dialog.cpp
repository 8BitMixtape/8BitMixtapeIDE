#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

//    ui->textEdit->setPlainText("(t*9&t>>4|t*5&t>>7|t*3&t/1024)-1");

     connectedPort = 0;
     n_generator = 0;

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_dial_valueChanged(int value)
{
}

void Dialog::on_pushButton_3_clicked()
{
    tmrAutoDetect.setInterval(500);
    connect(&tmrAutoDetect, SIGNAL(timeout()), this, SLOT(SlotCheckNewPort()));

    //connectedPort = QextSerialEnumerator::getPorts().count();
    //qDebug() << "Current Port " << connectedPort;

    QString workingDir= qApp->applicationDirPath() + "/../Resources";
    QStringList arguments;

    QProcess makeProcess;

    makeProcess.setWorkingDirectory(workingDir);

    QFile file(workingDir + "/main.c");
    QFile noiseFile("/tmp/noise.c");

    file.open(QIODevice::ReadOnly);
    QString cNoise = file.readAll();
    file.close();


    noiseFile.open(QIODevice::WriteOnly);


    cNoise.replace("{template}", ui->textEdit->toPlainText());

    QTextStream out(&noiseFile);

    out << cNoise;
    noiseFile.close();




    ui->label_3->setText("COMPILING...");

    ///avr-gcc -Wall -Os -DF_CPU=16500000 -mmcu=attiny85 -c noise.c -o noise.o
    arguments << "-Wall";
    arguments << "-Os";
    arguments << "-DF_CPU=16500000";
    arguments << "-mmcu=attiny85";
    arguments << "-c";
    arguments << "/tmp/noise.c";
    arguments << "-o";
    arguments << "/tmp/noise.o";

    makeProcess.start(workingDir + "/avr/bin/avr-gcc", arguments);
    makeProcess.waitForFinished();

    arguments.clear();
    arguments << "-Wall";
    arguments << "-Os";
    arguments << "-DF_CPU=16500000";
    arguments << "-mmcu=attiny85";
    arguments << "-o";
    arguments << "/tmp/noise.elf";
    arguments << "/tmp/noise.o";

    makeProcess.start(workingDir + "/avr/bin/avr-gcc", arguments);
    makeProcess.waitForFinished();

    arguments.clear();
    arguments << "-f";
    arguments << "/tmp/noise.hex";

    makeProcess.start("rm", arguments);
    makeProcess.waitForFinished();

    arguments.clear();
    arguments << "-j";
    arguments << ".text";
    arguments << "-j";
    arguments << ".data";
    arguments << "-O";
    arguments << "ihex";
    arguments << "/tmp/noise.elf";
    arguments << "/tmp/noise.hex";

    makeProcess.start(workingDir + "/avr/bin/avr-objcopy", arguments);
    makeProcess.waitForFinished();


    arguments.clear();
    arguments << "--format=avr";
    arguments << "--mcu=attiny85";
    arguments << "/tmp/noise.elf";

    makeProcess.start(workingDir + "/avr/bin/avr-size", arguments);
    makeProcess.waitForFinished();

    tmrAutoDetect.start();

}

void Dialog::SlotCheckNewPort()
{
    USBAspDetector babygnusb;
    QString workingDir= qApp->applicationDirPath() + "/../Resources";
    QStringList arguments;
    QProcess makeProcess;
    makeProcess.setWorkingDirectory(workingDir);

    bool connected = babygnusb.Detect();
    //avrdude -c usbasp -b 9600 -p attiny85 -C /avr/etc/avrdude.conf -U flash:w:noise.hex:i

    if (connected)
    {
        ui->label_3->setText("BABYGNUSB CONNECTED >> FLASHING");
        usleep(1000);

        tmrAutoDetect.stop();

        arguments.clear();        

        arguments << "-c";
        arguments << "usbasp";
        arguments << "-b";
        arguments << "9600";
        arguments << "-p";
        arguments << "attiny85";
        arguments << "-C";
        arguments << workingDir + "/avr/etc/avrdude.conf";
        arguments << "-U";
        arguments << "flash:w:/tmp/noise.hex:i";


        makeProcess.start(workingDir + "/avr/bin/avrdude", arguments);
        makeProcess.waitForFinished();
        ui->label_3->setText("BABYGNUSB CONNECTED >> FLASH OK");

    }else{
        ui->label_3->setText("DETECTING...");
    }
}

void Dialog::on_pushButton_4_clicked()
{
    m_audioOutput->stop();
}

void Dialog::on_pushButton_5_clicked()
{
    USBAspDetector BabyGnusbDetect;
    BabyGnusbDetect.Detect();
}

void Dialog::on_pushButton_2_clicked()
{
    m_device = QAudioDeviceInfo::defaultOutputDevice();
    m_audioOutput = 0;
   // m_format.setFrequency(4000);
    m_format.setChannels(1);
    m_format.setSampleSize(8);
    m_format.setSampleRate(8000);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    if(!n_generator)
    {
        n_generator = new NoiseGen(this);
        delete m_audioOutput;
        m_audioOutput = 0;
        m_audioOutput = new QAudioOutput(m_device, m_format, this);
        connect(ui->dial, SIGNAL(valueChanged(int)), n_generator,SLOT(Pot1(int)));
        connect(ui->dial_2, SIGNAL(valueChanged(int)), n_generator,SLOT(Pot2(int)));
        connect(ui->dial_3, SIGNAL(valueChanged(int)), n_generator,SLOT(Pot3(int)));
        connect(ui->dial_4, SIGNAL(valueChanged(int)), n_generator,SLOT(Pot4(int)));

        n_generator->formulaScript = ui->textEdit->toPlainText();
        //n_generator->formulaScript.replace("pot1", "%2", Qt::CaseInsensitive);
        //n_generator->formulaScript.replace("pot2", "%3", Qt::CaseInsensitive);
        //n_generator->formulaScript.replace("t", "%1", Qt::CaseInsensitive);

        n_generator->start();
        m_audioOutput->start(n_generator);

    }else{
        n_generator->formulaScript = ui->textEdit->toPlainText();
        //n_generator->formulaScript.replace("pot1", "%2", Qt::CaseInsensitive);
        //n_generator->formulaScript.replace("pot2", "%3", Qt::CaseInsensitive);
        //n_generator->formulaScript.replace("pot3", "%2", Qt::CaseInsensitive);
        //n_generator->formulaScript.replace("pot4", "%3", Qt::CaseInsensitive);

        //n_generator->formulaScript.replace("t", "%1", Qt::CaseInsensitive);

    }



}

void Dialog::on_pushButton_clicked()
{
    this->on_pushButton_3_clicked();
}

void Dialog::on_dial_3_valueChanged(int value)
{

}
