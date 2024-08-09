#!/usr/bin/env python3

import sys

curr_ord = []

for line in sys.stdin:
    line = line.strip()
    attribs = line.split("\t")
    keys = attribs[0].split(",")
    attribs = attribs[1:]
    if keys[2] == "order":
        curr_ord = attribs
    else:
        # CID PID ORDID QUANT PPU RID RATING TEXT
        print(f"{keys[0]}", end="\t")
        print(f"{keys[1]}", end="\t")
        #print(f"{keys[2]}", end="\t")
        for i in range(len(curr_ord)):
            print(f"{curr_ord[i]}", end="\t")
        for i in attribs:
            print(f"{i}", end="\t")
        print()
