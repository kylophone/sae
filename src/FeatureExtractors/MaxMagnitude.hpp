#pragma once

#include "FeatureExtractor.hpp"

namespace sae {

  class MaxMagnitude : public FeatureExtractor
  {
    private:
      FeatureMap file_analysis = {
        {"max_value",    (double)   0},
        {"block_number", (uint64_t) 0},
        {"block_index",  (uint16_t) 0},
        {"file_index",   (uint64_t) 0}
      };

      uint64_t _block_number = 0;
      uint16_t _block_size   = 0;

    public:
      MaxMagnitude(uint16_t block_size);
      const std::string name(void) { return "max_magnitude"; };
      FeatureMap process_frame(double *buf, uint16_t len);
      FeatureMap analyze(void);
  };

}
