#ifndef NOISEGEN_H
#define NOISEGEN_H

#include <QObject>
#include <QDebug>
#include <QIODevice>
#include <QAudioOutput>
#include <QTimer>
#include <QByteArray>
#include <QScriptEngine>

#include <math.h>

class NoiseGen : public QIODevice
{
    Q_OBJECT
public:
    NoiseGen(QObject *parent);
    ~NoiseGen();

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;
    QString formulaScript;
    QString formulaCompiled;
    static QScriptValue addToBuffer(QScriptContext *context, QScriptEngine *engine);
    static QStringList audioBuffer;

private:
    QScriptEngine engine;

    qint64 m_pos;
    qint64 s_pos;
    int pot1;
    int pot2;
    int pot3;
    int pot4;

    QByteArray m_buffer;
public slots:
    void Pot1(int value);
    void Pot2(int value);
    void Pot3(int value);
    void Pot4(int value);
};

#endif // NOISEGEN_H
