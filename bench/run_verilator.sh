#!/bin/bash
unset VERILATOR_ROOT
cd $1/
echo `pwd`
cp $1.v top.v
verilator --cc top.v --coverage-line
cd obj_dir/
make -f Vtop.mk
cd ..
rm top.v
cd ..

