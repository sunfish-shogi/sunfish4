/* Worker.cpp
 *
 * Kubo Ryosuke
 */

#include "search/tree/Worker.hpp"

namespace sunfish {

void initializeWorker(Worker& worker) {
  initializeSearchInfo(worker.info);
}

} // namespace sunfish
