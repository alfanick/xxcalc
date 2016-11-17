# XXCalc

XXCalc is a scientific calculator interpreting an infix notation with
basic solving capabilities. It is written in C++ and it is meant to be
quick, robust, embeddable and easily extentable.

```
>>> (3+(4-1))*5
30
>>> 2 * x + 0.5 = 1
x=0.25
>>> 2x + 1 = 2(1-x)
x=0.25
>>> (x^3+2x-1)^3
x^9+6x^7-3x^6+12x^5-12x^4+11x^3-12x^2+6x-1
>>> bind(ans, 1)
8
```


## Features

The main program `xxcalc` is a command line linear solver capable of:

* addition, subtraction, multiplication, division and exponentiation of polynomials,
* supporting of nested parentheses,
* solving linear equations,
* supporting pi and e constants,
* supporting `log10(number)`, `log(number, base)` functions,
* evaluating polynomials using `bind(expression, value)`,
* reporting variety of errors to user.

This program can perform arithmetic operations on polynomials of any
degree (as long as a result of the operation is still a polynomial).
However, solving is implemented only for linear equations (polynomials
of degree 1 at most).

If libreadline is installed, command line interface is a bit interactive
and supports history, otherwise a basic standard input and output
methods are used.


## Build instructions

In order to build the program following requirements must be satisfied:

* compiler of C++11 standard (clang or gcc),
* CMake 3.0+ build tool,
* git client (for unit test library),
* optionally libreadline for pretty input.

The project uses cmake build scripts, however usage of them can be
simplified - assuming the dependencies are satisfied, `./build.sh` will
compile the program into bin directory.


## Basis of operation

The solver is split into multiple extensible functional units. The user
can embed the solver into his own program and modify its behaviour on
variety of levels.

The very first step is a `Tokenizer` - it takes an input as a text
string and performs tokenization (lexing) process. Tokens are smallest
complete pieces detected in the input, such as operators, numbers
(including scientfic notation), identifiers or parentheses. Such
tokenized input is given to a `Parser` - the parser transforms the input
from common infix form into Reverse Polish Notation using the linear
time Shunting Yard algorithm. The input obeys rules of associativity and
precedence for operators, which must be registered beforehand. Using
`xxcalc-debug` one can observe output of tokenizer and parser.

Parsed tokens in RPN form are evaluated using a `Evaluator`. Evaluator
takes the input and transforms it into computed value. The evaluator
support constants and multiple argument functions. The basic unit of
data used in computations is a `Value` which represents a polynomial.
This allows to do complex symbolic operations with ease and abstracts
other parts of software from the implementation of these operations. The
value is a well designed class with support for many operators, which
makes interaction with this type a comfortable operation.

Initially the evaluator has no defined functions or constants. A
`PolynomialCalculator` is providing basic arithmetic operations, log
functions and some constants - they are registered with the parser and
the evaluator. A constant `x` represents a polynomial of degree 1 and
coefficient 1 which makes inputting polynomials a familiar experience.

A `LinearSolver` extends the polynomial calculator with a basic solving
capabilities. A new operator `=` is defined with the lowest precedence -
it takes its operands  and using basic arithmetic operations extracts
value of the symbol `x` in a linear expression. This value is returned
as the output of `=` operation. This makes the implementation very clean
and readable as there is no difference between common computation or
solving expressions.


## Unit testing

The project is heavily unit tested using Catch unit test framework. Unit
test can be executed using `xxcalc-test`. The library is automatically
downloaded with a CMake build script.


## Authors

* Amadeusz Juskowiak <juskowiak@amdeusz.me>
