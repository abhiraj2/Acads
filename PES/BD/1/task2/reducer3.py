#!/usr/bin/env python3

import sys

curr_prod = None
curr_quant = 0

for line in sys.stdin:
    line = line.strip()
    prod_name, quant = line.split("\t")
    #print("Debug", prod_name, quant)
    quant = int(quant)
    if curr_prod == prod_name:
        curr_quant += quant
    else:
        if curr_prod:
            print(f"{curr_prod}\t{curr_quant}")
        curr_prod = prod_name
        curr_quant = quant
if curr_prod:
    print(f"{curr_prod}\t{curr_quant}")
    