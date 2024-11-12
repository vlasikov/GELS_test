#ifndef CHROME_MB_H
#define CHROME_MB_H

#include <QObject>

class chrome_mb : public QObject
{
    Q_OBJECT
public:
    explicit chrome_mb(QObject *parent = nullptr);

    bool connect(char *IP_addr);
    void disconnect(void);
    int readParam(void);
    int writeParam(void);

    //ReadHoldingRegisters
    uint16_t RHRegSettings[32];
    uint16_t RHRegTract1_0_250[125];
    uint16_t RHRegTract1_250_354[52];
    uint16_t RHRegTract1_356[177];
    uint16_t RHRegID[10];

    uint16_t WSReg[64];         //Write Single Register
    uint16_t IReg[64];          //Input Register
    uint16_t Password;

signals:

private:
    bool connected;
};

#endif // CHROME_MB_H
