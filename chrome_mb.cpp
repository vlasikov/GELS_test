#include "chrome_mb.h"
#include <modbus/modbus.h>

modbus_t *ctx;

chrome_mb::chrome_mb(QObject *parent) : QObject(parent)
{
  connected = false;
}

bool chrome_mb::connect(char *IP_addr)
{
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

void chrome_mb::disconnect(void)
{
  if (connected)
  {
    connected = false;
    modbus_close(ctx);
    modbus_free(ctx);
  }
}
