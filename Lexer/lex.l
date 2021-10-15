%{
enum TYPE
{
    AUTO, SHORT, INT, LONG,
    FLOAT, DOUBLE, CHAR, STRUCT,
    UNION, ENUM, TYPEDEF, CONST,
    UNSIGNED, SIGNED, EXTERN, REGISTER,
    STATIC, VOLATILE, VOID, IF,
    ELSE, SWITCH, CASE, FOR,
    DO, WHILE, GOTO, CONTINUE,
    BREAK, DEFAULT, SIZEOF, RETURN,
    VALUE, ID, SEMI, MACRO,
    COMMENT
};
    
    
    
%}

macro   #(.)*
comment (\/\/(.)*)|(\/\*(.)*(\*/\/))
space	[ \n\t\r]
letter	[A-Za-z_]
digit	[0-9]
id	    {letter}({letter}|{digit})*
bin     0(b|B)(0|1)+
oct     0([0-7])+
hex     0(x|X)([0-9A-Fa-f])+
dec	    (({digit}+(\.{digit}*)?)|(\.{digit}+))((E|e)[+-]?{digit}+)?
num     ({bin}|{oct}|{hex}|{dec})

%%

auto        {return AUTO;}
short       {return SHORT;}
int         {return INT;}
long        {return LONG;}
float       {return FLOAT;}
double      {return DOUBLE;}
char        {return CHAR;}
struct      {return STRUCT;}
union       {return UNION;}
enum        {return ENUM;}
typedef     {return TYPEDEF;}
const       {return CONST;}
unsigned    {return UNSIGNED;}
signed      {return SIGNED;}
extern      {return EXTERN;}
register    {return REGISTER;}
static      {return STATIC;}
volatile    {return VOLATILE;}
void        {return VOID;}
if          {return IF;}
else        {return ELSE;}
switch      {return SWITCH;}
case        {return CASE;}
for         {return FOR;}
do          {return DO;}
while       {return WHILE;}
goto        {return GOTO;}
continue    {return CONTINUE;}
break       {return BREAK;}
default     {return DEFAULT;}
sizeof      {return SIZEOF;}
return      {return RETURN;}

;           {return SEMI;}

{space}	    {;}
{macro}     {return MACRO;}
{comment}   {return COMMENT;}
{num}       {return VALUE;}
{id}        {return ID;}


%%

void output(int c)
{
    if (c <= RETURN)
        printf("<%s, %s>\n", yytext, yytext);
    else if (c == VALUE)
        printf("<value, %s>\n", yytext);
    else if (c == ID)
        printf("<id, %s>\n", yytext);
    else if (c == SEMI)
        printf("<semi, %s>\n", yytext);
    else if (c == MACRO || c == COMMENT)
        ;
    else
        printf("<error, %s>\n", yytext);
}


int main(int argc, char** argv)
{
    if (argc >= 2)
    {
        if ((yyin = fopen(argv[1], "r")) == NULL)
        {
            printf("Can not read file %s\n", argv[1]);
            return 0;
        }
    }
    int c;
    while (c = yylex())
    {
        output(c);
    }
    fclose(yyin);
}
