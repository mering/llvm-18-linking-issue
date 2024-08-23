// NOTE The linker error disappears when commenting out the next line or moving it after the other include
#include <gtest/gtest.h>

#include "absl/log/check.h"

TEST(Foo, Bar) {
  int u = 0;
  CHECK(false) << "Invalid " << u << ".";
}

// NOTE The linker error disappears when commenting out the next line
//namespace absl { ABSL_NAMESPACE_BEGIN namespace log_internal { template LogMessage& LogMessage::operator<<(int const&); } ABSL_NAMESPACE_END } 
