#include "usbaspdetector.h"
#include <QDebug>

USBAspDetector::USBAspDetector(QObject *parent): QObject(parent)
{
    listUSBasp = new QProcess(this);
}

bool USBAspDetector::Detect()
{
    bool detected;
    //listUSBasp->start("sh", QStringList() << "-c" << "system_profiler SPUSBDataType | grep USBasp");
    //listUSBasp->waitForFinished();
    //QByteArray result = listUSBasp->readAllStandardOutput();
    //qDebug() << result;
    //listUSBasp->close();
    CFMutableDictionaryRef matchingDictionary = NULL;
    SInt32 idVendor = 0x16c0; // set vendor id
    SInt32 idProduct = 0x05dc; // set product id
    io_iterator_t iterator = 0;
    io_service_t usbRef;

    matchingDictionary = IOServiceMatching(kIOUSBDeviceClassName);

    CFDictionaryAddValue(matchingDictionary,CFSTR(kUSBVendorID), CFNumberCreate(kCFAllocatorDefault,kCFNumberSInt32Type, &idVendor));
    CFDictionaryAddValue(matchingDictionary,CFSTR(kUSBProductID),CFNumberCreate(kCFAllocatorDefault,kCFNumberSInt32Type, &idProduct));
    IOServiceGetMatchingServices(kIOMasterPortDefault,matchingDictionary, &iterator);

    usbRef = IOIteratorNext(iterator);

    if (usbRef == 0)
    {
        //qDebug() << "device not found";
        detected = false;
    }else{
        //qDebug() << "device found";
        detected = true;
    }

    IOObjectRelease(iterator);
    IOObjectRelease(usbRef);

    return detected;
}
