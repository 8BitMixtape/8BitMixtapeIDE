#ifndef USBASPDETECTOR_H
#define USBASPDETECTOR_H

#include <QProcess>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/USBSpec.h>

class USBAspDetector : public QObject
{
    Q_OBJECT
public:
    USBAspDetector(QObject *parent = 0);
    bool Detect();
private:
    QProcess *listUSBasp;
};

#endif // USBASPDETECTOR_H
