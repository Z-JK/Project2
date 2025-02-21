#include "PARSE.H"
using std::pair;
static TT token;
static Graph* curGraph = nullptr;
static vector<Edge> tokenList;
static pair<int, int> Rexp();
static pair<int, int> Or();
static pair<int, int> And();
static pair<int, int> Clos();
static void addToken(const string &s);
static void addToken(const char &ch);
static void addToken(const int &num);

static void syntaxError(const string &message)
{
	out << "\n>>> Syntax error at line" << lineno << ": " << message;
	Error = true;
}

static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else 
		syntaxError("unexpected token occur!\n");
}

pair<int, int> Rexp()
{
	pair<int, int> left = Or();
	pair<int, int> right;
	pair<int, int> op;
	while (token == TT::OR)
	{
		match(TT::OR);
		right = Or();
		op.first = curGraph->addNode();
		op.second = curGraph->addNode();
		
		curGraph->addEdge(op.first, left.first, -1); // 表示epsillion 传递
		curGraph->addEdge(op.first, right.first, -1); // 表示epsillion 传递
		curGraph->addEdge(left.second, op.second, -1); // 表示epsillion 传递
		curGraph->addEdge(right.second,op.second, -1); // 表示epsillion 传递

		left = op;
	}
	return left;
}

pair<int, int> Or()
{
	pair<int, int> left = And();
	pair<int, int> right;
	while ( token == TT::LPARENT|| token == TT::NAME || 
			token == TT::STRING || token == TT::CHAR ||
			token == TT::NUM    || token == TT::SQUARE)
	{
		right = And();
		
		curGraph->addEdge(left.second, right.first, -1); // 表示epsillion 传递
		left.second = right.second;

	}
	return left;
}
pair<int, int> And()
{
	pair<int, int> left = Clos();
	pair<int, int> op;
	while ( token == TT::CLOSURE || token == TT::POSCLOSURE ||
			token == TT::OPTION )
	{
		op.first = curGraph->addNode();
		op.second = curGraph->addNode();
		switch (token)
		{
		case TT::CLOSURE:
			curGraph->addEdge(op.first, op.second, -1); // 表示epsillion 传递
			curGraph->addEdge(op.first, left.first, -1); // 表示epsillion 传递
			curGraph->addEdge(left.second, left.first, -1); // 表示epsillion 传递
			curGraph->addEdge(left.second, op.second, -1); // 表示epsillion 传递
			break;
		case TT::POSCLOSURE:
			curGraph->addEdge(op.first, left.first, -1); // 表示epsillion 传递
			curGraph->addEdge(left.second, left.first, -1); // 表示epsillion 传递
			curGraph->addEdge(left.second, op.second, -1); // 表示epsillion 传递
			break;
		case TT::OPTION:
			curGraph->addEdge(op.first, left.first, -1); // 表示epsillion 传递
			curGraph->addEdge(left.second, op.second, -1); // 表示epsillion 传递
			curGraph->addEdge(op.first, op.second, -1); // 表示epsillion 传递
			break;
		default:
			break;
		}

		match(token);
		left = op;
	}
	return left;
}

pair<int, int> Clos()
{
	pair<int, int> left; 
	pair<int, int> right; 
	switch (token)
	{
	case TT::LPARENT:
		match(TT::LPARENT);
		left = Rexp();
		match(TT::RPARENT);
		break;
	case TT::CHAR:
		left.first = curGraph->addNode();
		left.second = curGraph->addNode();
		curGraph->addEdge(left.first, left.second, tokenString[0]);
		addToken(tokenString[0]);
		match(TT::CHAR);
		break;
	case TT::NAME:
		left.first = curGraph->addNode();
		left.second = curGraph->addNode();
		curGraph->addEdge(left.first, left.second, tokenString);
		addToken(tokenString);
		match(TT::NAME);
		break;
	case TT::STRING:
		left.first = curGraph->addNode();
		left.second = curGraph->addNode();
		curGraph->addEdge(left.first, left.second, tokenString[0]);
		addToken(tokenString[0]);
		for (int i = 1; i < tokenString.length(); i++)
		{
			right.first = curGraph->addNode();
			right.second = curGraph->addNode(); 
			curGraph->addEdge(right.first, right.second, tokenString[i]);
			addToken(tokenString[i]);
			curGraph->addEdge(left.second, right.first, -1);
			left.second = right.second;
		}
		match(TT::STRING);
		break;
	case TT::NUM:
		left.first = curGraph->addNode();
		left.second = curGraph->addNode();
		curGraph->addEdge(left.first, left.second, atoi(&tokenString[0]));
		addToken(atoi(&tokenString[0]));
		for (int i = 1; i < tokenString.length(); i++)
		{
			right.first = curGraph->addNode();
			right.second = curGraph->addNode(); 
			curGraph->addEdge(right.first, right.second, atoi(&tokenString[i]));
			addToken(atoi(&tokenString[i]));
			curGraph->addEdge(left.second, right.first, -1);
			left.second = right.second;
		}
		match(TT::NUM);
		break;
	case TT::SQUARE:
		left.first = curGraph->addNode();
		left.second = curGraph->addNode();

		for (int i = 1; i < tokenString.length()-1; i++)
		{
			if(tokenString[i+1] == '-')
			{
				int l = (int)tokenString[i++];
				int r = (int)tokenString[i++ + 1];
				char ch;
				bool digit = isdigit(r);
				for (; l <= r; l++)
				{
					ch = (char)l;
					if (digit)
					{
						curGraph->addEdge(left.first, left.second, atoi(&ch));
						addToken(atoi(&ch));
					}
					else
					{
						curGraph->addEdge(left.first, left.second, ch);
						addToken(ch);
					}
				}
			}
			else if(isdigit(tokenString[i]))
			{
				curGraph->addEdge(left.first, left.second, atoi(&tokenString[i]));
				addToken(atoi(&tokenString[i]));
			}
			else if (tokenString[i] != '-')
			{
				curGraph->addEdge(left.first, left.second, tokenString[i]);
				addToken(tokenString[i]);
			}
		}
		match(TT::SQUARE);
		break;
	default:
		break;
	}
	return left;
}


Graph* parse(int &s, int &e, vector<Edge>& tL)
{
	curGraph = new Graph;
	tokenList = vector<Edge>(0);
	token = getToken();
	pair<int, int> p = Rexp();
	s = p.first;
	e = p.second;
	tL = tokenList;
	return curGraph;
}

static void addToken(const int& num)
{
	for (int i = 0; i < tokenList.size(); i++)
		if (tokenList[i].type_ == TT::NUM)
			if (tokenList[i].num_ == num) return;
	tokenList.push_back(Edge(num));
}

static void addToken(const char& ch)
{
	for (int i = 0; i < tokenList.size(); i++)
		if (tokenList[i].type_ == TT::CHAR)
			if (tokenList[i].ch_ == ch) return;
	tokenList.push_back(Edge(ch));
}

static void addToken(const string& s)
{
	for(int i= 0; i < tokenList.size(); i++)
		if (tokenList[i].type_ == TT::NAME)
			if (*tokenList[i].s_ == s) return;
	string* s_ = new string(s);
	tokenList.push_back(Edge(s_));
}
