# SimpleBasic

I intended to write a programming language interpreter.
The BASIC dialect was chosen just because it is kind of
easy to implement.

And so I did it.

### Commands

This dialect works with integer numbers and supports only
general BASIC commands, at the moment:

- `PRINT` ..
- `INPUT` ..
- `GOTO` ..
- `GOSUB` ..
- `IF` .. `THEN` ..
- `LET` ..
- `END`
- `STOP`

### Hello World

```
SimpleBasic
Ok
10 PRINT "Hello, World!"
20 END
RUN
Hello, World!
Ok
```

