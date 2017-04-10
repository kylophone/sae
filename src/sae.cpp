#include <Common.h>
#include <FeatureExtractors/Common.h>

#include <cxxopts.hpp>
#include <sndfile.h>

struct FeatureDataAsString
{
  std::string operator()(uint16_t x) { std::ostringstream out; out << x; return out.str(); }
  std::string operator()(uint64_t x) { std::ostringstream out; out << x; return out.str(); }
  std::string operator()(double x)   { std::ostringstream out; out.precision(3); out << std::fixed << x; return out.str(); }
};

int software_automatic_ear(std::string input_file, uint16_t block_size) {
  SNDFILE *sndfile;
  SF_INFO file_info;
  file_info.format = SF_FORMAT_RAW | SF_ENDIAN_LITTLE | SF_FORMAT_PCM_16;
  file_info.channels = 1;
  file_info.samplerate = 1;
  if (!(sndfile = sf_open(input_file.c_str(), SFM_READ, &file_info))) {
    std::cerr << "error: cannot open input file: `" << input_file << "'"
              << std::endl
              << sf_strerror(NULL)
              << std::endl;
    return 1;
  }

  double *buf = new (std::nothrow) double[block_size]();
  if (!buf) {
    std::cerr << "error: cannot allocate internal buffer"
              << std::endl;
    return 1;
  }

  /* register feature_extractors */
  std::vector<sae::FeatureExtractor*> feature_extractor;
  feature_extractor.push_back(new sae::MaxMagnitude(block_size));
  feature_extractor.push_back(new sae::ZeroCrossingRate(block_size));

  std::cout << "input_file: " << input_file << std::endl
            << "frame_count: " << file_info.frames << std::endl
            << "block_size: " << block_size << std::endl;

  /* extract block_features */
  uint64_t block_ctr = 0;
  while (uint16_t len = sf_read_double(sndfile, buf, block_size)) {
    std::cout << "block: "
              << block_ctr++
              << std::endl;

    for(sae::FeatureExtractor* fe : feature_extractor) {
      std::cout << "\t"
                << "feature: "
                << fe->name()
                << std::endl;

      sae::FeatureMap fm = fe->process_frame(buf, len);

      for (auto const &i : fm)
        std::cout << "\t\t"
                  << i.first
                  << ": "
                  << mpark::visit(FeatureDataAsString{}, i.second)
                  << std::endl;
    }
  }

  sf_close(sndfile);
  delete[] buf;

  /* file_analysis */
  std::cout << "file:"
            << std::endl;

  for(sae::FeatureExtractor* fe : feature_extractor) {
    std::cout << "\t"
              << "analyzer: "
              << fe->name()
              << "_statistics"
              << std::endl;

    sae::FeatureMap fm = fe->analyze();

    for (auto const &i : fm)
      std::cout << "\t\t"
                << i.first
                << ": "
                << mpark::visit(FeatureDataAsString{}, i.second)
                << std::endl;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  std::string input_file;
  uint16_t    block_size = 1024;

  cxxopts::Options options(argv[0], "Software Automatic Ear");
  try {
    options.add_options()
      ("i,input", "input file (16-bit little-endian PCM, mono)", cxxopts::value<std::string>())
      ("b,block_size", "block size (optional, default: 1024, valid: 1-65535)", cxxopts::value<uint16_t>())
      ("h,help", "show help")
      ;
    options.parse(argc, argv);

    if (options.count("help")) {
      std::cout << options.help()
                << std::endl;
      return 0;
    }

    if (!options.count("input")) {
      std::cerr << "error: input file required"
                << std::endl
                << std::endl
                << options.help()
                << std::endl;
      return 1;
    } else {
      input_file = options["input"].as<std::string>();
    }

    if (options.count("block_size")) {
      uint16_t bs = options["block_size"].as<uint16_t>();
      if (bs == 0) {
        std::cerr << "error: block_size must be greater than 0"
                  << std::endl
                  << std::endl
                  << options.help()
                  << std::endl;
        return 1;
      } else {
        block_size = bs;
      }
    }

  } catch(const cxxopts::OptionException &e) {
    std::cerr << "error parsing options: "
              << e.what()
              << std::endl
              << std::endl
              << options.help()
              << std::endl;
    return 1;
  }

  return software_automatic_ear(input_file, block_size);
}
