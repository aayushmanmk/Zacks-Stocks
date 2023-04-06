#!/bin/bash
datafiles=`ls *.dat`
for file in $datafiles
do
	./readfilegets $file
done
