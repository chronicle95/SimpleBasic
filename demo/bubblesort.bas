10 DIM A(10)
20 PRINT "Enter ten numbers:"
30 I = 0
40 INPUT N
50 A[I]=N
60 I = I + 1
70 IF I < 10 THEN GOTO 40
80 ' Sort the array
100 I = 0
110 J = 0
120 IF A(J) > A(J+1) THEN GOTO 250
130 J = J + 1
140 IF J < 9 THEN GOTO 120
150 I = I + 1
160 IF I < 10 THEN GOTO 110
170 ' Print the array
180 PRINT "Sorted:"
190 I = 0
200 PRINT A(I);" ";
210 I = I + 1
220 IF I < 10 THEN GOTO 200
230 PRINT ""
240 END
250 Z = A(J)
260 A[J] = A(J+1)
270 A[J+1] = Z
280 GOTO 130
