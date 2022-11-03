cd PHT/
for d in *; do
	cd $d;
	clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm test.c -o test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=4 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=4 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json test.bc
	echo "$d is Done."
	cd ../
done

cd ../BTB/
for d in *; do
	cd $d;
	clang-6.0 -g -c -O0 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__ -emit-llvm test.c -o test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=4 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=4 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json test.bc
	ulimit -s unlimited
	../../../../klee/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json test.bc
	echo "$d is Done."
	cd ../
done
