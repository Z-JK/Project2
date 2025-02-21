#include "SCANNER.H"
#include "REXP.H"

/* states in scanner DFA */
typedef enum class StateType
{
    START, INNUM, INCHAR, INSTRING, INTRANS, INSQUARE, DONE
}ST;

string tokenString;

static string lineBuf; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static bool EOF_flag = false; /* corrects ungetNextChar behavior on EOF */
static bool EOL_flag = false;
const int EOL = -1;

void printLineBuf()
{
    out << lineBuf;
}

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */

bool loadLine(void) 
{
    if (in.eof())
    {
        EOF_flag = true;
        return false;
    }
    else 
    {
        getline(in, lineBuf);
        linepos = 0;
        bufsize = lineBuf.length();
        lineno++;
        EOL_flag = false;
        return true;
    }
}

static int getNextChar(void)
{
    if (!(linepos < bufsize))
    {
        EOL_flag = true;
        return EOL;
    }
    else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{
    if (!EOF_flag && !EOL_flag) linepos--;
}

/* uses linear search */
static bool nameLookup(string& s)
{
    return names.find(s) != names.end();
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{
    /* holds current token to be returned */
    TokenType currentToken;
    /* current state - always begins at START */
    StateType state = ST::START;
    /* flag to indicate save to tokenString */
    tokenString = "";
    bool save;
    while (state != ST::DONE)
    {
        int c = getNextChar();
        save = true;
        switch (state)
        {
        case ST::START:
            if (isdigit(c))
                state = ST::INNUM;
            else if (isalpha(c))
                state = ST::INSTRING;
            else if (c == '\\')
            {
                save = false;
                state = ST::INTRANS;
            }
            else if (c == '[')
                state = ST::INSQUARE;
            else if ((c == ' ') || (c == '\t') || (c == '\n'))
                save = false;
            else
            {
                state = ST::DONE;
                switch (c)
                {
                case EOL:
                    currentToken = TT::ENDLINE;
                    break;
                case '=':
                    currentToken = TT::EQ;
                    break;
                case '+':
                    currentToken = TT::POSCLOSURE;
                    break;
                case '*':
                    currentToken = TT::CLOSURE;
                    break;
                case '&':
                    currentToken = TT::AND;
                    break;
                case '|':
                    currentToken = TT::OR;
                    break;
                case '?':
                    currentToken = TT::OPTION;
                    break;
                case '(':
                    currentToken = TT::LPARENT;
                    break;
                case ')':
                    currentToken = TT::RPARENT;
                    break;
                case '_':
                    currentToken = TT::UNDERLINE;
                    break;
                default:
                    currentToken = TT::ERROR;
                    break;
                }
            }
            break;
       
        case ST::INNUM:
            if (!isdigit(c))
            { /* backup in the input */
                ungetNextChar();
                save = false;
                state = ST::DONE;
                currentToken = TT::NUM;
            }
            break;
        case ST::INSTRING:
            if (!isalpha(c))
            { /* backup in the input */
                ungetNextChar();
                save = false;
                state = ST::DONE;
                currentToken = TT::STRING;
            }
            break;
        case ST::INSQUARE:
            if (c == ']')
            {
                state = ST::DONE;
                currentToken = TT::SQUARE;
            }
            break;
        case ST::INTRANS:
            state = ST::DONE;
            switch (c)
            {
            case '\\':
            case '+':
            case '&':
            case '|':
            case '?':
            case '[':
            case ']':
            case '(':
            case ')':
                currentToken = TT::CHAR;
                break;
            default:
                ungetNextChar();
                save = false;
                currentToken = TT::ERROR;
                break;
            }
            break;
        case ST::DONE:
        default: /* should never happen */
            out << "Scanner Bug: state=" << (int)state << std::endl; 
            state = ST::DONE;
            currentToken = TT::ERROR;
            break;
        }
        if (save)
            tokenString += (char)c;
        if (state == ST::DONE)
        {
            if (currentToken == TT::STRING)
                if(nameLookup(tokenString)) currentToken = TT::NAME;
        }
    }
    return currentToken;
} /* end getToken */

