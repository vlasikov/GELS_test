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
/*
 * Размер MODBUS PDU ограничен значением, унаследованным от реализации MODBUS на основе проводной последовательной сети (максимум RS485 ADU = 256 байт).

Таким образом, MODBUS PDU для последовательной линии обмена = 256 - адрес сервера (1 байт) - CRC (2 байта) = 253 байта.

Следовательно:

RS232 / RS485 ADU = 253 байта + адрес сервера (1 байт) + CRC (2 байта) = 256 байт.
TCP MODBUS ADU = 253 байта + MBAP (7 байт) = 260 байт.
*/
int chrome_mb::readParam(void){
//    uint length_readed_registers;
    if (connected)  {
        int rc;
        modbus_set_slave(ctx, 1);
//  offset 0x0000
//  Настройки прибора
//        length_readed_registers=sizeof(RHRegSettings)/2;                                    //32;
        rc = modbus_read_registers(ctx, 0, sizeof(RHRegSettings)/2, RHRegSettings);
        if (-1 == rc) {
            qDebug()<<"error modbus_read_registers";
        }

//        rc = modbus_read_registers(ctx, 0, 5, RHRegSettings);
//        rc = modbus_read_registers(ctx, 5, 10, &RHRegSettings[5]);
//        qDebug()<<RHRegSettings;

//  offset 0x0100
//  Таблица градуировки изм. тракта №1
//  (8 газов, 10 концентраций)
//  356 байт = 178 слов
//        length_readed_registers=125;      // max 253 byte (125 words, 253 byte)
//        qDebug() <<sizeof (RHRegTract1_0_250);
        rc = modbus_read_registers(ctx, 0x0100, 125, RHRegTract1_0_356); // (modbas, offset, length, buff)
        if (-1 == rc) {
            qDebug()<<"RHRegTract1_0_249, error modbus_read_registers";
        }
        rc = modbus_read_registers(ctx, 0x0100+125, 52, &RHRegTract1_0_356[52]);               // (modbas, offset, length, buff)
        if (-1 == rc) {
            qDebug()<<"RHRegTract1_250_354, error modbus_read_registers";
        }

        return 1;
    }

    return 0;
}
