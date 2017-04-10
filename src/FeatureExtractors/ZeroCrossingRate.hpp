#pragma once

#include "FeatureExtractor.hpp"

namespace sae {

  class ZeroCrossingRate : public FeatureExtractor
  {
    private:
      FeatureMap _file_analysis = {
        {"mean",    (double) 0},
        {"std_dev", (double) 0}
      };

      uint64_t *_historgram;
      uint64_t _block_number = 0;
      uint16_t _block_size = 0;

    public:
      ZeroCrossingRate(uint16_t block_size);
      ~ZeroCrossingRate();
      const std::string name(void) { return "zero_crossing_rate"; };
      FeatureMap process_frame(double *buf, uint16_t len);
      FeatureMap analyze(void);
  };

}
