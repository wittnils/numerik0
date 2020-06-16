import time
# SUB
# Soll eine Dreiecksmatrix erstellen und die Loesungen anpassen
def gauss(A,r,x,n):
    # Die Diagonale ablaufen
    for i in range(0,n-1):
         # Die Elemente unter dem Diagonalenelement ablaufen
        for j in range(i+1,n):
            if A[i][i] == 0:
                # checkt: ist unter mir keine Null
                for k in range(i+1,n):
                # wenn das der fall ist, soll die Zeile mit meiner vertauscht 
                # werden
                    if A[k][i] != 0:
                        #Hilfsschleife wird erstellt und mit den Werten von A befuellt
                        q = []
                        for t in range(n-i):
                            q.append(t-i) 
                        for l in range(n-i):
                            q[l] = A[i][l]
                        for d in range(n-i):
                            A[i][d] = A[k][d]
                            A[k][d] = q[d]
                        rq = r[i]
                        r[i] = r[k]
                        r[k] = rq
                        break 
            # Faktor fuer deie jeweilige Spalte berechnen
            # Hier passiert Division by Zero. Hier muss die Ueberlegung 
            # fuer den allg. Fall machen
            f = -A[j][i]/A[i][i]
            # Alle Elemente der Zeile ablaufen
            for k in range(i,n):
                A[j][k] = A[j][k] + A[i][k]*f
                # Die Loesungswerte anpassen
            r[j] = r[j] + r[i]*f
                                  
# Soll rueckeinesetzen machen
def rueckeinsetzen(A,r,x,n):
    for i in range(n-1,-1,-1):
        summe = 0
        # Unter-unterprogramm zur Berechnung der Summe
        for j in range(i+1,n):
            summe += A[i][j]*x[j]
        x[i] = 1./A[i][i]*(r[i]-summe)

# MAIN
# Input der Koeffizienten
A = [[2.,2.,-3.],[-1.,-2.,-2.],[4.,1.,-2.]]
# Input des b-Vektors
r = [-7., 3., -1.]
x = []
n = len(r)
for c in range(n):
    x.append(1)
print("Das GLS hat folgende Form:")
print(A)
print("mit den folgenden B-Werten:")
print(r)
gauss(A,r,x,n)
rueckeinsetzen(A,r,x,n)
time.sleep(3)
print("Die Loesungen sind:", x)
time.sleep(6)
