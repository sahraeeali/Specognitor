cd hpn-ssh/
rm *.bc
rm *.ll
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm hash.c -o hash.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json hash.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json hash.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json hash.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json hash.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json hash.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json hash.bc

cd ..
cd openssl/
rm *.bc
rm *.ll
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm des.c -o  des.bc
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm str2key.c -o str2key.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json des.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json des.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json des.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json des.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json des.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json des.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json str2key.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json str2key.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json str2key.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json str2key.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json str2key.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json str2key.bc

cd ..
cd tegra/
rm *.bc
rm *.ll
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm camellia_generic.c -o camellia.bc
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm salsa20_generic.c -o salsa20.bc
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm seed.c -o  seed.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json camellia.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json camellia.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json camellia.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json camellia.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json camellia.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json camellia.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json salsa20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json salsa20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json salsa20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json salsa20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json salsa20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json salsa20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json seed.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json seed.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json seed.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json seed.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json seed.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json seed.bc

cd ..
cd tomcrypt/
rm *.bc
rm *.ll
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm aes.c -o  aes.bc
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm base16_encode.c -o  encoder.bc
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm chacha20poly1305_memory.c -o  chacha20.bc
clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm ocb3_init.c -o  ocb3.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json aes.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json aes.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json aes.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json aes.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json aes.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json aes.bc

ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json encoder.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json encoder.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json encoder.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json encoder.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json encoder.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json encoder.bc

ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json chacha20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json chacha20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json chacha20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json chacha20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json chacha20.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json chacha20.bc

ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json ocb3.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json ocb3.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json ocb3.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json ocb3.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json ocb3.bc
ulimit -s unlimited
../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -enable-btbmodel -BTB-sets=8 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json ocb3.bc

cd ..
