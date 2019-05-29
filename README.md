# SimpleBasic

This code is meant for study only. And for fun as well.
This is my first human readable programming language interpreter.
The BASIC dialect was chosen just because it is kind of
easy to implement.

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
If you wish to mark the order of calculations, use parenthesis. Keep this
in mind, especially when writing conditionals, as all operators are
treated equal.

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

### Internals

Usually, when you write an interpreter or a compiler, you
read the source code line by line and convert it to some
internal representation (like AST) and only then you run it
or translate to machine codes. For an interpreter you would
have some sort of virtual machine implemented, apart from the
parser.
But here I decided to remove the unnecessary step and run the
code directly from source! I guess this is bad, but it works, eh.

### Bugs

Tonns of them. But if it is something that does not stick out,
I consider it a hidden feature.

### License

GNU GPL v3
