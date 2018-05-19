/** @file libtld.cpp
 *  @brief Tald module implementation
 *
 *  @author Margo Martis
 *  @date 30. April 2016
 *  @version 0.1
 */

#include "libtald.hpp"

namespace libtald {

tald_t gTald({{TLD_DATA_EN, {TLD_ADDR_OUT, TLD_ADDR_OMASK}, 0},
              {TLD_DATA_IN, {TLD_ADDR_IN, TLD_ADDR_IMASK}, 0}});

void vTaldInit() {}

int iTaldSample(sensor_matrix_t &smx) {
  uint16_t u16, ix;

  for (int x = 0; x < 6; x++) { //size_t
    for (int y = 0; y < 6; y++) {//size_t
      u16 = gTald.read(x, y);
      smx[x][y] = u16;
      //BLE jaoks
      // ix = y * 2;
      //
      // smx[x][ix] = u16 & 0xFF; //lowbit
      // smx[x][ix + 1] = (u16 >> 8) & 0xFF; //highbit
    }
  }
  return 0;
}
}
