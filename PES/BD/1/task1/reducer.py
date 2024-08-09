#!/usr/bin/env python3
import sys

curr_player = None
curr_agg = 0
curr_iter = 0

dic = {}

curly_br_o = "{"
curly_br_c = "}"

for line in sys.stdin:
    name, local_strike_rate = line.split("\t")
    local_strike_rate = float(local_strike_rate)
    if curr_player:
        if curr_player == name:
            curr_agg += local_strike_rate
            curr_iter += 1
        else:
            print(f'{curly_br_o}"name": "{curr_player}", "strike_rate": {round(curr_agg/curr_iter, 3)}{curly_br_c}')
            curr_player = name
            curr_iter = 1
            curr_agg = local_strike_rate
    else:
        curr_player = name
        curr_iter = 1
        curr_agg = local_strike_rate

if curr_player:
    print(f'{curly_br_o}"name": "{curr_player}", "strike_rate": {round(curr_agg/curr_iter, 3)}{curly_br_c}')
