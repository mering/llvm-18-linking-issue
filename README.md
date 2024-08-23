# Reproduction example for LLVM 18 linking issue

Download Bazelisk:
```sh
curl -LSs "https://github.com/bazelbuild/bazelisk/releases/download/v1.20.0/bazelisk-linux-amd64" -o "./bazelisk"
chmod +x "./bazelisk"
```

Compile with Bazel:
```sh
./bazelisk build //test --verbose_failures
```

Observe the following linking error:
```
ld.lld: error: undefined symbol: absl::lts_20240116::log_internal::LogMessage& absl::lts_20240116::log_internal::LogMessage::operator<<<unsigned long, 0>(unsigned long const&)
```

Upstream bug: https://github.com/llvm/llvm-project/issues/102443

