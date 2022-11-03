# Specognitor
Specognitor is a symbolic execution engine with speculation semantics and cache modelling. Specognitor built on top of the KLEE symbolic execution engine, can thus provide a testing engine to check for the data leakage through cache side channel as shown via Spectre attacks.

- Our prediction logic can precisely model the speculative behavior of different systems which is a crucial factor in detecting Spectre vulnerabilities while maintaining the system performance and meeting energy consumption constraints.
- Our pattern detection mechanism can provide useful information to identify the root cause of data leakage, which is crucial for implementing an efficient defense mechanism.
- Our symbolic cache model can verify whether the sensitive data leakage due to speculative execution can be observed by an attacker at a given program point.

## Environment setting up.
This tool is based on KLEE v2.1, which needs the support of LLVM-6.0.  
NOTE: Suggest to refer "https://klee.github.io/build-llvm60/" to install all dependencies.  


### Install all the dependencies of LLVM
```
$ sudo apt-get install build-essential curl libcap-dev git python cmake libncurses5-dev python3-minimal python-pip python3-pip unzip libtcmalloc-minimal4 libgoogle-perftools-dev libsqlite3-dev doxygen bison flex libboost-all-dev perl minisat python-dev
$ sudo pip3 install lit tabulate
$ sudo pip install lit
```

### Install LLVM-6.0
```
$ sudo apt-get install clang-6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-tools
```
At this phase, it might be needed to install llvm as well.

### Install STP:
```
$ git clone https://github.com/stp/stp.git
$ cd stp/
$ git checkout tags/2.3.3
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ sudo cmake --install .
 ```

### Install uClibc
```
$ git clone -b klee_uclibc_v1.2 https://github.com/klee/klee-uclibc.git  
$ cd klee-uclibc  
$ git checkout tags/klee_uclibc_v1.2
$ ./configure --make-llvm-lib  
$ make -j2  
$ cd ..
```
if you have multiple clang installations, use the following code:
```
CC=/usr/bin/clang-6.0 ./configure --make-llvm-lib
```

## Build Specognitor
```
$ git clone https://github.com/sahraeeali/Specognitor.git
$ cd Specognitor/klee/
$ mkdir build
$ cd build
$ cp ../build.sh .  
  # (or build_debug.sh for debug version)
 ```
The content of build.sh
```
cmake \
  -DENABLE_SOLVER_STP=ON \
  -DENABLE_POSIX_RUNTIME=ON \
  -DENABLE_KLEE_UCLIBC=ON \
  -DKLEE_UCLIBC_PATH=/PATH/TO/ULIBC \
  -DLLVM_CONFIG_BINARY=/usr/lib/llvm-6.0/bin/llvm-config \
  -DLLVMCC=/usr/bin/clang-6.0 \
  -DLLVMCXX=/usr/bin/clang++-6.0 \
  -DCMAKE_BUILD_TYPE=Release \
  ..
```
Change "/PATH/TO/ULIBC" to your ulibc path.  
For different cmake build types you may find more information [here](https://klee.github.io/docs/developers-guide/#Run-time%20libraries)

```
$ ./build.sh
$ make -j 10
```    

Now you can use the "klee" and "klee-stat" in build/bin/

## Options to enable speculative execution and cache modeling
```
$ /PATH/TO/KLEE/ROOT/klee --help
...
Speculative execution options:
These options impact the speculative paths exploring and the cache modeling

-BTB-sets=<uint>                                       - BTB sets (default=256)
-BTB-tag-bits=<uint>                                   - Number of BTB tag bits (default=20)
-BTB-ways=<uint>                                       - BTB ways (default=1)
-PHT-reg-size=<uint>                                   - PHT register size (default=8)
-cache-line-size=<uint>                                - Cache line size (default=64)
-cache-sets=<uint>                                     - Cache sets (default=256)
-cache-ways=<uint>                                     - Cache ways (default=2)
-enable-btbmodel                                       - Enable BTB modeling (default=off).
-enable-cachemodel                                     - Enable Cache modeling (default=off).
-enable-phtmodel                                       - Enable PHT modeling (default=off).
-enable-speculative                                    - Enable Speculative exeuction modeling (default=off).
-max-sew=<uint>                                        - Maximum w (default=10)
-only-detect-marked-secret                             - Only detect marked secret (default=false)
-pattern pattern.json                                  - Pattern detection mechanism (Must be provided)
...
```
## Run a test without cache modelling: <br />

```
$ cd tests/Spectre-v1/PHT/v01/
$ clang-6.0 -g -c -emit-llvm test.c -o test.bc
```   
* "--enable-speculative" option enables the speculative paths exploring.  
* "--max-sew=#" set the Speculative Execution Windows (w) to # (default is 10).

#### For running the examples with **BTB**
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
```
You can change the BTB config parameters.

#### For running the examples with **PHT**
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 -pattern sample.json test.bc
```

You can change the PHT config parameters.

#### Run a test with cache modelling:
- Add "-enable-cachemodel" option to the command line.
- Cache configurations can be modified by adding the following parameters to command-line instruction.

  ```
  -cache-line-size=#
  -cache-sets=#
  -cache-ways=#
  ```

## Pattern Description
- Add "-pattern pattern.json" to the command line.
- Patterns should be described as a ```.json``` file.
- A pattern node can be described with following properties:
```
"nodeLabel" : {
  "isSpeculative" : integer,
  "startTTL" : integer,
  "stopTTL" : "false"/"true",
  "instruction" : string,
  "isSensitive" : integer,
  "checkCacheState" : integer,
  "isConst" : integer
}
```
  - ```integer``` values should be set to ```0``` or  ```1```. Setting values to ```2,3,4,...``` means the value of property is not important and removes the effect of the property on the node. For example, ```"isSpeculative" : 0``` means the instruction should not be detected in the speculation traces (should be detected in normal program execution).

Example of a Spectre-v1 pattern description can be presented as follows:
```json
{
  "0" : {
    "isSpeculative" : 0,
    "startTTL" : 32,
    "stopTTL" : "false",
    "instruction" : "Br",
    "isSensitive" : 2,
    "checkCacheState" : 2,
    "isConst" : 0
  },
  "1" : {
    "isSpeculative" : 1,
    "startTTL" : 0,
    "stopTTL" : "false",
    "instruction" : "Load",
    "isSensitive" : 2,
    "checkCacheState" : 2,
    "isConst" : 0
  },
  "2" : {
    "isSpeculative" : 1,
    "startTTL" : 0,
    "stopTTL" : "true",
    "instruction" : "Load",
    "isSensitive" : 1,
    "checkCacheState" : 1,
    "isConst" : 2
  }
}
```

This pattern consist of a non-speculative branch instruction ```"Br"```, an input-dependent (key-dependent) load instruction ```"Load"```,  a load instruction ```"Load"``` with input-dependent (key-dependent) address and after the third node, the system cache must be vulnerable to cache side-channel attacks ```"checkCacheState" : 1```.

## Run Specognitor on an example code
```c
#include <stdint.h>
#include <klee/klee.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 64];

uint8_t victim_fun(int idx)  __attribute__ ((optnone))
{
    uint8_t temp = 0;
    if (idx < array1_size) {    
        temp = array1[idx];
        temp &= array2[temp*64];
    }   

    /* This two lines disable the compiler optimization of array */
    array2[0] = 2;  
    array1[0] = 2;
    return temp;
}

int main() {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}
```
Compile above code to generate llvm bitcode:
```
clang-6.0 -g -c -emit-llvm test.c -o test.bc
```
Run the tool with the generated bitcode:
### BTB(1 entries, direct-mapped, no tag bits) - ROB(32 entries)
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
```
### BTB(1 entries, direct-mapped, no tag bits) - ROB(16 entries)
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 -pattern sample.json test.bc
```
### PHT(1 bit BHR, 1 entry PHT) - ROB(16 entries)
```
-check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 -pattern sample.json test.bc
```
### PHT(4 bit BHR, 16 entry PHT) - ROB(32 entries)
```
-check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 -pattern sample.json test.bc
```

The output of Specognitor for PHT(1 bit BHR, 1 entry PHT) and ROB(16 entries):
```
KLEE: WARNING: @Speculative execution modeling is enable! maxSEW=16
KLEE: Using STP solver backend
KLEE: WARNING:
@Start execution ==>

KLEE: WARNING ONCE: flushing 16384 bytes on read, may be slow and/or crash: MO10[16384] allocated at global:array2
KLEE: @CM: found a leakage: test.c: 22, ASM line=37, time = 30395, Cache ways: 2, Cache set: 256, count=0
KLEE: ----------------------- Leakage Detected --------------------
KLEE: -> test.c: 20, ASMLine: 24, liveness: 5
KLEE: -> test.c: 21, ASMLine: 30, liveness: -1
KLEE: -> test.c: 22, ASMLine: 37, liveness: -1
KLEE: ////////////////////////////////////////////////////////////////////////////////

KLEE: done: total instructions = 63
KLEE: done: completed paths = 2
KLEE: done: sp states without Prediction Logic = 2
KLEE: done: sp states with Prediction Logic = 1
KLEE: done: sp states reduction rate = 50%
KLEE: done: Common sp states = 1
KLEE: done: Completed sp states = 1
KLEE: done: Average instructions on speculative path = 16
KLEE: done: generated tests = 2
KLEE: done: loads: 14
KLEE: done: stores: 11
KLEE: done: constant loads: 11
KLEE: done: constant stores: 11
```
A description of the output: <br />
The line starts with `KLEE: @CM` denote there is a leakage found by the our tool with branch prediction and cache modeling. The detected leakage can be seen after `Leakage Detected` keyword. The number of lines depends on the pattern. Here, our pattern was a `BR-Load-Load"` that we presented earlier. For every node in our pattern, we print the corresponding source code line and llvm-IR line and also we print its liveness or time-to-live if it is defined for the node (`-1` if it is not defined).  
The rest of the output is the statistic of this test including the numbers of the executed instructions, explored paths, a lower bound on explored speculative paths without prediction logic (`sp states without Prediction Logic = 2`), the exact number of speculative states with prediction logic (`sp states with Prediction Logic = 1`), and the common states between these states (`Common sp states = 1`).

## Reproduce the data for time measurement.
Then run the bitcode with Specognitor with the commands mentioned above. You can run `klee-stats` to get the analysis time and the solver time.
```
/PATH/TO/KLEE/ROOT/build/bin/klee-stats klee-out-0
```
Output:
```
-------------------------------------------------------------------------
|   Path   |  Instrs|  Time(s)|  ICov(%)|  BCov(%)|  ICount|  TSolver(%)|
-------------------------------------------------------------------------
|klee-out-0|      63|     0.32|   100.00|   100.00|      42|       96.87|
-------------------------------------------------------------------------
```

# Using Specognitor with Docker
##Building the Docker image locally
```
$ git clone https://github.com/sahraeeali/Specognitor.git
$ cd Specognitor/klee
$ docker build -t Specognitor/Specognitor .
```
##Creating a Specognitor Docker container
```
docker run --rm -ti --ulimit='stack=-1:-1' Specognitor/Specognitor
```
