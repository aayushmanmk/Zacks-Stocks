#!/bin/bash

# to over come the single quote problem  this gets me the correct line

# $ awk -F" " '{ printf("{x:\x27%s\x27, y:%s}\n",$1, $2) }' newfile

stockslist=`cat snp.txt`
#stockslist=`cat stocks.txt`


for stock in $stockslist
do
echo $stock
/usr/bin/curl --output $stock.dat https://www.zacks.com/stock/quote/$stock?q=$stock

done
