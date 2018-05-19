
#include "SDFileSystem.h"

#include "libsd.hpp"

struct sd_fs_pinout_t { PinName MOSI, MISO, CLK, CS; };

// Static global SPI interface pinout.
const sd_fs_pinout_t SPI0 = { PB_15, PB_14, PB_13, PB_12 };

namespace libsd {

DigitalOut gLED_yellow(PC_4);
DigitalOut gLED_green(PC_5);


SDFileSystem gSD0(SPI0.MOSI, SPI0.MISO, SPI0.CLK, SPI0.CS, "SD0");

bool l_initFlag = false;

void notify_warn(int n) {
  gLED_yellow = 0;

  int stateval=0;

  for(int i=0; i<(n*2); ++i) {
    stateval=!stateval;
    gLED_yellow = stateval;
    wait_ms(200);
  }

  gLED_yellow = 0;
}

int _writeTestSamples_debug(const libtald::sensor_matrix_t& sensorData)
{
  // printf("Hello World!\n");

  gLED_yellow = 1;

  mkdir("/SD0/mydir", 0777);

  FILE *fp = fopen("/SD0/mydir/sdtest.txt", "w");

  if(fp == NULL)
  {
    // error("Could not open file for write\n");
    notify_warn(3);
    return -1;
  }
  else
  {
    fprintf(fp, "Hello fun SD Card World!");
    fclose(fp);
  }

  gLED_yellow = 0;
  gLED_green = 1;

  // printf("Goodbye World!\n");
  return 0;
}

int _writeTestSamples(const libtald::sensor_matrix_t& sensorData) {
  if(!l_initFlag) return -1;

	gLED_green = 0;
  gLED_yellow = 1;

  mkdir("/SD0/mydir", 0777);

  FILE *fp = fopen("/SD0/mydir/sdtest.txt", "a");

  if(fp == NULL) {
    notify_warn(3);

    return -2;
  }

  uint16_t u16;
  //uint16_t u8_low, u8_high;

  const int Sample_N = 6;
  const char* Sample_Format = "%u,"; // "%.3F,";


  //DatatoOutput
  for (int x = 0; x < Sample_N; x++) {
    for (int y = 0; y < Sample_N; y++) {
      u16 = sensorData[y][x];
      fprintf(fp,Sample_Format, u16);
    }
  }

  //  fprintf(fp, "%lu", sample.timestamp);
  fprintf(fp, Sample_Format, millis());

  gLED_yellow = 0;
  gLED_green = 1;

  fprintf(fp, "\n");
  fclose(fp);

  return 0;
}

int writeTestSamples(const libtald::sensor_matrix_t& sensorData) {
  return _writeTestSamples(sensorData);

	// return _writeTestSamples_debug(sensorData);
}

/**
*/
int init_SD0() {
  //  Initialize SD connection.
  int result = 0; //  FIXME:  = gSD0.begin();

  if(!result) {
    //  TODO: Log error.
  }

  l_initFlag = true;

}
}
