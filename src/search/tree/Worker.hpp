/* Worker.hpp
 *
 * Kubo Ryosuke
 */

#ifndef SUNFISH_SEARCH_TREE_WORKER_HPP__
#define SUNFISH_SEARCH_TREE_WORKER_HPP__

#include "search/SearchInfo.hpp"

namespace sunfish {

struct Worker {
  SearchInfo info;
};

void initializeWorker(Worker& worker);

} // namespace sunfish

#endif // SUNFISH_SEARCH_TREE_WORKER_HPP__
