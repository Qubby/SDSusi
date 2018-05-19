/** @file tld_comm.hpp
 *  @brief Tald communication module
 *
 *  @author Margo Martis
 *  @date 30. April 2016
 *  @version 0.1
 */

#ifndef TLD_COMM_H
#define TLD_COMM_H

#include "BLEPeripheral.h"
#include "app.h"
#include "libtald.hpp"

using namespace libtald;

namespace libtald {
namespace comm {

struct comm_ctx_t {
  sensor_matrix_t data;
  uint32_t pcount, psize, pix;
};

enum comm_state_e { ECS_IDLE, ECS_SENDING };

void recv();
void send();

void init();
void process();

bool isAvailable();
}
}

#endif
