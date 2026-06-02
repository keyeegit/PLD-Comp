grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* return_stmt '}' ;

stmt : decl_stmt | assign_stmt ;
decl_stmt : 'int' ID ('=' expr)? ';' ;
assign_stmt : ID '=' expr ';' ;

expr
    : '-' expr                     # unaryMinusExpr
    | expr op=('*'|'/'|'%') expr   # mulDivExpr
    | expr op=('+'|'-') expr       # addSubExpr
    | '(' expr ')'                 # parenExpr
    | ID                           # idExpr
    | CONST                        # constExpr
    ;

return_stmt : RETURN expr ';' ;

RETURN : 'return' ;
CONST : [0-9]+ ;
ID : [a-zA-Z][a-zA-Z0-9]* ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
