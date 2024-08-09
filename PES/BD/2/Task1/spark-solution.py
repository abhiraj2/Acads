#!/usr/bin/env python3

from pyspark.sql import SparkSession
from pyspark.sql.functions import desc
import sys

spark = SparkSession.builder\
        .master("local[*]")\
        .appName("sparkSolution")\
        .getOrCreate()

case12 = sys.argv[1]
case13 = sys.argv[2]
case14 = sys.argv[3]
case_key = sys.argv[4]
judges_clean = sys.argv[5]
judges_case_key = sys.argv[6]
acts_section = sys.argv[7]
output = sys.argv[8]

df = spark.read\
        .option("inferSchema", "true")\
        .option("header", "true")\
        .csv([case12, case13, case14])
state_table = spark.read\
        .option("inferSchema", "true")\
        .option("header", "true")\
        .csv(case_key)

stateJoinExpr = (df["state_code"] == state_table["state_code"]) & (df["year"] == state_table["year"])

res1 = df.join(state_table, stateJoinExpr).select(df["year"], df["state_code"], state_table["state_name"]).groupBy("state_code", "state_name").count().sort(desc("count")).limit(10)
res1 = res1.select("state_name")

result1 = [data[0] for data in res1.select("state_name").collect()]

df2 = spark.read\
        .option("inferSchema", "true")\
        .option("header", "true")\
        .csv(acts_section)

judge_dat = spark.read\
        .option("inferSchema", "true")\
        .option("header", "true")\
        .csv(judges_clean)

judge_merge = spark.read\
        .option("inferSchema", "true")\
        .option("header", "true")\
        .csv(judges_case_key)

join_case_judge = (df2["ddl_case_id"] == judge_merge["ddl_case_id"])
join_case_year = (df2["ddl_case_id"] == df["ddl_case_id"])
df2 = df2.where("criminal=1")


res2 = df2.join(judge_merge, join_case_judge).join(df, join_case_year)
res2 = res2.filter("ddl_decision_judge_id IS NOT NULL")
res2 = res2.groupBy("ddl_decision_judge_id").count().sort(desc("count")).limit(1)
# $SPARK_HOME/bin/spark-submit spark-solution.py ./Data/case_2012.csv ./Data/case_2013.csv ./Data/case_2014.csv ./Data/cases_state_key.csv ./Data/judges_clean.csv ./Data/judge_case_merge_key.csv ./Data/acts_sections.csv ./myout.txt

result2 = res2.select("ddl_decision_judge_id").collect()[0][0]
with open(output, "w+") as out_file:
        out_file.write(str((result1, result2)))