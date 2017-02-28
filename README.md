# SimpleBasic

I intended to write my first human readable programming
language interpreter (in opposite to BF and FALSE, although
the latter is pretty nice).
The BASIC dialect was chosen just because it is kind of
easy to implement.

And so I did it.

### Commands

This dialect works with integer numbers and supports only
general BASIC commands, *at the moment*:

- `PRINT <string>/<expr>; ...` print text strings and/or expression results
- `INPUT <string>; <var>` print prompt string and request number for input
- `GOTO <expr>` go to line, which number is evaluated as expression
- `GOSUB <expr>` go to subroutine
- `RETURN` return from subroutine
- `IF <expr> THEN <command>` if conditional expression is true, execute whatever command is after
- `LET <var> = <expr>` evaluate expression and store the result into variable.
  This operator is optional.
- `END` stop execution
- `STOP` stop execution
- `DIM <var> (<expr>)` declare an array

You can add comments to code by writing `'` character at the beginning
of the line (after line number).

#### Expressions

Order of operations is left-to-right, sequentual.
If you wish to mark the order, use parenthesis. Keep this
in mind, especially when writing conditionals, as all operators are
at the same level.

#### Interactive mode

In interactive mode you can:

- `RUN` run the entered program
- `LIST` program's source code
- `NEW` clear the program
- `SYSTEM` exit to OS

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
70 C = A + B
80 A = B
90 B = C
100 COUNT = COUNT + 1
110 IF COUNT < 10 THEN GOTO 60
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

### Bugs

Tonns of them. But if it is something that does not stick out,
I consider it a hidden feature.

### License

GNU GPL v3
