import math

class TSV:        
    def __init__(self, path=None):
        self.columns = []
        self.values = []
        self.cols = 0
        self.rows = 0
        if path!=None:
            self.Read_tsv(path)

    def Read_tsv(self, path):
        print("Reading TSV file")
        with open(path) as f:
            data = f.readlines()
            for idx, line in enumerate(data):
                if idx == 0:
                    self.columns = line.strip().split("\t")
                else:
                    self.values.append(line.strip().split("\t"))
            self.cols = len(self.values[0])
            self.rows = len(self.values)
        print("Read Complete")

    def Head(self):
        if self.cols==0 and self.rows==0:
            print("Error, not initialized.")
        else:
            print(f'''
Rows: {self.rows}
Columns: {self.cols}    
            ''')
            toPrint = 6
            if self.rows < 6:
                toPrint = self.rows
            for i in range(toPrint):
                for j in range(self.cols):
                    print(self.values[i][j], end="\t")
                print() 

    def Entropy(self, idx = None):
        if idx == None:
            idx = self.cols -1
        distinct = {}
        total = 0
        # print(self.cols)
        for row in self.values:    
            if row[idx] not in distinct.keys():
                distinct[row[idx]] = 1
            else:
                distinct[row[idx]]+=1
        for i in distinct.keys():
            total += distinct[i]
        entropy = 0
        for i in distinct.keys():
            temp = distinct[i]/total
            entropy += temp*math.log2(temp)
        entropy *= -1
        return entropy
    
    def Error(self, idx = None):
        if idx == None:
            idx = self.cols -1
        distinct = {}
        total = 0
        for row in self.values:
            if row[idx] not in distinct.keys():
                distinct[row[idx]] = 1
            else:
                distinct[row[idx]]+=1
        for i in distinct.keys():
            total += distinct[i]
        
        error = 0
        ma = -9999
        ele = None
        for i in distinct.keys():
            if ma < distinct[i]:
                ma = distinct[i]
                ele = i
                
        for i in distinct.keys():
            if distinct[i] != ma or i != ele:
                error += distinct[i]
        error = error/total
        return error
    
    def Split(self, idxX):
        distinct = {}
        for row in self.values:
            if row[idxX] not in distinct.keys():
                distinct[row[idxX]] = 1
            else:
                distinct[row[idxX]]+=1
        res0 = TSV()
        res1 = TSV()
        colName = self.columns[idxX]
        for i in self.columns:
            if i!=colName:
                res0.columns.append(i)
        res1.columns = res0.columns
        for row in self.values:
            if row[idxX] == "0":
                l = []
                for idx, ele in enumerate(row):
                    if idxX == idx:
                        continue
                    else:
                        l.append(ele)
                res0.values.append(l)
            else:
                l = []
                for idx, ele in enumerate(row):
                    if idxX == idx:
                        continue
                    else:
                        l.append(ele)
                res1.values.append(l)
        res0.rows = len(res0.values)
        res0.cols = len(res0.columns)
        res1.rows = len(res1.values)
        res1.cols = len(res1.columns)

        return (res0, res1)

    def MutualInfo(self, idxX, idxY = None):
        if idxY == None:
            idxY = self.cols - 1
        entropyY = self.Entropy()
        (df1, df2) = self.Split(idxX)
        factor1 = len(df1.values)/(len(df1.values)+ len(df2.values))
        factor2 = len(df2.values)/(len(df1.values)+ len(df2.values))
        entropyXY1 = df1.Entropy()
        entropyXY2 = df2.Entropy()
        return entropyY - (factor1*entropyXY1 + (factor2)*entropyXY2)

    def MaxVote(self, idx = None):
        if idx == None:
            idx = self.cols - 1
        distinct = {}
        total = 0
        for row in self.values:
            if row[idx] not in distinct.keys():
                distinct[row[idx]] = 1
            else:
                distinct[row[idx]]+=1
        for i in distinct.keys():
            total += distinct[i]
        
        error = 0
        ma = -9999
        ele = None
        for i in distinct.keys():
            if ma <= distinct[i]:
                ma = distinct[i]
                ele = i
        return ele
        

    def SaveToFile(self,path):
        with open(path, "w+") as f:
            for idx, colName in enumerate(self.columns):
                if idx < len(self.columns)-1:
                    print(colName, end="\t", file=f)
                else:
                    print(colName, end="\n", file=f)

            for row in self.values:
                for i, ele in enumerate(row):
                    if i < len(row)-1:
                        print(ele, end="\t", file=f)
                    else:
                        print(ele, end="\n", file=f)