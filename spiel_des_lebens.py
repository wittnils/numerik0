import random, os, time
# SUB
# erstellt die Matrix, die wir brauchen
def createmat(n):
    matA = range(n+2)
    matB = range(n+2)
    for i in range(0, n+2):
        matA[i] = range(n+2)
        matB[i] = range(n+2)
    return matA, matB # Matrizen wieder auffangen

def clearmat(A, n):
    for i in range(n+2):
        for j in range(n+2):
            A[i][j] = 0
    return A

def initmat(A,n):
    for i in range(1,n+1):
        for j in range(1,n+1):
            A[i][j] = random.randint(0,1)
    return A
            
def printmat(A,n):
	for i in range(1,n+1):
		for j in range(1,n+1):
			if A[i][j] == 1:
				print("X"),  
			else:
				print ("."),
		print

def evalmat(A,B,n):
	for i in range(1,n+1):
		for j in range(1, n+1):
			B[i][j] = A[i-1][j-1] + A[i][j-1] + A[i+1][j-1] + A[i+1][j] + A[i-1][j] + A[i-1][j+1] + A[i][j+1] + A[i+1][j+1] 
	return B

def newgen(A,B,n):
	for i in range(1, n+1):
		for j in range(1, n+1):
			if B[i][j] < 2 or B[i][j] > 3:
				A[i][j] = 0
			elif (B[i][j] == 2 and A[i][j] == 1) or B[i][j] == 3:
				A[i][j] = 1 

# MAIN
n = int(input("n = "))
x = int(input("Anzahl Generationen = ")
print
A,B = createmat(n)
A = clearmat(A,n)
B = clearmat(B,n)
A = initmat(A,n)
print
i = 0
while i != x:
	B = evalmat(A,B,n)
	newgen(A,B,n)
	print("Generation:"), i+1
	time.sleep(0.2)
	os.system("cls")
	printmat(A,n)
	print
	time.sleep(.3)
	i += 1
