Let's play with new cpp ;)

mkdir build

cmake  -DCMAKE_CXX_COMPILER="/usr/local/bin/clang++" -DCMAKE_CXX_FLAGS="-std=c++1z -stdlib=libc++" ..

make
