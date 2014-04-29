#include "noisegen.h"

NoiseGen::NoiseGen(QObject *parent): QIODevice(parent), m_pos(0), s_pos(0), pot1(0), pot2(0),  pot3(0), pot4(0)
{
    QScriptValue fun = engine.newFunction(addToBuffer);
    engine.globalObject().setProperty("addToBuffer", fun);
}

NoiseGen::~NoiseGen()
{

}

void NoiseGen::start()
{
    open(QIODevice::ReadOnly);
}

void NoiseGen::stop()
{
    m_pos = 0;
    close();
}

qint64 NoiseGen::readData(char *data, qint64 len)
{
    char mybuf[len];
    int i = 0;

    QString audioGen;
QString formulaCompiled;

formulaCompiled = formulaScript;
formulaCompiled.replace("pot1", QString::number(pot1), Qt::CaseInsensitive);
formulaCompiled.replace("pot2", QString::number(pot2), Qt::CaseInsensitive);
formulaCompiled.replace("pot3", QString::number(pot3), Qt::CaseInsensitive);
formulaCompiled.replace("pot4", QString::number(pot4), Qt::CaseInsensitive);

    audioGen = "for (t = %0; t < (%0+%1);t++) { addToBuffer(%2); }";
    engine.evaluate(audioGen.arg(s_pos).arg(len).arg(formulaCompiled));

    //qDebug() << formulaScript.arg(pot1).arg(pot2).arg(pot3);
    //return 0;
    for(int t=0 ;t< (len)  ;t++)
       {
        //formulaStream = formulaCompiled.;
        //((t*("36364689"[t>>13&7]&15))/12&128)+(((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127)
        //((t*("36364689"[t>>13&7]&15))/12&128)
 //       +(((((t>>pot1)^(t>>12)-2)%11*t)/4|t>>pot2)&127)
    //  sprintf(&mybuf[i], "%c",  ((t*("36364689"[t>>13&7]&15))/12&128)        +(((((t>>pot1)^(t>>12)-2)%11*t)/4|t>>pot2)&127));
             //sprintf(&mybuf[i], "%c", ((t*("36364689"[t>>13&7]&15))/12&128)        +(((((t>>pot1)^(t>>12)-2)%11*t)/4|t>>pot2)&127) );
            // sprintf(&mybuf[i], "%c", ((t*("454545"[t>>pot3&7]&15))/12&128)        +(((((t>>pot1)^(t>>12)-2)%11*t)/4|t>>pot2)&127) );

        //     sprintf(&mybuf[i], "%c", ((t*("36364689"[t>>13&7]&15))/12&128)+(((((t>>pot1)^(t>>12)-2)%11*t)/4|t>>pot2)&127) );

  //      sprintf(&mybuf[i], "%c", (t*9&t>>pot2|t*5&t>>pot1|t*3&t/1024)-1  );
       // char test = (char) engine.evaluate(formulaScript.arg(t).arg(pot1).arg(pot2)).toUInt32();
        sprintf(&mybuf[i], "%c", audioBuffer[i].toInt() );
       // qDebug() << mybuf[i];
//qDebug() << audioBuffer[i];
        //qDebug() << formulaScript.arg(t).arg(pot1).arg(pot2);
          //qDebug() << (QString::number(t) + "<<" + QString::number(pot1));

           i++;
       }

    NoiseGen::audioBuffer.clear();

    memcpy(data, mybuf, len);
    s_pos += len;
    return len;
}

QStringList NoiseGen::audioBuffer;

QScriptValue NoiseGen::addToBuffer(QScriptContext *context, QScriptEngine *engine)
{
   QScriptValue a = context->argument(0);
   NoiseGen::audioBuffer << a.toString();
   return 0;
}

qint64 NoiseGen::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    return 0;
}

qint64 NoiseGen::bytesAvailable() const
{
    return 8000 + QIODevice::bytesAvailable();
}


void NoiseGen::Pot1(int val)
{
    pot1 = val;
}

void NoiseGen::Pot2(int val)
{
    pot2 = val;
}

void NoiseGen::Pot3(int val)
{
    pot3 = val;
}

void NoiseGen::Pot4(int val)
{
    pot4 = val;
}
