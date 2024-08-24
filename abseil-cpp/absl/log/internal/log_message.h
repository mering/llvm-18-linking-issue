// Copyright 2022 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ABSL_LOG_INTERNAL_LOG_MESSAGE_H_
#define ABSL_LOG_INTERNAL_LOG_MESSAGE_H_

#include "absl/strings/has_absl_stringify.h"

namespace absl {
namespace log_internal {

class LogMessage {
 public:
  // By-value overloads for small, common types let us overlook common failures
  // to define globals and static data members (i.e. in a .cc file).
  // clang-format off
  // The CUDA toolchain cannot handle these <<<'s:
  LogMessage& operator<<(signed int v) { return operator<< <signed int>(v); }
  // clang-format on

  // Literal strings.  This allows us to record C string literals as literals in
  // the logging.proto.Value.
  //
  // Allow this overload to be inlined to prevent generating instantiations of
  // this template for every value of `SIZE` encountered in each source code
  // file. That significantly increases linker input sizes. Inlining is cheap
  // because the argument to this overload is almost always a string literal so
  // the call to `strlen` can be replaced at compile time. The overload for
  // `char[]` below should not be inlined. The compiler typically does not have
  // the string at compile time and cannot replace the call to `strlen` so
  // inlining it increases the binary size. See the discussion on
  // cl/107527369.
  template <int SIZE>
  LogMessage& operator<<(const char (&buf)[SIZE]);

  // This prevents non-const `char[]` arrays from looking like literals.
  template <int SIZE>
  LogMessage& operator<<(char (&buf)[SIZE]);

  // Types that support `AbslStringify()` are serialized that way.
  template <typename T,
            typename std::enable_if<absl::HasAbslStringify<T>::value,
                                    int>::type = 0>
  LogMessage& operator<<(const T& v);

  // Types that don't support `AbslStringify()` but do support streaming into a
  // `std::ostream&` are serialized that way.
  template <typename T,
            typename std::enable_if<!absl::HasAbslStringify<T>::value,
                                    int>::type = 0>
  LogMessage& operator<<(const T& v);

  // Note: We explicitly do not support `operator<<` for non-const references
  // because it breaks logging of non-integer bitfield types (i.e., enums).

};

// Note: the following is declared `ABSL_ATTRIBUTE_NOINLINE`
template <typename T,
          typename std::enable_if<absl::HasAbslStringify<T>::value, int>::type>
LogMessage& LogMessage::operator<<(const T& v) {
  return *this;
}

// Note: the following is declared `ABSL_ATTRIBUTE_NOINLINE`
template <typename T,
          typename std::enable_if<!absl::HasAbslStringify<T>::value, int>::type>
LogMessage& LogMessage::operator<<(const T& v) {
  return *this;
}

template <int SIZE>
LogMessage& LogMessage::operator<<(const char (&buf)[SIZE]) {
  return *this;
}

// Note: the following is declared `ABSL_ATTRIBUTE_NOINLINE`
template <int SIZE>
LogMessage& LogMessage::operator<<(char (&buf)[SIZE]) {
  return *this;
}
// We instantiate these specializations in the library's TU to save space in
// other TUs.  Since the template is marked `ABSL_ATTRIBUTE_NOINLINE` we will be
// emitting a function call either way.
extern template LogMessage& LogMessage::operator<<(const int& v);

}  // namespace log_internal
}  // namespace absl

#endif  // ABSL_LOG_INTERNAL_LOG_MESSAGE_H_
