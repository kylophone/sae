#pragma once

#include <Common.h>

namespace sae {

  class FeatureExtractor
  {
    public:
      virtual ~FeatureExtractor(){};
      virtual const std::string name(void) = 0;
      virtual FeatureMap process_frame(double *buf, uint16_t len) = 0;
      virtual FeatureMap analyze(void) = 0;
  };

}
