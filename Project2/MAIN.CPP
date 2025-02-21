#include "GLOBALS.H"
#include "SCANNER.H"
#include "REXP.H"

std::ifstream in;
std::ofstream out;

int lineno = 0;
bool Error = false;
int main()
{
	in.close();
	in.open("in.txt", std::ios::in);
	out.close();
	out.open("out.txt", std::ios::out);
	
	while (loadLine()) {
		Rexp* rexp = getRexp();
		if (rexp != nullptr)
			Rexps.push_back(rexp);
	}
	for (int i = 0; i < Rexps.size(); i++)
	{
		//out << Rexps[i]->name_ <<" NFA:" << std::endl;
		//if(Rexps[i]->nfa_!= nullptr) Rexps[i]->nfa_->show();
		//out << Rexps[i]->name_ << " DFA:" << std::endl;
		/*for (int j = 0; j < Rexps[i]->tokenList.size(); j++)
		{
			if (Rexps[i]->tokenList[j].type_ == TT::NUM)
				out << Rexps[i]->tokenList[j].num_ << "\t";
			else if (Rexps[i]->tokenList[j].type_ == TT::CHAR)
				out << Rexps[i]->tokenList[j].ch_ << "\t";
			else if (Rexps[i]->tokenList[j].type_ == TT::NAME)
				out << *Rexps[i]->tokenList[j].s_ << "\t";
		}*/
		out << Rexps[i]->name_ << " DFA:" << std::endl;
		if(Rexps[i]->dfa_ != nullptr) Rexps[i]->dfa_->showSet(Rexps[i]->statesSet);
		out << std::endl;
	}

	
	return 0;
}