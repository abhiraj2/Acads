import sys
import pandas as pd
import tsv

if __name__ == "__main__":
    inp = sys.argv[1]
    output = sys.argv[2]

    df = tsv.TSV(inp)
    with open(output, "w") as f:
        print(f"entropy: {round(df.Entropy(), 5)}", file=f)
        print(f"error: {round(df.Error(), 5)}", file=f)
