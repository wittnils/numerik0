'''
Dies ist ein kleines Python-Programm zur numerischen Loesung von First-Order ODEs, d.h. von DGL mit y'=f(x,y) und y(x_0)=y_0. 
Dabei wird die sog. Eulermethode verwendet. Sie basiert auf dem Differentialquotienten
aus der Analysis, der bekanntermassen die Abl. f'(x) def. als lim_{h -> 0} (1/h*(f(x+h)-f(x))) =: f'(x). Fuer genuegend kleine h ist also f'(x) durch 1/h*(f(x+h)-(f(x))) approximierbar.
D.h. nach Umformen gilt: f(x+h)=f(x)+h*f'(x)=f(x)+h*f(x,y). Dadurch erhalten wir die diskretisierte Loesung des Problems. 
h bez. wir i.A. als Schrittweite (auch Inkrement oder Feinheit genannt).    
'''
# Exemplarisch lösen wir hier y'=y mit y(x_0) = 1.

import math 
import matplotlib.pyplot as plt

h = float(input("Schrittweite: "))
# Berechnen des n+1-ten Schrittes 
def nextVal(y_n, h):
    return y_n+h*y_n 

# MAIN
# Berechnung der Approximation mittels Euler-Verfahren  
y_0 = 1
ApprValues = [y_0] 
i = 0
while i < 10/h - 1:
    j = nextVal(ApprValues[i],h)
    ApprValues.append(j)
    i += 1
print(ApprValues)  

# Berechnung der exakten Werte 
ExactValues=[y_0]
i = 0
while i < 10/h-1:
    j = math.exp(h*i)
    ExactValues.append(j)
    i += 1
i = 0

# Berechnung des Fehlers 
while i < 10/h: 
    print(ExactValues[i]-ApprValues[i])
    i += 1

# Hier geht es um die graphische Ausgabe des Problems, dazu initalisieren wir x-Werte und ordnen diesem Intervall die berechneten y-Werte zu
x = []
# Initilaisierung der x-Werte 
i = 0
while i < 10/h:
    x.append(h*i)
    i += 1

# Approximation:   
# Plotte die Approximation  
plt.plot(x, ApprValues, label = "Approximation mit Euler") 
  
# Exakt
# Plotte die exakten Werte  
plt.plot(x, ExactValues, label = "Exakte Werte") 
  
# Formatierung des Plots
plt.xlabel("x-Achse") 
plt.ylabel("y-Achse")  
plt.title("Euler-Methode zur numerischen Loesung von Anfangswert ODE-Problemen") 
plt.legend() 
  
# Ruft den Plot auf 
plt.show() 