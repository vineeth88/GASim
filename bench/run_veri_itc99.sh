#!/bin/bash
cd $1/
echo `pwd`
verilator --cc --coverage-line --prefix Vtop -CFLAGS -g $1.v
cd obj_dir/
make -f Vtop.mk
cp Vtop__ALL.a ../
cd .. 
mv obj_dir/ cpp/
cd ..

