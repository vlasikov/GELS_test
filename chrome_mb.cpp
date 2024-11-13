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
    if (connected)  {
        int rc;
        modbus_set_slave(ctx, 1);

        //  Настройки прибора, offset 0x0000
        rc = modbus_read_registers(ctx, 0, sizeof(RHRegSettings)/2, RHRegSettings);
        if (-1 == rc) {
            qDebug()<<"error modbus_read_registers";
        }

        //  offset 0x0100, Таблица градуировки изм. тракта №1, (8 газов, 10 концентраций), 356 байт = 178 слов
        rc = modbus_read_registers(ctx, 0x0100, 125, RHRegTract1_356);                      // func(modbas*, offset, length, buff*)
        if (-1 == rc) {
            qDebug()<<"RHRegTract1_0_249, error modbus_read_registers()";
        }
        rc = modbus_read_registers(ctx, 0x0100+125, 52, &RHRegTract1_356[52]);              // func(modbas*, offset, length, buff*)
        if (-1 == rc) {
            qDebug()<<"RHRegTract1_250_354, error modbus_read_registers()";
        }

        //  offset 0x4000, Идентификатор устройства
        rc = modbus_read_registers(ctx, 0x4000, 10, RHRegID); // (modbas, offset, length, buff)
        if (-1 == rc) {
            qDebug()<<"RHRegID, error modbus_read_registers()";
        }

        return 1;
    }
    return 0;
}

int chrome_mb::writeParam(void){
    if (connected)  {
//        Значение пароля вычисляется из идентификатора микроконтроллера (сумма 6 регистров без учета переполнения):
//        uint16_t password = (uint16_t)(reg0 + reg1 + reg2 + reg3 + reg4 + reg5);

        int rc;
        rc = modbus_write_register(ctx, 0x04FF, Password);
        if (-1 == rc) {
            qDebug()<<"Password, error modbus_write_register()";
        }

//        word = 0xabcd;
//        rc = modbus_write_register(ctx, 0x0100, word);
//        if (-1 == rc) {
//            qDebug()<<"RHRegTract1_356, error modbus_write_registers()";
//        }

//        float f = 0.25;
//        uint32_t f_uint32 = uint32_t(f & 0xFFFF);
//        qDebug()<< f_uint32;

        float f = TableGraduation.Column1.valueConc[0];
        QByteArray array(reinterpret_cast<const char*>(&f), sizeof(f));
        qDebug()<<"array "<<array;
        float* out = reinterpret_cast<float*>(array .data());
        qDebug()<<"out "<< *out;

        // 1.1
        RHRegTract1_356[1] = array[2] | (array[3]>>24);
        RHRegTract1_356[0] = array[0] | (array[1]>>8);
        rc = modbus_write_registers(ctx, 0x0100, 2, RHRegTract1_356);
        if (-1 == rc) {
            qDebug()<<"RHRegTract1_356, error modbus_write_registers()";
        }
        return 1;
    }
    return 0;

}
