# recursive-descent-parser
A simple Recursive Descent Parser for an EBNF grammar of a simple arithmetic expression.

## EBNF for Arithmetic Expression
    N = { expr, term, factor, number, digit }
    T = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, +, *, (, ) }
    P = Productions:
      * <expr> --> <term> { + <term> }
      * <term> --> <factor> { * <factor> }
      * <factor> --> ( <expr> ) | <number>
      * <number> --> <digit> { <digit> }
      * <digit> --> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    S --> <expr>
