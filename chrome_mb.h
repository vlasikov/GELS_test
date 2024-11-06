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

signals:

private:
    bool connected;
};

#endif // CHROME_MB_H
