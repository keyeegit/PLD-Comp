grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' stmt* return_stmt '}' ;

stmt : decl_stmt | assign_stmt ;
decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : ID ('=' expr)? ;
assign_stmt : ID '=' expr ';' ;

expr
    : '-' expr                          # unaryMinusExpr
    | '!' expr                          # notExpr
    | expr op=('*'|'/'|'%') expr        # mulDivExpr
    | expr op=('+'|'-') expr            # addSubExpr
    | expr op=('<'|'>') expr            # relCmpExpr
    | expr op=('=='|'!=') expr          # eqCmpExpr
    | expr '&' expr                     # bitAndExpr
    | expr '^' expr                     # bitXorExpr
    | expr '|' expr                     # bitOrExpr
    | '(' expr ')'                      # parenExpr
    | ID                                # idExpr
    | CONST                             # constExpr
    | CHAR_CONST                        # charConstExpr
    ;

return_stmt : RETURN expr ';' ;

RETURN : 'return' ;
CONST : [0-9]+ ;
CHAR_CONST : '\'' (~['\\\r\n] | '\\' .) '\'' ;
ID : [a-zA-Z][a-zA-Z0-9]* ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
