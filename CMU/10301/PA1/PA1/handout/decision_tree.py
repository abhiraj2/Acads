import tsv, sys

class Node:
    '''
    Here is an arbitrary Node class that will form the basis of your decision
    tree. 
    Note:
        - the attributes provided are not exhaustive: you may add and remove
        attributes as needed, and you may allow the Node to take in initial
        arguments as well
        - you may add any methods to the Node class if desired 
    '''
    def __init__(self):
        self.left = None
        self.right = None
        self.attr = None
        self.vote = None
    

class DecisionTree:
    def __init__(self, df, maxDepth):
        self.root = Node()
        self.df = df
        self.maxDepth = maxDepth
    
    def Train(self, root, df=None, depth=0):
        if df == None:
            df = self.df

        #df.Head()
        if depth == self.maxDepth or df.cols < 2:
            m = df.MaxVote()
            root.vote = m
            return
        split = -1
        ma = df.MutualInfo(0)
        for i in range(len(df.columns)-1):
            if ma <= df.MutualInfo(i) and ma > 0:
                ma = df.MutualInfo(i)
                split = i
        print(f'''
Maximum Mutual Info: {ma} from {split}
''')
        df1 = None
        df0 = None
        if split < 0:
            m = df.MaxVote()
            root.vote = m
            return
        else:
            print(f'''
Splitting Tree on {df.columns[split]}
''')
            root.attr = df.columns[split]
            (df0, df1) = df.Split(split)
            # df0.Head()
            # df1.Head()
            root.left = Node()
            root.right = Node()
            self.Train(root.left, df1, depth+1)
            self.Train(root.right, df0, depth+1)

    def Print_Tree(self, root, depth=0):
        print("|\t"*depth, end="")
        if root.left == None and root.right == None:
            print(root.vote)
            return
        else:
            if root.attr != None:
                print(root.attr)
            else:
                print(root.vote)
            self.Print_Tree(root.left, depth+1)
            self.Print_Tree(root.right, depth+1)
    
    def Test(self, root, df):
        res = []
        error = 0
        for row in df.values:
            res.append(self.get_prediction(root, row, df))
        lastCol = df.cols - 1
        for idx in range(len(res)):
            if res[idx] != df.values[idx][lastCol]:
                error+=1

        error /= len(df.values)
        return (res, error)
        
    
    def get_prediction(self, root, row, df):
        if root.attr == None:
            #print(f"Vote: {root.vote}")
            return root.vote
        else:
            idx = df.columns.index(root.attr)
            if row[idx] == "1":
                return self.get_prediction(root.left, row, df)
            if row[idx] == "0":
                return self.get_prediction(root.right, row, df)


if __name__ == '__main__':
    train_inp = sys.argv[1]
    test_inp = sys.argv[2]
    max_depth = int(sys.argv[3])
    train_out = sys.argv[4]
    test_out = sys.argv[5]
    metrics_out = sys.argv[6]

    df_train = tsv.TSV(train_inp)
    df_test = tsv.TSV(test_inp)
    dt = DecisionTree(df_train, max_depth)
    dt.Train(dt.root)
    dt.Print_Tree(dt.root)

    res_train = dt.Test(dt.root, df_train)
    res_test = dt.Test(dt.root, df_test)
    
    with open(train_out, "w+") as f:
        for i in res_train[0]:
            print(i, file=f)
    with open(test_out, "w+") as f:
        for i in res_test[0]:
            print(i, file=f)
    with open(metrics_out, "w+") as f:
        print(f"error(train): {res_train[1]}", file=f)
        print(f"error(test): {res_test[1]}", file=f)
    
