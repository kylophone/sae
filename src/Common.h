#pragma once

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <mpark/variant.hpp>

namespace sae
{

  using FeatureData = mpark::variant<uint16_t, uint64_t, double>;
  using FeatureMap  = std::map<std::string, FeatureData>;

}
