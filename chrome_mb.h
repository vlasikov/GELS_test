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

    uint16_t RHReg[64];       //ReadHoldingRegisters
    uint16_t WSReg[64];       //Write Single Register
    uint16_t  IReg[64];       //Input Register

signals:

private:
    bool connected;
};

#endif // CHROME_MB_H
