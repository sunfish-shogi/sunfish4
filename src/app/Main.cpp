/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "app/Def.hpp"
#include "core/util/CoreUtil.hpp"
#include <iostream>

using namespace sunfish;

int main(int, char**, char**) {
  // initialize static objects
  CoreUtil::initialize();

  std::cerr << SUNFISH_NAME " version " SUNFISH_VERSION << '\n';
  std::cerr << SUNFISH_COPYRIGHT << '\n';
  std::cerr << std::endl;

  return 0;
}
