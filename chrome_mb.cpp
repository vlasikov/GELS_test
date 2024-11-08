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
int chrome_mb::readParam(void){
    if (connected)  {
        int rc;
        modbus_set_slave(ctx, 1);
        const uint number_readed_registers=32;
        rc = modbus_read_registers(ctx, 0, number_readed_registers, RHReg);
        if (-1 == rc) {
            qDebug()<<"error modbus_read_registers";
        }
//        if (rc == -1) {
//            fprintf(stderr, "%s\n", modbus_strerror(errno));
//        }
        return 1;
    }
    return 0;
}
