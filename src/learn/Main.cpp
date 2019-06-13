/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "common/console/Console.hpp"
#include "common/program_options/ProgramOptions.hpp"
#include "common/resource/Resource.hpp"
#include "core/util/CoreUtil.hpp"
#include "search/eval/Evaluator.hpp"
#include "search/eval/FeatureTemplates.hpp"
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
  po.addOption("type", "t", "batch(default)|online", true);
  po.addOption("silent", "s", "silent mode");
  po.addOption("summary", "m", "print value summary of eval-ex.bin");
  po.addOption("gen-td-csa", "csa", "generate training data file from CSA files");
  po.addOption("optimize", "o", "convert from expanded FV(eval-ex.bin) to optimized FV(eval.bin)");
  po.addOption("merge", "g", "merge expanded FV files");
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

  // invalid arguments
  for (const auto& invalidArgument: po.getInvalidArguments()) {
    MSG(error) << "ERROR: "  << invalidArgument.reason << ": `" << invalidArgument.arg << "'";
  }
  if (!po.getInvalidArguments().empty()) {
    return 1;
  }

  if (po.has("summary")) {
    std::unique_ptr<Evaluator::FVType> fv(new Evaluator::FVType());
    if (!load(*fv)) {
      MSG(error) << "failed to load eval-ex.bin";
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
    if (!td.appendCsaFiles(args[0])) {
      MSG(error) << "failed to find CSA files";
      return 1;
    }

    if (!td.writeToFile(args[1])) {
      MSG(error) << "failed to write training data";
      return 1;
    }

    return 0;
  }

  if (po.has("optimize")) {
    std::unique_ptr<Evaluator::FVType> fv(new Evaluator::FVType());
    std::unique_ptr<Evaluator::OFVType> ofv(new Evaluator::OFVType());
    if (!load(*fv)) {
      MSG(error) << "failed to load eval-ex.bin";
      return 1;
    }
    optimize(*fv, *ofv);
    if (!save(*ofv)) {
      MSG(error) << "failed to save eval.bin";
      return 1;
    }
    return 0;
  }

  if (po.has("merge")) {
    auto& args = po.getStdArguments();
    if (args.size() < 2) {
      MSG(error) << "Usage: sunfish_ln --merge FILE1 FILE2 [FILE3 ...]";
      return 1;
    }

    std::unique_ptr<FeatureVector<float>> mfvf(new FeatureVector<float>());
    memset(reinterpret_cast<void*>(mfvf.get()), 0, sizeof(Evaluator::FVType));

    std::unique_ptr<Evaluator::FVType> fv(new Evaluator::FVType());
    for (auto& arg : args) {
      if (arg == "eval-ex.bin") { // TODO: magic number
        MSG(error) << "you can not specify eval-ex.bin as a source file name";
        return 1;
      }
      if (!load(arg.c_str(), *fv)) {
        MSG(error) << "failed to load " << arg;
        return 1;
      }
      each(*mfvf, *fv, [](float& m, int16_t& f) {
        m += f;
      });
    }

    std::unique_ptr<Evaluator::FVType> mfv(new Evaluator::FVType());
    each(*mfv, *mfvf, [&args](int16_t& f, float& m) {
      f = int16_t(m / float(args.size()) + 0.5f);
    });
    if (!save(*mfv)) {
      MSG(error) << "failed to save eval-ex.bin";
      return 1;
    }

    return 0;
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

  if (po.getValue("type") == std::string("online")) {
#if !MATERIAL_LEARNING_ONLY
    OnlineLearning online;
    return online.run() ? 0 : 1;
#else // MATERIAL_LEARNING_ONLY
    LOG(error) << "online learning is not supported in MATERIAL_LEARNING_ONLY mode.";
    return 1;
#endif

  } else if (!po.has("type") || po.getValue("type") == std::string("batch")) {
    BatchLearning batch;
    return batch.run() ? 0 : 1;

  } else {
    MSG(error) << "ERROR: invalid type: " << po.getValue("type");
  }
}
