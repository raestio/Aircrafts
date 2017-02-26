# AIRCRAFTS
A program processes the data from the aircraft's identification.

Modern aircraft are equipped with an automatic system that transmits basic flight data (identification, speed, ...).
This data has a finite size and are sent over and over again. For reasons of redundancy, the data are transmitted independently on different frequencies.
We assume that we have receivers that can catch that broadcast. Each receiver processes the data on one frequency and receives identification from just one plane. The receiver receives the flight (identification) data sequentially, after sufficient amount
of data is received the data will be transmitted to our program for processing. This is repeated several times until the entire
period of flight data is processed. Thus, if the aircraft data are 5, 12, 9, 8, 64, 93, the receiver may deliver identification e.g. in 3 parts:

5, 12

9, 8, 64

93

This is the way how this program receives flight (identification) data from individual receivers. Received data parts the program builds via the Red Black tree (by ID of receiver) and compare with others. Its goal is to identify duplicates, so identify which receivers receive data from the same aircraft. The situation is complicated by the fact that the receivers may not be synchronized. If the aircraft transmits data for example: 5, 12, 9, 8, 64, 93, the one receiver can capture it in the form: 5, 12, 9, 8, 64, 93, but the the receiver operating on a different can receive the data with offset: 9, 8, 64, 93, 5, 12. But these two messages are considered to be identical.

The output of the program are the identification duplicates. On the one line of the output is the list of receivers (IDs), which receive the flight (identification) data from the same aircraft.

Reading the data ends with reaching the EOF.

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

