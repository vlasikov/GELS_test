#include "chrome_mb.h"
#include <modbus/modbus.h>
#include <QDebug>

modbus_t *ctx;

chrome_mb::chrome_mb(QObject *parent) : QObject(parent){
  connected = false;
}

bool chrome_mb::connect(char *IP_addr){
  ctx = modbus_new_tcp(IP_addr, 502);
  if (modbus_connect(ctx) == -1) {
//    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    connected = false;
  }
  else
    connected = true;

  return connected;
}

void chrome_mb::disconnect(void){
  if (connected)  {
    connected = false;
    modbus_close(ctx);
    modbus_free(ctx);
  }
}

// чтение настроек с платы
QString chrome_mb::readParam(void){
    QString result;
    if (connected)  {
        qDebug()<<"readParam()";
        int rc;
        uint16_t tab_reg[64];
        modbus_set_slave(ctx, 1);
        const uint number_readed_registers=2;
        rc = modbus_read_registers(ctx, 0, number_readed_registers, tab_reg);
        qDebug()<<tab_reg[0];

        result = QString::number((tab_reg[0])&0xFF) + "." + QString::number((tab_reg[0]>>8)&0xFF) +"." + QString::number((tab_reg[1])&0xFF) + "." + QString::number((tab_reg[1]>>8)&0xFF);
        qDebug()<< result;
        qDebug()<< "IP = " <<((tab_reg[0])&0xFF)<<"."<<((tab_reg[0]>>8)&0xFF) <<((tab_reg[1])&0xFF)<<"."<<((tab_reg[1]>>8)&0xFF);

        if (rc == -1) {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
        }
    }
    return result;
}
