grammar RegEx;

re : single = re OP |
    left = re right = re |
    left = re or = '|' right = re |
    '('single = re')' |
    CHAR;
OP : '*' | '+' | '?' ;
CHAR : [0-9];
