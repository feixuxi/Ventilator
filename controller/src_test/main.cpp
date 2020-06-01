#include "debug.h"
#include "hal.h"
#include "hal_stm32.h"
#include "uart_dma.h"
#include <string.h>

char r[20];

extern UART_DMA dmaUART;

class DummyTxListener : public UART_DMA_TxListener {
  void onTxComplete() { debug.Print("$"); }
  void onTxError(){};
};

class DummyRxListener : public UART_DMA_RxListener {
public:
  void onRxComplete() {
    debug.Print("&");
    debug.Print(r);
  }
  void onCharacterMatch() { debug.Print("@"); }
  void onRxError(RxError_t e) {
    if (RX_ERROR_TIMEOUT == e) {
      debug.Print("T");
    } else {
      debug.Print("#");
    };
  }
};

// FramingRxFSM listener = FramingRxFSM();
DummyRxListener rxlistener;
DummyTxListener txlistener;

DMACtrl dmaController(DMA1_BASE);
constexpr uint8_t txCh = 1;
constexpr uint8_t rxCh = 2;
UART_DMA dmaUART(UART3_BASE, DMA1_BASE, txCh, rxCh, rxlistener, txlistener,
                 '.');

int main() {
  Hal.init();
  dmaController.init();

  debug.Print("*");
  char s[] = "ping ping ping ping ping ping ping ping ping ping ping ping\n";
  bool dmaStarted = false;

  dmaStarted = dmaUART.startTX(s, strlen(s));
  if (dmaStarted) {
    debug.Print("!");
  }

  dmaUART.charMatchEnable();

  dmaStarted = dmaUART.startRX(r, 10, 115200 * 2);
  if (dmaStarted) {
    debug.Print("!");
  }

  while (1) {
    Hal.watchdog_handler();
    // char i[1];
    // if (1 == debug.Read(i, 1)) {
    //   Hal.reset_device();
    // }
    Hal.delay(milliseconds(10));
  }
}
