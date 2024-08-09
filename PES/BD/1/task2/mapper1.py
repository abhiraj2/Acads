#!/usr/bin/env python3

import sys


for line in sys.stdin:
    if line == "\n":
        continue
    line = line.rstrip()
    cols = line.split("\t")
    if cols[0] == "order":
        print(f"{cols[2]},{cols[3]},{cols[0]}\t{cols[1]}\t{cols[4]}\t{cols[5]}")
    else:
        print(f"{cols[3]},{cols[2]},{cols[0]}\t{cols[1]}\t{cols[4]}\t{cols[5]}")
