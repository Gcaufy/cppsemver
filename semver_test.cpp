#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

#include "semver.h"

using namespace std;

struct TCase {
  string input;
  string expect;
};

static bool printIfFalse(const bool assertion, const string& msg) {
  if(!assertion) {
    cout << msg << std::endl;
  }
  return assertion;
}


int main() {

  TCase cases[] = {
    { "v1.2.3", "1.2.3" },
    { "1.2.3", "1.2.3" },
    { "1a.2.3", "" },
    { "  1.2.3  ", "" }, // will not trim space
    { "3", "3.0.0" },
    { "0", "0.0.0" },
    { "v1.2.3-alpha.01", "1.2.3-alpha.01" },
    { "v1.2.3-alpha.01   ", "1.2.3-alpha.01   " }, // keep spaces
    { "v1.2.3+alpha.02", "1.2.3-alpha.02" }, // +-. is connector
    { "v1.2.3abc", "1.2.3-abc" }, // connector can be not exist
    { "1.2", "1.2.0" }, // padding 0
    { "v1.2.abc", "" },
    { "v1.2\t.3", "" },
    { "v1.2-abc", "" },
    { "v1", "1.0.0" },
    { "v1.abc", "" },
    { "v1..abc", "" },
    { "..abc", "" },
    { ".abc", "", },
    { "...abc", "" },
    { ".....abc", "" },
    { "0000.00.00.00.000", "0.0.0-00.000" },
    { "v1.2.3.4.5", "1.2.3-4.5" },
    { "a1.2.3.4.5", "" },
    { "121.223.45-beta.1", "121.223.45-beta.1" },
    { "121.263.45-beta.1", "" },
  };

  for (auto c : cases) {
    semantic::Version v(c.input);
    stringstream s;
    s << " Input: " << c.input << "\nExpect: " << c.expect << "\nActual: " << v.Coerce();
    assert(printIfFalse(v.Coerce() == c.expect, s.str()));
  }

  semantic::Version v123("1.2.3");
  semantic::Version v221("2.2.1");
  semantic::Version v123b("1.2.3");
  semantic::Version v124("1.2.4");
  semantic::Version v103("1.0.3");
  semantic::Version v1103("11.0.3");

  assert(printIfFalse(v123.Compare(v221) == -1, "123 < 221"));
  assert(printIfFalse(v123.Compare(v123b) == 0, "123 == 123"));
  assert(printIfFalse(v123.Compare(v124) == -1, "123 < 124"));
  assert(printIfFalse(v123.Compare(v103) == 1, "123 > 103"));
  assert(printIfFalse(v123.Compare(v1103) == -1, "123 == 1103"));

  assert(printIfFalse(v123.Eq(v123b), "123 == 123"));
  assert(printIfFalse(v123.Lte(v123b), "123 <= 123"));
  assert(printIfFalse(v123.Lte(v124), "123 <= 124"));
  assert(printIfFalse(v123.Lt(v1103), "123 < 1103"));
  assert(printIfFalse(v123.Gt(v103), "123 > 103"));
  assert(printIfFalse(v123.Gte(v103), "123 >= 103"));
  assert(printIfFalse(v123.Gte(v123b), "123 >= 123"));
  assert(printIfFalse(v123.Eq(v123b), "123 == 123"));
  assert(printIfFalse(v123.Neq(v1103), "123 != 1103"));

  assert(printIfFalse(v123 == v123b, "123 == 123"));
  assert(printIfFalse(v123 <= v123b, "123 <= 123"));
  assert(printIfFalse(v123 <= v124, "123 <= 124"));
  assert(printIfFalse(v123 < v1103, "123 < 1103"));
  assert(printIfFalse(v123 > v103, "123 > 103"));
  assert(printIfFalse(v123 >= v103, "123 >= 103"));
  assert(printIfFalse(v123 >= v123b, "123 >= 123"));
  assert(printIfFalse(v123 == v123b, "123 == 123"));
  assert(printIfFalse(v123 != v1103, "123 != 1103"));

  cout << "All Test Pass" << endl;
}
