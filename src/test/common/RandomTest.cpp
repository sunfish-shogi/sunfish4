/* RandomTest.cpp
 *
 * Kubo Ryosuke
 */

#include "test/Test.hpp"
#include "common/math/Random.hpp"

using namespace sunfish;

TEST(RandomTest, testShuffle) {
  Random random;

  {
    std::array<int, 1> a = { 0 };
    random.shuffle(a.begin(), a.end());
    ASSERT_EQ(0, a[0]);
  }

  {
    std::array<int, 2> a = { 0, 1 };
    random.shuffle(a.begin(), a.end());
    ASSERT_TRUE((a[0] == 0 && a[1] == 1) || (a[0] == 1 && a[1] == 0));
  }

  {
    std::array<int, 10> a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int c[10] = { 0 };
    random.shuffle(a.begin(), a.end());
    for (unsigned i = 0; i < a.size(); i++) {
      c[a[i]]++;
    }
    for (unsigned i = 0; i < a.size(); i++) {
      ASSERT_EQ(1, c[i]);
    }
  }
}
