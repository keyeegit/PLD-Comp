grammar ifcc;

axiom : func_def+ EOF ;

func_def : type ID '(' param_list? ')' block ;
param_list : param (',' param)* ;
param : 'int' ID ;
type : 'int' | 'void' ;

stmt 
    : decl_stmt 
    | assign_stmt 
    | array_assign_stmt
    | return_stmt
    | putchar_stmt
    | getchar_stmt 
    | if_stmt
    | while_stmt
    | block
    | call_stmt
    ;

call_stmt: ID '(' (expr (',' expr)*)? ')' ';' ;
block : '{' stmt* '}' ;
decl_stmt : 'int' decl_item (',' decl_item)* ';' ;
decl_item : ID ('[' CONST ']')? ('=' expr)? ;
assign_stmt : ID '=' expr ';' ;
array_assign_stmt : ID '[' expr ']' '=' expr ';' ;
putchar_stmt : 'putchar' '(' expr ')' ';' ;
getchar_stmt : 'getchar' '(' ')' ';' ;
if_stmt : 'if' '(' expr ')' stmt ('else' stmt)? ;
while_stmt : 'while' '(' expr ')' stmt ;

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
    | expr '&&' expr                    # logicalAndExpr
    | expr '||' expr                    # logicalOrExpr
    | '(' expr ')'                      # parenExpr
    | ID '(' (expr (',' expr)*)? ')'    # callExpr
    | ID '[' expr ']'                   # arrayAccessExpr
    | ID                                # idExpr
    | CONST                             # constExpr
    | CHAR_CONST                        # charConstExpr
    | 'getchar' '(' ')'            # getcharExpr
    ;

return_stmt : RETURN expr? ';' ;

RETURN : 'return' ;
CONST : [0-9]+ ;
CHAR_CONST : '\'' (~['\\\r\n] | '\\' .) '\'' ;
ID : [a-zA-Z][a-zA-Z0-9]* ;
COMMENT : '/*' .*? '*/' -> skip ;
LINE_COMMENT : '//' ~[\r\n]* -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);

