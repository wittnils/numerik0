import math
import time
# SUB
# definiert f(x)
def f(x):
    return math.sin(x)
# Hier wird das GSV definiert
def gs(u,r,n,h):
    om = 2./(1+math.sin(math.pi*h))
    for k in range(1,n):
        u_neu = -0.5*(r[k]*h**2-u[k-1]-u[k+1])
        u[k] = u[k] + om*(u_neu - u[k])
        
def printl(u,h):
    for b in range(n+1):
        print >> o_d,b*h,u[b]

def maxdef(u,r,n,h):
    maxdefekt = 0
    for i in range(1,n-1):
        defekt = abs(r[i] - (1./h**2*(u[i-1]-2*u[i]+u[i+1])))
        maxdefekt = max(maxdefekt, defekt)
    return maxdefekt
               
# MAIN
# erstmal pragmatisch fuer unser spez. Beispiel
x = 0
n = input("Anzahl der Intervalle: ")
u = range(n+1)
r = range(n+1)
t = 1000000
h = 1./n
# Startwerte fuer die inneren Werte von u werden festgelegt
for k in range(1,n):
    u[k] = 0
# Randwerte von u
u[0] =0 
u[n] = 1
# r wird mit den Funktionswerten von f befuellt
for k in range(n+1):
    x = k*h
    r[k] = f(x)
i = 0
md_0 = maxdef(u,r,n,h)
t1 = time.time()
while i<t:
    gs(u,r,n,h)
    md = maxdef(u,r,n,h)
    if md < md_0*10**(-6):
        print i
        break
    i += 1
t2 = time.time()
print "Zeit: ",t2-t1
time.sleep(3)
o_d=open("erg.txt","w")
#printl rundet stark
printl(u,h)
o_d.close()
