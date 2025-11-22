grammar RegEx;

RE : RE '|' RE | RE RE | RE OP | '('RE')' | [0-9] ;
OP : '*' | '+' | '?' ;
