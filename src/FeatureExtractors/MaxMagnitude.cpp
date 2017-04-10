#include <Common.h>
#include "MaxMagnitude.hpp"

namespace sae {

  MaxMagnitude::MaxMagnitude(uint16_t block_size) {
    _block_size = block_size;
  }

  FeatureMap MaxMagnitude::process_frame(double *buf, uint16_t len) {
    double max_magnitude = 0.;
    uint16_t block_index = 0;

    for (uint16_t i = 0; i < len; i++) {
      double magnitude = std::abs(buf[i]);
      if (magnitude > max_magnitude) {
        max_magnitude = magnitude;
        if (max_magnitude > mpark::get<double>(file_analysis["max_value"])) {
          file_analysis["max_value"]    = (double) max_magnitude;
          file_analysis["block_index"]  = (uint16_t) i;
          file_analysis["block_number"] = (uint64_t) _block_number;
        }
        block_index = i;
      }
    }

    _block_number++;

    FeatureMap block_features;
    block_features["value"] = max_magnitude;
    block_features["block_index"] = block_index;
    return block_features;
  };

  FeatureMap MaxMagnitude::analyze(void) {
    file_analysis["file_index"] = (uint64_t) ((mpark::get<uint64_t>(file_analysis["block_number"])
          * _block_size)
          + (mpark::get<uint16_t>(file_analysis["block_index"])));
    return file_analysis;
  };

}
