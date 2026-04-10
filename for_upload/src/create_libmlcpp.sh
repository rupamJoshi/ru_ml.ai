set -e
rm -f *.o
rm -f ../lib/libmlcpp.a
g++  -I ../include  -c  *.cpp
ar  rcs  ../lib/libmlcpp.a   *.o
rm  -f  *.o
