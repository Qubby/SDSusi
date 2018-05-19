#include "app.h"
#include "SDFileSystem.h"
#include "tald-app.hpp"

void millisStart(void) {
  SysTick_Config(SystemCoreClock / 1000);
}

int main() {
    millisStart();

    taldApp::init();

    while(1) {
      taldApp::process();

    }
}

volatile unsigned long _millis;
extern "C" void SysTick_Handler(void) { _millis++; }
unsigned long millis(void) { return _millis; }
