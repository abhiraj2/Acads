#!/usr/bin/env python3

import sys

for line in sys.stdin:
    if line == "\n":
        continue
    line = line.rstrip()
    cols = line.split("\t")
    print(f"{cols[1]}\t{cols[3]}")