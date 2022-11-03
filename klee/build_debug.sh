cmake \
  -DENABLE_SOLVER_STP=ON \
  -DENABLE_POSIX_RUNTIME=ON \
  -DENABLE_KLEE_UCLIBC=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DKLEE_UCLIBC_PATH=/PATH/TO/ULIBC \
  -DLLVM_CONFIG_BINARY=/usr/lib/llvm-6.0/bin/llvm-config \
  -DLLVMCC=/usr/bin/clang-6.0 \
  -DLLVMCXX=/usr/bin/clang++-6.0 \
  ..
