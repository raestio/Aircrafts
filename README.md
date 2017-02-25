# Planes
A program processes the data from the plane's identification.

Input in EBNF language:
    
    input      ::= { whiteSpace } { msg { whiteSpace } }
    whiteSpace ::= ' ' | '\t' | '\n' | '\r'
    msg        ::= integer { whiteSpace } ':' { whiteSpace } '[' list ']' { whiteSpace }
    list       ::= '[' { whiteSpace } integer { listRest } { whiteSpace } ']' { whiteSpace }             
    listRest   ::= { { whiteSpace } ','  { whiteSpace } integer }
    integer    ::= ['+' | '-'] digit { digit }
    digit      ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
