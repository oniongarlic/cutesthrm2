#ifndef STHRM2_H
#define STHRM2_H

#include <QObject>
#include <QDebug>
#include <QBluetoothSocket>

class STHrm2 : public QObject
{
    Q_OBJECT
public:
    explicit STHrm2(QObject *parent = 0);

    bool start();
    bool stop();

private slots:
    void readSocket();
    void connected();
    void disconnected();

signals:
    void batteryChanged(uint level);
    void hrChanged(uint hr);

private:
    void readByte();
    enum ReadStates {
        StateInitial,
        StateMarker1,
        StateMarker2,
        StateUnknown1,
        StateBattery,
        StateHRM,
        StateUnknown2,
        StateUnknown3
    };
    QBluetoothSocket *socket;
    ReadStates m_state;
    uint m_mark1;
    uint m_mark2;
    uint m_u1;
    uint m_hr;
    uint m_battery;
};

#endif // STHRM2_H
