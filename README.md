# ceval
is a small library that allows you to get the result of a string that contains a math calculation.

## Description
ceval is a small library that allows you to get the result of a string that contains a math calculation.
Here is an example ```
printf("%f", ceval("2*7/1.5 * 1.4"));``` -> ```13.066666```
ceval follows all mathematical rules.
Currently supported operands are

1. ()
2. /
3. \*
4. \+
5. \-

this is also the way they are executed.
This also allows full floating point numbers.

## Usage

To generate a library file in the `output` folder
```
make
```
```
make lib
```

To run some examples
```
make run
```
