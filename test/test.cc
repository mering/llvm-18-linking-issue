// SOURCE: https://github.com/google/googletest/blob/v1.14.0/googletest/include/gtest/gtest-message.h#L121-L150
#include <type_traits>
// NOTE The linker error disappears when using "absl/strings/has_absl_stringify.h" instead of "absl/strings/internal/has_absl_stringify.h".
// This change has also been done in googletest in https://github.com/google/googletest/commit/80306a7b5ec8ddaabd26540f2827b903d69e7a02 and is present in v1.15.0
#include "absl/strings/internal/has_absl_stringify.h"
class Message {
 public:
  // Streams a non-pointer value to this object. If building a version of
  // GoogleTest with ABSL, this overload is only enabled if the value does not
  // have an AbslStringify definition.
  template <typename T,
            typename std::enable_if<
                !absl::strings_internal::HasAbslStringify<T>::value,  // NOLINT
                int>::type = 0
            >
  inline Message& operator<<(const T& val) {
    return *this;
  }
};

#include "absl/log/internal/log_message.h"
#define MY_LOG() ::absl::log_internal::LogMessage()

int main() {
  int u = 0;
  MY_LOG() << "Invalid " << u << ".";
}

// NOTE The linker error disappears when commenting out the next line
//namespace absl { ABSL_NAMESPACE_BEGIN namespace log_internal { template LogMessage& LogMessage::operator<<(int const&); } ABSL_NAMESPACE_END } 
