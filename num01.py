# Skript zur Berechnung von so Numerik-Sachen
import random 
import copy
err = 2**32
list2 = [0,0,0,0,0,0,0,0,0,0]

def calc(list2,list,exp):
    i = 0
    sum = 1./2
    while i<9:
        if(list[i] == -1):
            list2[i] = 0
        else:
            list2[i] = 1
        i = i+1
    i = 0
    while i<9: 
        sum += list2[i]*2**(-i)*2**(-1)
        i = i+1
    return sum*exp 


def checall(i,list,exp,toApprox):
    global err, smallest
    if i!=-1:
        checall(i-1,list,exp,toApprox)
        list[i] = list[i]*(-1)
        checall(i-1,list,exp,toApprox)
    else:
        if( abs(calc(list2,list,exp)-toApprox) < err ):
            err = abs(calc(list2,list,exp) - toApprox)
            smallest = copy.deepcopy(list)

# main
exp = 2**(-1)
list2 = [0,0,0,0,0,0,0,0,0,0]
list = [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]
toApprox = 0.3 
checall(9,list,exp,toApprox)
print(err)
print(smallest)
print(calc(list2,smallest,exp))
print(round(calc(list2,smallest,exp),4))
print("helloword")
