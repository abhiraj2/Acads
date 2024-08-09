#!/usr/bin/env python3

import sys

curr_ord = None
to_send = 0

for line in sys.stdin:
    line = line.strip()
    attribs = line.split("\t")
    keys = attribs[0].split(",")
    attribs = attribs[1:]
    if curr_ord == keys[0]:
        if int(keys[1]) < 3:
            for i in attribs:
                print(f"{i}", end="\t")
            print()
    else:
        curr_ord = keys[0]

        if int(keys[1]) < 3:
            to_send = 1
            for i in attribs:
                print(f"{i}", end="\t")
            print()
        else:
            to_send = 0
