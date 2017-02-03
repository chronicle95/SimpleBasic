# SimpleBasic

I intended to write my first human readable programming
language interpreter (in opposite to BF and FALSE, although
the latter is pretty nice).
The BASIC dialect was chosen just because it is kind of
easy to implement.

And so I did it.

### Commands

This dialect works with integer numbers and supports only
general BASIC commands, at the *moment*:

- `PRINT ..` print text strings and/or expression results
- `INPUT ..` print prompt string and request number for input
- `GOTO ..` go to line, which number is evaluated as expression
- `GOSUB ..` go to subroutine
- `IF .. THEN ..` if conditional expression is true, execute whatever command is after THEN
- `LET .. = ..` evaluate expression and store the result into variable
- `END` stop execution
- `STOP` stop execution

In interactive mode you can:

- `RUN` run the entered program
- `LIST` print program's source code
- `NEW` clear the program

### Examples

No comments:
```
SimpleBasic
Ok
10 PRINT "Hello, World!"
20 END
RUN
Hello, World!
Ok
```

Here is how you would calculate first 10 fibonacchi numbers:
```
10 LET A = 1
20 LET B = 1
30 LET COUNT = 0
40 PRINT A
50 LET C = A + B
60 LET A = B
70 LET B = C
80 LET COUNT = COUNT + 1
90 IF COUNT < 10 THEN GOTO 40
100 END
```

### Internals

Under the hood it is very crude. One of the reasons is that
the interpreter is targeted for devices that are restricted
on resources, especially, lacking the memory.
Usually when you write an interpreter or a compiler, you
read the source code line by line and convert it to some
internal representation (like AST) and only then you run it
or translate to machine codes. For an interpreter you would
have some sort of virtual machine implemented, aside from the
parser.
But here I decided to remove the unnecessary step and run the
code directly from source! Very funny, but in fact it works.
