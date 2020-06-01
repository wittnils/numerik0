''' 
Python-Code zur numerischen Approximation von Integralen. 
Wichtig ist, dass die eingegebene Funktion auch integrierbar ist! D.h., dass sie auf dem zu berechnenden Intervall 
integrierbar ist, ansonsten kann dieses Programm offensichtlich nicht funktionieren
'''
import math 

def f(x):
    # Hier die Funktionsvorschrift reinschreiben
    return math.sin(x)**math.cos(x)

def AreaLeft(a,b,n):
    h = float(b-a)/n
    i = 0
    sum = 0
    while i<n:
        sum += f(a+i*h)
        i += 1
    return sum*h

def AreaRight(a,b,n):
    h = float(b-a)/n
    i = 0
    sum = 0
    while i<n:
        sum += f(a+(i+1)*h)
        i += 1
    return sum*h

def DeltaArea(a,b,n):
    return (float(b-a))/n*(f(a)-f(b))

# Main 
a = input("Startwert: ")
b = input("Endwert: ")
n = input("Schrittzahl: ")

IntLeft = AreaLeft(a,b,n)
IntRight = AreaRight(a,b,n)

print(IntLeft,IntRight)

