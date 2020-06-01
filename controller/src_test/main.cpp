#include "debug.h"
#include "hal.h"
#include "uart_dma.h"
#include <string.h>

char r[20];

class DummyTxListener : public TxListener {
  void onTxComplete() override { debug.Print("$"); }
  void onTxError() override { debug.Print("E"); };
};

class DummyRxListener : public RxListener {
public:
  void onRxComplete() override {
    debug.Print("&");
    debug.Print(r);
  }
  void onCharacterMatch() override { debug.Print("@"); }
  void onRxError(RxError_t e) override {
    if (RX_ERROR_TIMEOUT == e) {
      debug.Print("T");
    } else {
      debug.Print("#");
    };
  }
};

DummyRxListener rxlistener;
DummyTxListener txlistener;

DMACtrl dma_ctrl_test(DMA1_BASE);

constexpr uint8_t txCh = 1;
constexpr uint8_t rxCh = 2;
UART_DMA uart_dma_test(UART3_BASE, DMA1_BASE, txCh, rxCh, '.');

int main() {
  Hal.init();
  dma_ctrl_test.init();

  debug.Print("*");
  char s[] = "ping ping ping ping ping ping ping ping ping ping ping ping\n";
  bool dmaStarted = false;

  dmaStarted = uart_dma_test.startTX((uint8_t *)s, strlen(s), 0);
  if (dmaStarted) {
    debug.Print("!");
  }

  uart_dma_test.charMatchEnable();

  dmaStarted = uart_dma_test.startRX((uint8_t *)r, 10, seconds(2), 0);
  if (dmaStarted) {
    debug.Print("!");
  }

  while (1) {
    Hal.watchdog_handler();
    // char i[1];
    // if (1 == debugRead(i, 1)) {
    //   Hal.reset_device();
    // }
    Hal.delay(milliseconds(10));
  }
}
