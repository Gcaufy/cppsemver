#include <iostream>
#include <string>
#include <cstdint>

namespace semantic {

// version max string length is 255.
// each version is one char.
constexpr uint32_t kVersionMaxLen = 255;

// identifier_ connector, can be -/+
// - is the default connector
// v1.2.3-alpha.0
constexpr char kAllowIdentifierConnector[] = "-+.";

class Version {
 public:
  unsigned char major_ = '\0';
  unsigned char minor_ = '\0';
  unsigned char patch_ = '\0';
  std::string identifier_;

  Version(const std::string& v) {
    original_ = v;
    this->_Parse();
  }

  std::string Coerce() {
    if (!this->IsValid()) return "";
    std::stringstream s;
    s << static_cast<int>(major_) << '.' << static_cast<int>(minor_) << '.' << static_cast<int>(patch_);
    if (identifier_.length() > 0) {
      s << kAllowIdentifierConnector[0] << identifier_;
    }
    return s.str();
  }

  bool IsValid() {
    return valid_;
  }
  /**
  * greater return 1
  * lesser return -1
  * equal return 0;
  */
  int Compare(const Version& v) {
    if (major_ > v.major_) return 1;
    else if (major_ < v.major_) return -1;
    else {
      if (minor_ > v.minor_) return 1;
      else if (minor_ < v.minor_) return -1;
      else {
        if (patch_ > v.patch_) return 1;
        else if (patch_ < v.patch_) return -1;
        else {
          return identifier_.compare(v.identifier_);
        }
      }
    }
  }

  bool Gt(const Version& v) { return this->Compare(v) == 1; }
  bool Gte(const Version& v) { return this->Compare(v) >= 0; }
  bool Lt(const Version& v) { return this->Compare(v) == -1; }
  bool Lte(const Version& v) { return this->Compare(v) <= 0; }
  bool Eq(const Version& v) { return this->Compare(v) == 0; }
  bool Neq(const Version& v) { return this->Compare(v) != 0; }

 private:
  std::string original_ = "";
  bool valid_ = true;

  void _Parse() {
    if (original_.length() > kVersionMaxLen || original_.length() == 0) {
      return;
    }
    // string index;
    uint32_t i = 0;
    // version type: major_ = 0, minor_ = 1, patch_ = 2, identifier_ = 3;
    uint32_t j = 0;

    // trim left v
    if (original_[i] == 'v') {
      i++;
    }

    uint32_t sum = 0;
    bool has_calculation = false;
    while (i < original_.length()) {
      char cur = original_[i];
      // 1.2.3
      if (cur != '.') {  // it's numercial or identifier_
        int c = static_cast<int>(cur - '0');
        if (c >= 0 && c <= 9) { // it's numerical
          sum = sum * 10 + static_cast<uint32_t>(c);
          i++;
          has_calculation = true;
          continue;
        }
        // it's not numerical
        if (j == 2 && has_calculation) { // patch_ is not set yet, set patch_ first
          if (!this->_SetVersion(j, sum)) {
            return;
          }
          sum = 0;
          j++;
          has_calculation = false;
          this->_SetIdentifier(i);
          return;
        } 
        // invalid if it's not setting patch_
        this->_MarkInvalid();
        return;
      }
      if (j < 3 && has_calculation) { // all version ares not set yet.
        if (!this->_SetVersion(j, sum)) {
          return;
        }
        sum = 0;
        j++;
        has_calculation = false;
        if (j == 3) { // major_, minor_, patch_ are all set
          this->_SetIdentifier(i);
          return;
        }
      }
      i++;
    }
    if (j < 3 && has_calculation) {
      this->_SetVersion(j, sum);
      has_calculation = false;
    }
  }
  bool _SetVersion(const uint32_t pos, const uint32_t sum) {
    // all version has to be one char
    if (sum > kVersionMaxLen) {
      this->_MarkInvalid();
      return false;
    }
    if (pos == 0) major_ = static_cast<unsigned char>(sum);
    if (pos == 1) minor_ = static_cast<unsigned char>(sum);
    if (pos == 2) patch_ = static_cast<unsigned char>(sum);
    return true;
  }

  void _SetIdentifier(const uint32_t pos) {
    char *ptr = const_cast<char *>(original_.c_str());
    // put the reset to identifier_

    std::string allow_connector(kAllowIdentifierConnector);

    // v1.1.2-alpha.0
    // v1.2.1+beta.0
    bool has_connector = allow_connector.find(original_[pos]) != std::string::npos;

    // skip if this is a connector
    identifier_ = (ptr + pos + ((has_connector) ? 1 : 0));
  }
  // mark to invalid
  void _MarkInvalid() {
    major_ = '\0';
    minor_ = '\0';
    patch_ = '\0';
    valid_ = false;
  }
};

static bool operator < (Version l, Version r) { return l.Lt(r); }
static bool operator <= (Version l, Version r) { return l.Lte(r); }
static bool operator > (Version l, Version r) { return l.Gt(r); }
static bool operator >= (Version l, Version r) { return l.Gte(r); }
static bool operator == (Version l, Version r) { return l.Eq(r); }
static bool operator != (Version l, Version r) { return l.Neq(r); }

}  // namespace semantic


