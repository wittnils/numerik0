''' In diesem Code soll das Runge-Kutta-Verfahren zur Loesung ODEs realisiert werden. 
Wir betr. dafuer das Anfangswertproblem der Form y'(t) = f(y,t) und y(t_0) = y_0, das wir numerisch loesen wollen. 
Wir waehlen ein h > 0 (man waehlt am besten h so, s.d. die Intrervalle "glatt" aufgehen) und def.:

y_{n+1} = y_n + 1/6*(k_1 + 2k_2 +2k_3 + k_4) mit: 

k_1 = h*f(t_n, y_n)                         Steigung am Beginn des Intervalls  
k_2 = h*f(t_n + h/2, y_n + k_1/2)           Steigung in der Mitte des Intervalls mit y und k_1
k_3 = h*f(t_n + h/2, y_n + k_2/2)           Steigung in der Mitte des Intervalls mit y und k_2
k_4 = h*f(t_n + h, y_n + k_3)               Steigung am Ende des Intervalls

Wir loesen hier das Problem : 
    dy/dt = sin(t)^2*y mit y(t_0) = 1 und t_0 = 1
'''
import math 
import matplotlib.pyplot as plt

# Runge-Kutta-Methode
def Kvalues(t_n,y_n,h):
    k_1 = h*( math.sin(t_n)**2 * y_n )
    k_2 = h*( math.sin(t_n+ h/2)**2 * (y_n+k_1/2) )
    k_3 = h*( math.sin(t_n+ h/2)**2 * (y_n+k_2/2) )
    k_4 = h*( math.sin(t_n+ h)**2 * (y_n+k_3) )
    KValues = [k_1,k_2,k_3,k_4]
    print(KValues)
    return KValues

def nextY(y_n, KValues):
    return y_n + 1./6*(KValues[0]+2*KValues[1]+2*KValues[2]+KValues[3])

# Euler-Methode
# Der 
def nextVal(y_n, h, t):
    return h*(math.sin(t)**2 * y_n)+y_n 

# Execution 
# Initialisierung 

# Schrittweite
h = 0.1

# Initalwerte
y_0 = 1
t = 0
ApprValues = [y_0]
ApprValuesEuler = [y_0] 
ExactValues = [y_0]

# Laenge des zu approximierenden Intervalls
IntervalSize = 10
NumberOfComp = round(IntervalSize/h) 

tValues = []
i = 0
while i <= NumberOfComp:
    tValues.append(t+h*i)
    i += 1

# Berechnung mit Runge-Kutta
i = 0
yr = y_0
while i < NumberOfComp: 
    KValues = Kvalues(tValues[i],yr,h)
    yr = nextY(yr,KValues)
    ApprValues.append(yr)
    i = i+1 

# Berechnung der Approximation mittels Euler-Verfahren  
i = 0
while i < NumberOfComp:
    t = tValues[i]
    j = nextVal(ApprValuesEuler[i],h,t)
    ApprValuesEuler.append(j)
    i += 1

# Computing exact Values
i = 0
while i < NumberOfComp:
    t = tValues[i]
    ExactValues.append(math.exp(1./2*(t-math.sin(t)*math.cos(t))))
    i += 1
# Plotting 

# Approximation:   
# Plotte die Approximation  
plt.plot(tValues, ApprValues, label = "Approximation mit Runge-Kutta") 
plt.plot(tValues, ApprValuesEuler, label = "Approximation mit Euler") 
plt.plot(tValues, ExactValues, label = "Exakte Werte") 
  
# Formatierung des Plots
plt.xlabel("x-Achse") 
plt.ylabel("y-Achse")  
plt.title("Runge-Kutta- und Euler-Methode zur numerischen Loesung von Anfangswert ODE-Problemen") 
plt.legend() 
  
# Ruft den Plot auf 
plt.show() 