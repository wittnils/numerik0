# -*- coding: cp1252 -*-
import time
import random
# SUB
# Erstellen eines NxN-Spielfeldes
def resetsmat(smat, n):
    for k in range(2):
        for j in range(n):
            smat[k][j] = 0
    smat[2][0] = 0
    smat[2][1] = 0

def createmat(n):
    feld = range(n)
    for i in range(n):
        feld[i] = range(n)
    return feld
        
# Befuellen der NxN-Matrix mit "."
def fillmat(feld, n):
    for i in range(n):
        for j in range(n):
            feld[i][j] = "."

# Druckt das Spielfeld
def printmat(feld, n):
    for i in range(n):
        for j in range(n):
            print(feld[i][j],)
        print
        
# Befuellen durch den Spieler 1
def fill_1(feld, n, x_spieler1, y_spieler1):
    # liegt die eingabe innerhalb der Matrix?
    while x_spieler1 >= n or x_spieler1 < 0 or y_spieler1 < 0 or y_spieler1 >= n:
        print("Die Eingabe ist unzulaessig!")
        print("Gib einen Wert innerhalb der Matrix an.")
        x_spieler1 = input("X-Koordinate: ") -1
        y_spieler1 = input("Y-Koordinate: ") -1
    if feld[y_spieler1][x_spieler1] != ".":
        print("Die Eingabe ist nicht korrekt.")
        print("Gib nun erneut Koordinaten ein.")
        x_spieler1 = input("X-Koordinate: ") -1
        y_spieler1 = input("Y-Koordinate: ") -1
    feld[y_spieler1][x_spieler1] = "X"
            
# Befuellen durch Spieler 2/Computer
def fill_2(feld, n, x_spieler2, y_spieler2, spielstatus):
    if spielstatus == 1:
        while x_spieler2 >= n or x_spieler2 < 0 or y_spieler2 < 0 or y_spieler2 >= n:
            print("Die Eingabe ist unzulaessig!")
            print("Gib einen Wert innerhalb der Matrix an.")
            x_spieler2 = input("X-Koordinate: ") -1
            y_spieler2 = input("Y-Koordinate: ") -1
        while feld[y_spieler2][x_spieler2] != ".":
                print("Die Eingabe ist nicht korrekt.")
                print("Gib nun erneut Koordinaten ein.")
                x_spieler2 = input("X-Koordinate: ") -1
                y_spieler2 = input("Y-Koordinate: ") -1
        feld[y_spieler2][x_spieler2] = "O"
    if spielstatus == 2:
        x_spieler2, y_spieler2 = computer(feld, n)
        feld[y_spieler2][x_spieler2] = "O"

# Strategie des Computers
def computer(feld, n, smat):
    '''pos = 0
    for k in range(n):
        pos += 1
        if smat[0][k] == (n-1)*n or smat[0][k] == (n+1)*(n-1):
            break
    if pos == n:
        pos = 0
    else:
    '''    
    u = 1
    # Schaut nach, ob eine Zeile, Spalte oder Diagonale fast voll besetzt ist
    if u == 2:
        print("1")
        #for k in range(2):
            #for p in range(n):
            #if smat[k][p] == n**2+1 or smat[k][ip] == n-1:
                # Sucht in dieser Zeile oder Spalte nach dem noch freien Element

                # Besetzt dieses
    # ist das nicht so, macht er es zufaellig
    else:
        x_spieler2 = random.randint(0,n-1)
        y_spieler2 = random.randint(0,n-1)
        while feld[y_spieler2][x_spieler2] != ".":
            x_spieler2 = random.randint(0,n-1)
            y_spieler2 = random.randint(0,n-1)
    return x_spieler2, y_spieler2

# Checken, ob jemand gewonnen hat
# Hilfsmatrix fuer die Auswertung und Benutzung von win
def hilfsmat(hmat, feld, n):
    for i in range(n):
        for j in range(n):
            if feld[i][j] == ".":
                hmat[i][j] = 0
            elif feld[i][j] == "X":
                hmat[i][j] = n+1
            elif feld[i][j] == "O":
                hmat[i][j] = 1

def evalmat(hmat, n ,smat):
    # Die Summen der Spalten berechnen
    #print hmat
    for j in range(n):
        for k in range(n):
            smat[0][j] += hmat[j][k]
    # Die Summen der Zeilen berechnen
    for i in range(n):
        for t in range(n):
            smat[1][i] += hmat[t][i]
    # Die Summen der Diagonalen ermitteln
    for q in range(n):
            smat[2][0] += hmat[q][q]
    for r in range(n):
        smat[2][1] += hmat[(n-1)-r][r]
                
def win(smat, n, ende):
    for j in range(2):
        for k in range(n):
            if smat[j][k] == n:
                ende = 2
            if smat[j][k] == (n+1)*n:
                ende = 1
    if smat[2][0] == n:
        ende = 2
    if smat[2][0] == (n+1)*n:
        ende = 1
    if smat[2][1] == n:
        ende = 2
    if smat[2][1] == (n+1)*n:
        ende = 1
    return ende

def tie(feld, n, ende):
    h = 0
    for j in range(n):
        for k in range(n):
            if feld[j][k]!= ".":
                h += 1
    if h == n**2:
        ende = 3
    return ende

# MAIN
# Spieler 1 beginnt
spieler = 2
# Abfrage, in welchen Dimensionen das Spiel gespielt werden soll
n = input("In welchen Dimensionen soll das Spiel gespielt werden? ")
# Hilfsmatrix zur Evaluierung wird angelegt
hmat = range(n)
for t in range(n):
    hmat[t] = range(n)
# in smat werden die summen der zeilen, spalten und diagonalen gespeichert
# smat[1] sind Zeilen, smat[2] sind Spalten und smat[3] die Diagonalen
smat = range(3)
smat[0] = range(n)
smat[1] = range(n)
smat[2] = range(2)
resetsmat(smat, n)
feld = createmat(n)
fillmat(feld, n)
printmat(feld,n)
# Abfrage, ob gegen einen Menschen oder einen Computer gespielt wird
spielstatus = input("Soll gegen einen Computer (2) oder einen anderen Spieler (1) gespielt werden? ")
ende = 0
# Fall 1: Spieler vs. Spieler
while ende == 0:
    if spielstatus == 1:
        spieler = 3 - spieler
        print("Spieler", spieler, "ist am Zug")
        # Fragt den Spieler nach seinem n�chsten Zug
        if spieler == 1:
            # Eingabe von S1 wird aufgenommen
            x_spieler1 = input("X-Koordinate: ") - 1
            y_spieler1 = input("Y-Koordinate: ") - 1
            # Das Feld wird gefuellt bzw. s.o.
            fill_1(feld, n, x_spieler1, y_spieler1)
        if spieler == 2:
            # Eingabe von Spieler 2 wird aufgenommen
            x_spieler2 = input("X-Koordinate: ") - 1
            y_spieler2 = input("Y-Koordinate: ") - 1
            fill_2(feld, n, x_spieler2, y_spieler2, spielstatus)
        printmat(feld, n)
        resetsmat(smat,n)
        hilfsmat(hmat, feld, n)
        evalmat(hmat, n, smat)
        ende = win(smat, n, ende)
        # Tie funktioniert noch nicht
        ende = tie(feld, n, ende)
    # Fall 2: Spieler vs. Computer
    if spielstatus == 2:
        spieler = 3 - spieler
        if spieler == 1:
            print("Du bist am Zug!")
            x_spieler1 = input("X-Koordinate: ") -1
            y_spieler1 = input("Y-Koordinate: ") -1
            # Das Feld wird gefuellt bzw. s.o.
            fill_1(feld, n, x_spieler1, y_spieler1)
        if spieler == 2:
            x_spieler2, y_spieler2 = computer(feld,n)
            fill_2(feld, n, x_spieler2, y_spieler2, spielstatus)
            printmat(feld, n)
        resetsmat(smat,n)
        hilfsmat(hmat, feld, n)
        evalmat(hmat, n, smat)
        ende = win(smat, n, ende)
        ende = tie(feld, n, ende)
            
# Ende des Spiels durch Sieg
if ende == 1:
    print
    printmat(feld,n)
    print("Spieler 1 hat gewonnen!")
    time.sleep(3)
if ende == 2:
    print
    printmat(feld,n)
    print("Spieler 2 hat gewonnen!")
    time.sleep(3)
# Ende durch Untentschieden
if ende == 3:
    print()
    printmat(feld,n)
    print("Das Spiel ging unentschieden aus!")