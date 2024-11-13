#ifndef CHROME_MB_H
#define CHROME_MB_H

#include <QObject>

typedef struct _Column{
    uint32_t    valueChrome[10];
    float       valueConc[10];
} Column;

typedef struct _Window{
    uint16_t    left[10];
    uint16_t    right[10];
} Window;

typedef struct _Graduation{
    float valueConc[10];
    Column Column1, Column2, Column3, Column4, Column5, Column6, Column7, Column8;
    Window Window1, Window2, Window3, Window4, Window5, Window6, Window7, Window8;
} Graduation;

class chrome_mb : public QObject{
    Q_OBJECT
public:
    explicit chrome_mb(QObject *parent = nullptr);

    bool connect(char *IP_addr);
    void disconnect(void);
    int readParam(void);
    int writeParam(void);

    uint16_t    RHRegSettings[32];          //ReadHoldingRegisters

    uint16_t    RHRegTract1_356[177];       // что-то тут не скодится по числу байт
    Graduation  TableGraduation;

    uint16_t    RHRegID[10];

    uint16_t    WSReg[64];                  //Write Single Register
    uint16_t    IReg[64];                   //Input Register
    uint16_t    Password;

signals:

private:
    bool connected;
};


#endif // CHROME_MB_H
