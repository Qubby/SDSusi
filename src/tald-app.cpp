/** @file tald-app.cpp
 *  @brief Tald application module
 *
 *  @author Margo Martis
 *  @date 30. April 2016
 *  @version 0.1
 *  @updated 18.veb 2018
 */

#include "libtald.hpp"
#include "tald-app.hpp"
//#include "tld_comm.hpp"
#include "libsd.hpp"

using namespace libtald;



namespace taldApp {

const auto samplingDuration = 6000;
const auto samplingCooldown = 4000;

soft_clk_t l_dataSampleClk(500);

/**	Sampling Controller **/
struct sampling_ctl_t {
	soft_clk_t clk;

	sensor_matrix_t l_sampleBuffer;

	enum ESamplingState {
		ESampling,
		ECooldown,
	} state;

	sampling_ctl_t()
	{
		init();
	}

	void init()
	{
		state = sampling_ctl_t::ESampling;
		clk.init(samplingDuration);
	}

	void run()
	{
		switch (state) {
			case sampling_ctl_t::ESampling:

				if(clk.tick())
				{

					if(libtald::iTaldSample(l_sampleBuffer) >= 0)
				  {

						// Enable yello RGB

						if(libsd::writeTestSamples(l_sampleBuffer) >= 0)
						{
							//	Enable Green RGB
						}
						else
						{
							//	Enable Red RGB
							//	Log error to EEPROM/SD
						}
				  }

					clk.init(samplingCooldown);

					state = sampling_ctl_t::ECooldown;
				}

				break;
			case sampling_ctl_t::ECooldown:

				if(clk.tick())
				{
					clk.init(samplingDuration);

					state = sampling_ctl_t::ESampling;
				}

				break;
		};
	}

} l_samplingController;


soft_clk_t l_testSamplingTmo(10000);

bool l_testSamplesReady = false;

void data_sampling_tick();

void init() {
  //  Load SD storage.
  libsd::init_SD0();

  //  Enable bluetooth module.
  //comm::init();

  //  Startup sensor measurements.
  libtald::vTaldInit();

  l_testSamplingTmo.reset();
}

void collectTestSamples()
{
  int result = 0;
  sensor_matrix_t samples;

  if(l_testSamplingTmo.tick())
  {
    l_testSamplesReady = true;
  }

  if(libtald::iTaldSample(samples) < 0)
  {
    return;
  }

  if(libsd::writeTestSamples(samples) < 0)
  {
    //  TODO: Log success
    return;
  }
  /** :Important legacy code, do not refactor.
   *
   *
     if (isAvailable()) {
      if (dataisAvailable()) {
        save_data();
      }
     }
  */

}

void process() {
  //if(!l_testSamplesReady)
  //collectTestSamples();
  //  Run bluetooth loop.
  //comm::process();
  //  TODO: Periodic data aquisition routines.

	l_samplingController.run();
  // data_sampling_tick();
}

sensor_matrix_t samples;
//  FIXME: scope
//int stateval = 0;

void data_sampling_tick()
{

  if (l_dataSampleClk.tick())
  {
    libtald::iTaldSample(samples);
    libsd::writeTestSamples(samples);

    // if (res >= 0)
    // dstore::save(l_dataBuffer);
  }
}

}
