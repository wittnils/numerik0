# Skript zur Berechnung von so Numerik-Sachen
import random
import copy


def calc(liste,exp,len):
    i = 2
    sum = 1./2
    while i<len+2:
        sum += liste[i-2]*2**(-i)
        print(liste[i-2]*2**(-i))
        i = i+1
    return sum*exp

err = 2**32
def checall(i,liste,exp,toApprox):
    global err, smallest
    if i!=-1:
        checall(i-1,liste,exp,toApprox)
        liste[i] = 1
        checall(i-1,liste,exp,toApprox)
    else:
        print(liste)
        if( abs(calc(liste,exp,len)-toApprox) < err ):
            print(liste)
            err = abs(calc(liste,exp,len) - toApprox)
            smallest = copy.deepcopy(liste)
    
# main
len = 2 
liste = []
exp = 2**(-1)
for i in range(len):
    liste.append(0)
testl = [1,0]
toApprox = 0.3
checall(1,liste,exp,toApprox)
print(err)
print(smallest)
print(calc(smallest,exp,len))
print(calc(testl,exp,len))
