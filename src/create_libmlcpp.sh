set -e
rm -f *.o
rm -f ../lib/libmlcpp.a
g++  -I ../include  -c -O3  *.cpp
ar  rcs  ../lib/libmlcpp.a   *.o
rm  -f  *.o
