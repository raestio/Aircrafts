# PLANES
A program processes the data from the plane's identification.

Input in EBNF language:
    
    input      ::= { whiteSpace } { msg { whiteSpace } }
    whiteSpace ::= ' ' | '\t' | '\n' | '\r'
    msg        ::= integer { whiteSpace } ':' { whiteSpace } '[' list ']' { whiteSpace }
    list       ::= '[' { whiteSpace } integer { listRest } { whiteSpace } ']' { whiteSpace }             
    listRest   ::= { { whiteSpace } ','  { whiteSpace } integer }
    integer    ::= ['+' | '-'] digit { digit }
    digit      ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'


# Examples:
1)
IN:

    0 : [ 1, 8, 56, -9 ]
    666 : [ 42, 7, 11, -96, 8 ]
    42 : [ 9, 3, 6, 8, 12, 9, 3 ]
    15 : [ 8, 12, 9 ]
    666 : [ 56, -9, 1, 8, 56, -9 ]
    0 : [ 83, 42, 7 ]
    21:[3, 6, 8, 12, 9, 3, 6, 8, 12]
    63 : [ 8, 12, 9, 3, 6, 8 ]
    15 : [3]
    666 : [83]
    42: [6, 8, 12, 9, 3, 6, 8, 12]
    0: [11, -96, 8, 56, -9 ]
    15  :   [   6   , 8, 12, 9, 3, 6, 8, 12, 9, 3, 6]
    21 : [9, 3, 6, 8, 12, 9 ]
    31 : [ 8, 11, 9, 3, 6, 8 ]
        
OUT:

    0, 666
    42, 15, 21
    31

2)
IN:

    0 : [ 1, 2, 3, 4, 5 ]
    1 : [ 2, 1, 3, 4, 5 ]
    2 : [ 5, 1, 4, 2, 3 ]
    3 : [ 3, 4, 5, 1, 2 ]
    4 : [ 5, 3, 1, 4, 2 ]
    10000 : [ 4, 5, 2, 1, 3]

OUT:

    1, 10000
    4
    2
    0, 3

3)
IN:

    0 : [ 1, 2, 4 ]
    1 : [ 2, 1 ]
    2 : [ 2, 3, 1 ]
    3 : [ 1, 2 ]
    4 : [ 2, 4, 1]
   
OUT:

    0, 4
    2
    1, 3

