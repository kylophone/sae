#include <Common.h>
#include "ZeroCrossingRate.hpp"

namespace sae {

  ZeroCrossingRate::ZeroCrossingRate(uint16_t block_size) {
    _block_size = block_size;
    _historgram = new uint64_t[block_size - 1]();
  }

  ZeroCrossingRate::~ZeroCrossingRate() {
    delete []_historgram;
  }

  FeatureMap ZeroCrossingRate::process_frame(double *buf, uint16_t len) {
    FeatureMap block_features;

    uint16_t zcr = 0;
    for (uint16_t i = 0; i < len - 1; i++) {
      if ((buf[i] >= 0 && buf[i + 1] < 0) ||
          (buf[i] < 0 && buf[i + 1] >= 0)) {
        zcr++;
      }
    }

    _block_number++;

    block_features["value"] = zcr;
    _historgram[zcr]++;
    return block_features;
  };

  FeatureMap ZeroCrossingRate::analyze(void) {
    uint64_t sum = 0;
    for(uint16_t i = 0; i < _block_size - 1; i++) {
      sum += (i * (_historgram[i]));
    }
    double mean = ((double) sum / _block_number);

    uint64_t variance = 0;
    for(uint16_t i = 0; i < _block_size - 1; i++) {
      for(uint64_t j = 0; j < _historgram[i]; j++){
        variance += ((i - mean) * (i - mean));
      }
    }
    variance /= (_block_number * _block_size);
    double std_dev = sqrt(variance);

    _file_analysis["mean"] = mean;
    _file_analysis["std_dev"] = std_dev;
    return _file_analysis;
  };

}
