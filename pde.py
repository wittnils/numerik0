import math
import time
# SUB
# definiert f(x)
def f(x,y):
    return 0.
    
# definiert g(x)
def g(x,y):
    return x+2.*y
    
# Rand befuellen
def rand(u,n,h):
    # Oberer Rand
    for k in range(n+1):
        x = k*h
        u[k][0] = g(x,0.)
    # Unterer Rand
    for i in range(n+1):
        x = i*h
        u[i][n] = g(x,1.)
    # Linker Rand
    for j in range(1,n):
        y = j*h
        u[0][j] = g(0.,y)
    # Rechter Rand
    for p in range(1,n):
        y = p*h
        u[n][p] = g(1.,y)
             
'''# Errechntet den maximalen Fehler (error=u-u_h)       
def maxerror(u,n,g,h):
    m_error = 0
    for i in range(1,n):
        y = i*h
        for j in range(1,n):
            x = j*h
            error = abs(u[j][i] - g(x,y))
            m_error = max(error,m_error)
    return m_error'''
    
def MG(u,r,n,h):
    nc=n/2
    hc=h*2.
    # Vorglaettung des Gitters
    gs(u,r,n,h)
    gs(u,r,n,h)
    #print "Nach GS:", maxdef(u,r,n,h)
    rc = []
    hilf = [] 
    for i in range(0,nc+1):
        rc.append(i)
        hilf.append(i)
    for ic in range(0,nc+1):
        rc[ic] = hilf
    # uc (Fehler) mit 0 initialiseren
    uc = []
    for i in range(0,nc+1):
        uc.append(i)
    for ic in range(0,nc+1):
        uc[ic]=hilf
        for jc in range(0,nc+1):
            uc[ic][jc]= 0.
    injektion(u,r,nc,h,rc)
    #print "Nach Injektion:", maxdef(uc,rc,nc,hc)
    # Wenn ich auf dem groebsten Gitter bin
    if nc == 2:
        gs(uc,rc,nc,hc)
    else:
        MG(uc,rc,nc,hc)
    #print "Nach Injektion:", maxdef(uc,rc,nc,hc)
    # interpoliert die Naeherung uc vom groben auf das feine Gitter
    intadd(u,uc,nc)
    #print "Nach Interpolation:", maxdef(u,r,n,h)
    gs(u,r,n,h)

# Berechnet den Defekt in den Punkten, die auch zum Gitter gehoeren    
def injektion(u,r,nc,h,rc):
    for ic in range(1,nc):
        i = 2*ic
        for jc in range(1,nc):
            j = 2*jc
            # befuellt den Defekt der Grobgitterpunkte in rc 
            rc[ic][jc] = r[i][j] - (1./h**2*(u[i-1][j]-4.*u[i][j]+u[i+1][j]+u[i][j+1]+u[i][j-1]))
            
# interpoliert die Naeherung uc vom groben aufs feinere Gitter 
def intadd(u,uc,nc):
    #print "nc,uc,u:",nc
    #print uc
    #print u
    #print
    for ic in range(0,nc):
        i = 2*ic
        for jc in range(0,nc):
            j = jc*2
            # bildet MW aus den GGP 
            u[i][j] = u[i][j] + uc[ic][jc]
            u[i+1][j] = u[i+1][j] + 0.5*(uc[ic][jc]+uc[ic+1][jc])
            u[i][j+1] = u[i][j+1] + 0.5*(uc[ic][jc]+uc[ic][jc+1])
            u[i+1][j+1] = u[i+1][j+1] + 0.25*(uc[ic][jc]+uc[ic+1][jc]+uc[ic][jc+1]+uc[ic+1][jc+1])
            

# Hier wird das GSV definiert
def gs(u,r,n,h):
    for i in range(1,n):
        for j in range(1,n): 
            u[i][j] = -0.25*(h**2*r[i][j]-u[i-1][j]-u[i+1][j]-u[i][j+1]-u[i][j-1])
        
def printl(u,h):
    for i in range(n+1):
        for j in range(n+1):
            print >> o_d,i*h,j*h,u[i][j]
        print >> o_d

# Berechnet den Defekt in jedem Gitterpunkt und sucht nach dem groessten
def maxdef(u,r,n,h):
    maxdefekt = 0
    for i in range(1,n-1):
        for j in range(1,n-1):
            defekt = abs(r[i][j] - (1./h**2*(u[i-1][j]-4*u[i][j]+u[i+1][j]+u[i][j+1]+u[i][j-1])))
            maxdefekt = max(maxdefekt, defekt)
    return maxdefekt
    
# r mit f(x,y) befuellen    
def rbef(r,h,n):
    for v in range(n+1):
        y=v*h
        for b in range(n+1):
            x=b*h
            r[b][v]=f(x,y)
        x=0
           
# innere Werte nullen        
def nullen(u,n):
    for m in range(1,n):
        for l in range(1,n):
            u[m][l]=0.
                
# MAIN
print("Das ist ein Programm zur Diskretisierung der Poisson-Gleichung durch ein Mehrgitterverfahren")
x = 0
y = 0
n = input("Anzahl der Intervalle: ")
print("Es wird nun ein Gleichungssystem mit n*n Unbekannten errechnet")
u = []
helfe = []
for i in range(n+i):
    u.append(i)
    helfe.append(i)
# Stellt eine Zweidimensionale Liste von u her
k = 0
for k in range(n+1):
    u[k] = helfe
# Stellt r als zweidimensional her
k = 0
r = []
helfeR = []
for i in range(n+i):
    r.append(i)
    helfeR.append(i)
for k in range(n+1):
    r[k] = helfeR
t = 256
h = 1./n
# Startwerte fuer die inneren Werte von u werden festgelegt
nullen(u,n)
# Randwerte von u einsetzen
rand(u,n,h)
# r wird mit den Funktionswerten von f befuellt
rbef(r,h,n)
# Sucht den groessten Defekt, bevor das GSV ueberhaupt angefangen hat 
md_0 = maxdef(u,r,n,h)
print("startdefekt:")
print(md_0)
t1 = time.time()
# Doppelte Abbruchbedingung: Nach i Schritten oder, sobald der Defekt
# einen Wert unterschritten hat
i = 0
while i<t:
    MG(u,r,n,h)
    md = maxdef(u,r,n,h)
    print(md)
    if md < md_0*10**(-11):
        print("Iterationen :") 
        print(i)
        break
    i += 1
t2 = time.time()
print("Zeit:")
print(t2-t1)
time.sleep(5)
o_d=open("erg.txt","w")
printl(u,h)
o_d.close()
time.sleep(5)
