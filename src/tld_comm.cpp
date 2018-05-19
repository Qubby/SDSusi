/** @file tld_comm.cpp
 *  @brief Tald communication module implementation
 *
 *  @author Margo Martis
 *  @date 30. April 2016
 *  @version 0.1
 */

#include "tld_comm.hpp"

Serial serial(USBTX, USBRX); // TX,RX
//SPI spi(PB_5, PB_4, PB_3);

namespace libtald {
namespace comm {

DigitalInOut BLE_RDY(PB_6);
DigitalInOut BLE_REQ(PA_15);

BLEPeripheral *l_pBLEPeripheral;

BLEService uartService = BLEService("713d0001503e4c75ba943148f18d941e");

BLECharacteristic txCharacteristic =
    BLECharacteristic("713d0002503e4c75ba943148f18d941e", BLENotify, 20);
BLECharacteristic rxCharacteristic =
    BLECharacteristic("713d0003503e4c75ba943148f18d941e", BLEWrite, 20);

comm_state_e g_send_st;
comm_ctx_t g_ctx;

bool BLEisAvailable = false;

// void debug() {
//   if (g_ctx.data[0][0] > 128) {
//     // led = 0;
//   } else {
//     // led = 1;
//   }
// }

bool isAvailable() { return BLEisAvailable; }

int create_data_post() {
  // FIXME: load data from storage instead
  // if(read_sensor_matrix(g_ctx.data) < 0) return -1;

  // debug();

  g_ctx.psize = 12; // TLD_SMX_Y;  // Packet length
  g_ctx.pcount = 1; // TLD_SMX_X; // Packet count
  g_ctx.pix = 0;

  iTaldSample(g_ctx.data);

  return 0;
}

int process_data_post() {
  l_pBLEPeripheral->poll();

  while (l_pBLEPeripheral->_nRF8001._aciState.data_credit_available > 0) {
    if (g_ctx.pix < g_ctx.pcount) {
      txCharacteristic.setValue(&(g_ctx.data[0][0]), g_ctx.psize);
      g_ctx.pix++;
    } else
      return 1;

    l_pBLEPeripheral->poll();
  }

  if (g_ctx.pix >= g_ctx.pcount)
    return 1;

  return 0;
}

void send() {
  int res;
  switch (g_send_st) {
  case ECS_IDLE:
    res = create_data_post();
    if (res >= 0)
      g_send_st = ECS_SENDING;
    break;
  case ECS_SENDING:
    res = process_data_post();
    if (res != 0)
      g_send_st = ECS_IDLE;
    break;

  default:
    break;
  }
}

void init() {
  DigitalOut RESET_BLE(PD_2);
  RESET_BLE = 0;
  wait_us(0.5);
  RESET_BLE = 1;

  //  Allocate on heap instead.
  l_pBLEPeripheral = new BLEPeripheral(&BLE_REQ, &BLE_RDY, NULL);

  l_pBLEPeripheral->setLocalName("NRF8001");
  l_pBLEPeripheral->setAdvertisedServiceUuid("713d0000503e4c75ba943148f18d941e");

  l_pBLEPeripheral->addAttribute(uartService);

  l_pBLEPeripheral->addAttribute(rxCharacteristic);
  l_pBLEPeripheral->addAttribute(txCharacteristic);

  l_pBLEPeripheral->begin();
  g_send_st = ECS_IDLE;

  BLEisAvailable = false;
}

void process() {
  BLECentral central = l_pBLEPeripheral->central();
  if (central) {
    if (central.connected()) {
      send();
    }
  }
}
}
}
