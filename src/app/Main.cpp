/* Main.cpp
 *
 * Kubo Ryosuke
 */

#include "app/Def.hpp"
#include <iostream>

int main(int, char**, char**) {
  std::cerr << SUNFISH_NAME " version " SUNFISH_VERSION << '\n';
  std::cerr << SUNFISH_COPYRIGHT << '\n';
  std::cerr << std::endl;

  return 0;
}
