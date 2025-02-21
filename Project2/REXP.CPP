#include "REXP.H"
const int MAX_STATE_SIZE = 100;
std::vector<Rexp*> Rexps = std::vector<Rexp*>(0);
std::map<string, int> names;


/* states in getRexp DFA */
typedef enum class StateType
{
	START, INUNDERLINE, INNAME, INCODE, INS, DONE
}ST;

static int stringToNum(const string& s)
{
	int result = atoi(&s[0]);
	for (int i = 0; i < s.length(); i++)
	{
		result *= 10;
		result += atoi(&s[i]);
	}
	return result;
}

static enum class ErrType 
{
	NO_SPACE, DUPLICATED_NAME, WRONG_NAME_FORMAT, WRONG_REXP_FORMAT
};



static ErrType errType = ErrType::NO_SPACE;
static Rexp* curRexp = nullptr;
static Graph* dfa(int s, vector<set<int>> &set);
static Graph* ldfa(const Graph* dfa, const vector<Edge>* tokenList, const set<int>* s2, vector<set<int>> &s);

static Graph* getNfa(void);
Rexp* getRexp()
{
	Rexp* newRexp = new Rexp;
	if (newRexp == nullptr)
		return newRexp;
	curRexp = newRexp;
	ST state = ST::START;
	TT token;
	int s = 0, e = 0;
	int mode = 0;
	while (state != ST::DONE)
	{
		token = getToken();
		switch (state)
		{
		case ST::START:
			if (token == TT::UNDERLINE) {
				mode = 1;
				state = ST::INUNDERLINE;
			}
			else if (token == TT::STRING)
			{
				if (names.find(tokenString) == names.end())
				{
					names[tokenString] = Rexps.size();
					newRexp->name_ = tokenString;
					state = ST::INNAME;
				}
				else {
					errType = ErrType::DUPLICATED_NAME;
					state = ST::DONE;
					newRexp = nullptr;
				}
			}
			else
			{
				errType = ErrType::WRONG_NAME_FORMAT;
				state = ST::DONE;
				newRexp = nullptr;
			}
			break;
		case ST::INUNDERLINE:
			if(token == TT::STRING)
			{
				if (names.find(tokenString) == names.end())
				{
					names[tokenString] = Rexps.size();
					newRexp->name_ = tokenString;
					state = ST::INNAME;
				}
				else {
					errType = ErrType::DUPLICATED_NAME;
					state = ST::DONE;
					newRexp = nullptr;
				}
			}
			else 
			{
				errType = ErrType::WRONG_NAME_FORMAT;
				state = ST::DONE;
				newRexp = nullptr;
			}
			break;
		case ST::INNAME:
			if (token == TT::NUM)
			{
				newRexp->code_ = stringToNum(tokenString);
				state = ST::INCODE;
			}
			else if (token == TT::EQ)
				state = ST::DONE;
			else
			{
				errType = ErrType::WRONG_NAME_FORMAT;
				state = ST::DONE;
				newRexp = nullptr;
			}
			break;
		case ST::INCODE:
			if (token == TT::EQ)
				state = ST::DONE;
			else if (token == TT::STRING && tokenString.length() == 1 && tokenString[0] == 'S')
			{
				mode = 2;
				newRexp->len_ = 1;
				state = ST::INS;
			}
			else
			{
				errType = ErrType::WRONG_NAME_FORMAT;
				state = ST::DONE;
				newRexp = nullptr;
			}
			break;
		case ST::INS:
			if (token == TT::EQ)
				state = ST::DONE;
			else
			{
				errType = ErrType::WRONG_NAME_FORMAT;
				state = ST::DONE;
				newRexp = nullptr;
			}
			break;
		case ST::DONE:
		default:
			break;
		}
	}
	if (newRexp == nullptr)
	{
		out << "Line " << lineno << ": ";
		printLineBuf();
		switch (errType)
		{
		case ErrType::NO_SPACE:
			out << " -- SPACE USED OUT" <<std::endl;
			break;
		case ErrType::DUPLICATED_NAME:
			out << " -- DUPLICATED NAME"<<tokenString <<std::endl;
			break;
		case ErrType::WRONG_NAME_FORMAT:
			out << " -- WRONG NAME FORMAT" << std::endl;
			break;
		case ErrType::WRONG_REXP_FORMAT:
			out << " -- WRONG REXP FORMAT" << std::endl;
			break;
		default:
			break;
		}
		return newRexp;
	}

	newRexp->nfa_ = parse(s, e, newRexp->tokenList);
	newRexp->s2.insert(e);
	newRexp->nfa_->getTset(newRexp->s2); //nfa 的终态
	out << newRexp->name_ << ": " << s << std::endl;
	if(!newRexp->nfa_) out<< newRexp->name_ << "!" << std::endl;
	if (mode >= 1)
	{
		newRexp->dfa_ = dfa(s, newRexp->statesSet);
		// 要处理出dfa 的终态存入s2中
		vector<bool> sarr = vector<bool>(newRexp->dfa_->getNodeSize());
		for (int i = 0; i < sarr.size(); i++)
		{
			for (auto it = newRexp->statesSet[i].begin(); it != newRexp->statesSet[i].end(); it++)
			{
				if (newRexp->s2.find(*it) != newRexp->s2.end())
				{
					sarr[i] == true;
					break;
				}
			}
		}
		vector<vector<int>> a(newRexp->tokenList.size(), vector<int>(newRexp->dfa_->getNodeSize()));
		Edge* p = nullptr;
		for (int i = 0; i < newRexp->dfa_->getNodeSize(); i++)
		{
			for (int j = 0; j < newRexp->tokenList.size(); j++)
			{
				p = newRexp->dfa_->node
			}
		}
		newRexp->ldfa_ = ldfa();
	}
	/*
	if (mode = 2)
	{
		newRexp->len_ = getLen();
	}*/

	return newRexp;
}
static Graph* ldfa(const Graph* dfa, const vector<Edge>* tokenList, const set<int>* s2, vector<set<int>> &s)
{
	Graph* newLdfa = new Graph();
	int row = dfa->getNodeSize() , col = tokenList->size();
	Edge* p = nullptr;
	long long *a = new long long [row];
	int *to = new int [row];
	for (int i = 0; i < row; i++)
	{
		a[i] = dfa->cal(i, tokenList, s2);
	}
	for (int i = 0; i < row; i++)
		to[i] = -1;
	for (int i = 0; i < row; i++)
	{
		if (to[i] != -1) continue;
		newLdfa->addNode();
		int index = newLdfa->getNodeSize() - 1;
		s.push_back(set<int>());
		s[index].insert(i);
		to[i] = index;
		for (int j = i + 1; j < row; j++)
		{
			if (to[i] != -1) continue;
			if (a[i] == a[j]) to[j] = index;
		}
	}
	dfa->modEdge(newLdfa, s, to);
	
	if(a!=nullptr) delete[] a;
	if (to != nullptr) delete[] to;
}

static Graph* dfa(int start,vector<set<int>> &stateSet)
{
	Graph* newDfa = new Graph();
	Graph* nfa = curRexp->nfa_;
	stateSet.push_back(set<int>());
	nfa->getEset(start, 0, stateSet);
	int stateIndex = newDfa->addNode();
	while (stateIndex < stateSet.size())
	{
		nfa->getNEset(newDfa, stateIndex, stateSet);
		stateIndex++;
	}
	return newDfa;
}
