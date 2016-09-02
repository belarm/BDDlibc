#!/bin/bash
mkdir Debug/images
cd Debug/images
../tests
for i in $(ls |grep dot$|xargs -n 1 basename -s .dot)
do 
	dot ${i}.dot -Tpng -o ${i}.png
done
cd -
echo "images created in Debug/images/"
eog Debug/images/
