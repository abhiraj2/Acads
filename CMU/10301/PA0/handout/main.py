import sys
import pandas as pd

def train(df):
    ones = 0
    zeroes = 0
    for i, row in df.iterrows():
        if row["heart_disease"] == 0:
            zeroes+=1
        else:
            ones+=1

    if zeroes > ones:
        return 0
    else:
        return 1

def test(df, maj):
    error = 0
    res=[]
    for i, row in df.iterrows():
        res.append(maj)
        if row["heart_disease"] != maj:
            error+=1
    return (error/len(res),res)
        
def wrtie_to_files(accu_train, accu_test, train_out, test_out, metric_out):
    with open(train_out, "w") as f:
        for i in accu_train[1]:
            f.write(str(i)+"\n")
    with open(test_out, "w") as f:
        for i in accu_test[1]:
            f.write(str(i)+"\n")
    with open(metric_out, "w") as f:
        f.write("error(train): "+str(accu_train[0])+"\n")
        f.write("error(test): "+str(accu_test[0])+"\n")
        

if __name__ == "__main__":
    df_train = pd.read_csv(sys.argv[1], sep='\t')
    df_test = pd.read_csv(sys.argv[2], sep='\t')
    train_out = sys.argv[3]
    test_out = sys.argv[4]
    metric_out = sys.argv[5]
    majority = train(df_train)
    accu_train = test(df_train, majority)
    accu_test = test(df_test, majority)
    wrtie_to_files(accu_train, accu_test, train_out, test_out, metric_out)
    

    