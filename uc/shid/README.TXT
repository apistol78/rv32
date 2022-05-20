README.TXT

---------------------

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

---------------------

shell 1:
1> openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -s tcl

shell 2:
1> gdb-multiarch load.elf
(gdb) target remote localhost:3333
(gdb) load
(gdb) monitor reset init
(gdb) continue

shell 3:
1> minicom -D /dev/ttyACM0 -b 115200
