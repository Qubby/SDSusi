/** @file libtald.hpp
 *  @brief Tald module implementation
 *
 *  @author Margo Martis
 *  @date 30. April 2016
 *  @version 0.1
 *  @updated 18.veb 2018
 */

#ifndef LIBTALD_H
#define LIBTALD_H

#include "app.h"
// CD demux
#define TLD_ADDR_OUT PortA
#define TLD_ADDR_OMASK 0xF0

// ADG mux
#define TLD_ADDR_IN PortC
#define TLD_ADDR_IMASK 0x0F

#define TLD_DATA_EN PA_1 // CD DEMUX)
#define TLD_DATA_IN PA_0 // ADG (MUX)

#define TLD_SMX_X 6 // 16
#define TLD_SMX_Y 6 // 16
#define TLD_SMX_L 256

namespace libtald {

// 3210 -> 1234
inline uint8_t addr_MUX(uint8_t ix) {
  uint8_t mask = 0, i = 0;
  while (i < 4) {
    mask <<= 1;
    mask |= (ix & 1);
    ix >>= 1;
    i++;
  }
  return mask;
}
// 6745 -> 1234
inline uint8_t addr_DeMUX(uint8_t ix) {
  uint8_t mask = ((ix >> 2) & 0b0011) | ((ix << 2) & 0b1100);

  return mask << 4;
}

typedef uint16_t sensor_matrix_t[16][16];

struct sample_t {
  sensor_matrix_t data;
  unsigned long int timestamp;
};

struct vec2u_t {
  unsigned int x, y;
};

struct bus_out_t {
  DigitalOut enableOut;
  PortOut port;
  uint8_t x;
};

struct bus_in_t {
  AnalogIn dataIn;
  PortOut port;
  uint8_t y;
};

struct tald_t {
  //  DeMUX Address bus.
  bus_out_t busOut;

  // MUX Address bus.
  bus_in_t busIn;

  void selectRow(unsigned int _x) {
    busOut.port = addr_DeMUX(_x);
    busOut.x = _x;

    busOut.enableOut = 1;
  }

  void selectColumn(unsigned int _y) {
    busIn.port = addr_MUX(_y);
    busIn.y = _y;
  }

  void select(unsigned int x, unsigned int y) {
    selectRow(x);
    selectColumn(y);
  }

  //  https://docs.mbed.com/docs/mbed-os-api/en/mbed-os-5.5/api/classmbed_1_1AnalogIn.html
  //  Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
  uint16_t read(unsigned int x, unsigned int y) {
    select(x, y);
    return busIn.dataIn.read_u16();
  }
};

void vTaldInit();
int iTaldSample(sensor_matrix_t &);

}
#endif
