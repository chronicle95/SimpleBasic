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
- `RETURN` return from subroutine
- `IF .. THEN ..` if conditional expression is true, execute whatever command is after THEN
- `LET .. = ..` evaluate expression and store the result into variable
- `END` stop execution
- `STOP` stop execution

You can add comments to code by writing `'` character at the beginning
of the line (after line number).

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

Here is how you would calculate and print first 10 fibonacchi numbers:
```
10 ' Program: Fibonacci sequence
20 LET A = 1
30 LET B = 1
40 LET COUNT = 0
50 ' Semicolon at the end will omit the line feed
60 PRINT A;
70 LET C = A + B
80 LET A = B
90 LET B = C
100 LET COUNT = COUNT + 1
110 IF COUNT < 10 THEN GOTO 40
120 ' Print EOL so that prompt comes at the next line
130 PRINT ""
140 END
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
