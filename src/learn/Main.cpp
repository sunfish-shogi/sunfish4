/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "common/resource/Resource.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/util/SearchUtil.hpp"
#include "learn/batch/BatchLearning.hpp"
#include "learn/online/OnlineLearning.hpp"
#include "learn/training_data/TrainingData.hpp"
#include "learn/util/LearningUtil.hpp"
#include "logger/Logger.hpp"
#include <iostream>
#include <fstream>

namespace {
namespace resources {

const char* LearnLogPath = "res/strings/learn_log_path";

} // namespace resources
} // namespace

using namespace sunfish;

int main(int argc, char** argv, char**) {
  // initialize static objects
  CoreUtil::initialize();
  SearchUtil::initialize();

  // program options
  ProgramOptions po;
  po.addOption("summary", "m", "print summary of eval.bin");
  po.addOption("gen-td-csa", "csa", "generate training data file from CSA files");
  po.addOption("silent", "s", "silent mode");
  po.addOption("type", "t", "batch(default)|online", true);
  po.addOption("help", "h", "show this help");
  po.parse(argc, argv);

  // --help or -h
  if (po.has("help")) {
    std::cout << po.help();
    return 0;
  }

  // if '--silent' or '-s' is NOT specified.
  if (!po.has("silent")) {
    Loggers::error.addStream(std::cerr, ESC_SEQ_COLOR_RED, ESC_SEQ_COLOR_RESET);
    Loggers::warning.addStream(std::cerr, ESC_SEQ_COLOR_YELLOW, ESC_SEQ_COLOR_RESET);
    Loggers::info.addStream(std::cerr);
    Loggers::send.addStream(std::cerr, true, true, ESC_SEQ_COLOR_BLUE, ESC_SEQ_COLOR_RESET);
    Loggers::receive.addStream(std::cerr, true, true, ESC_SEQ_COLOR_MAGENTA, ESC_SEQ_COLOR_RESET);
  }
 
  // log file
  std::ofstream fout;
  auto logPath = Resource::string(resources::LearnLogPath, "");
  if (!logPath.empty()) {
    fout.open(logPath, std::ios::out | std::ios::app);

    Loggers::error.addStream(fout, true, true);
    Loggers::warning.addStream(fout, true, true);
    Loggers::info.addStream(fout, true, true);
    Loggers::send.addStream(fout, true, true);
    Loggers::receive.addStream(fout, true, true);
  }

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    MSG(warning) << "WARNING: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }

  if (po.has("summary")) {
    std::unique_ptr<Evaluator::FVType> fv(new Evaluator::FVType());
    if (!load(*fv)) {
      MSG(error) << "failed to load eval.bin";
      return 1;
    }
    LearningUtil::printFVSummary(fv.get());
    return 0;
  }

  if (po.has("gen-td-csa")) {
    auto& args = po.getStdArguments();
    if (args.size() != 2) {
      MSG(error) << "Usage: sunfish_ln --gen-td-csa SRC_DIR DST_FILE";
      return 1;
    }

    TrainingDataGenerator td;
    if (!td.loadCsaFiles(args[0])) {
      MSG(error) << "failed to load CSA files";
      return 1;
    }

    Random random;
    td.shuffle(random);

    if (!td.writeToFile(args[1])) {
      MSG(error) << "failed to write training data";
      return 1;
    }

    return 0;
  }

  if (po.getValue("type") == std::string("online")) {
    OnlineLearning online;
    return online.run() ? 0 : 1;

  } else if (!po.has("type") || po.getValue("type") == std::string("batch")) {
    BatchLearning batch;
    return batch.run() ? 0 : 1;

  } else {
    MSG(error) << "ERROR: invalid type: " << po.getValue("type");
  }
}
