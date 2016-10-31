#include <QBluetoothAddress>

#include "sthrm2.h"

//#define DEBUG_DATA 1

STHrm2::STHrm2(QObject *parent) : QObject(parent), m_battery(0)
{
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

bool STHrm2::start()
{
    m_state=StateInitial;
    socket->connectToService(QBluetoothAddress("FF:0B:AE:1C:A3:E0"), 1);
    qDebug("Connecting...");

    return true;
}

bool STHrm2::stop()
{
    socket->disconnectFromService();
}

void STHrm2::readSocket()
{
#ifdef DEBUG_DATA
    QByteArray data=socket->readAll();
    qDebug() << data.toHex();
#else
    while (socket->bytesAvailable())
           readByte();
#endif
}

void STHrm2::readByte()
{
    quint8 c=0;
    char tmp;
    bool r=socket->getChar(&tmp);

    if (!r) {
        qWarning("getChar failed");
        return;
    }

    c=(quint8)tmp;

    qDebug() << "C: " << QByteArray::number(c, 16) << " @: " << m_state;

    switch (m_state) {
    // Looking for start message byte 0xFA
    case StateInitial:
        if (c==0xFA) {
            // We got start marker, go to next state
            m_state=StateMarker1;
        } else {
            return;
        }
        break;
    case StateMarker1:
        m_mark1=c;
        m_state=StateMarker2;
        break;
    case StateMarker2:
        m_mark2=c;
        if (m_mark1+m_mark2!=0xFF) {
            qWarning("Invalid marker sum");
            m_state=StateInitial;
            return;
        }
        m_state=StateUnknown1;
        break;
    case StateUnknown1:
        m_state=StateBattery;
        m_u1=c;
        break;
    case StateBattery:
        // Battery level ?
        if (m_u1==0x81 && c!=m_battery) {
            m_battery=c;
            qDebug() << "Battery level: " << m_battery;
            emit batteryChanged(m_battery);
        }
        m_state=StateHRM;
        break;
    case StateHRM:
        m_hr=c;
        qDebug() << "HR: " << m_hr;
        emit hrChanged(m_hr);
        // Ok, we got what we need, wait for initial marker. We ignore anything else for now
        m_state=StateInitial;
        break;
    case StateUnknown2:
        m_state=StateUnknown3;
        break;
    case StateUnknown3:
        m_state=StateHRM;
        break;
    }
}

void STHrm2::connected()
{
    qDebug() << "Connected to device!";
}

void STHrm2::disconnected()
{
    qDebug() << "Disconnected";
}


