#!/usr/bin/env python3

import sys


for line in sys.stdin:
    if line == "\n":
        continue
    line = line.rstrip()
    attribs = line.split("\t")
    print(f"{attribs[1]}, {attribs[6]}", end="\t")
    for i in attribs:
        print(f"{i}", end="\t")
    print()