#!/usr/bin/env python3
import sys
import json


for idx, line in enumerate(sys.stdin):
    if "[" in line or "]" in line:
        continue
    line = line.strip()
    line = line.rstrip(",")
    ele = json.loads(line)
    local_strike_rate = 0
    if ele["balls"] != 0:
        local_strike_rate = (ele["runs"]/ele["balls"])*100
    else:
        local_strike_rate = 0
    print(f"{ele['name']}\t{local_strike_rate}")