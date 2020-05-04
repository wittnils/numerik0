import random
import copy
import time 
import math

def round_up(n, decimals=0):
    multiplier = 10 ** decimals
    return math.ceil(n * multiplier) / multiplier

def nextVal(curValue,y):
    x = round_up(curValue-y,2)
    t = round_up(x+y,2)
    return t

# main 
y = -0.755
curValue = 2.46
for i in range(11):
    print(curValue)
    curValue = nextVal(curValue,y)
