#include "GRAPH.H"

int Graph::getNodeSize()const
{
	return nodeNum_;
}


int Graph::addNode()
{
	Edge* edge = new Edge(-1);
	if(edge != nullptr)
		node.push_back(edge);
	return nodeNum_++;
}

bool Graph::addEdge(const int &src, const int &dest, const char &ch)
{
	Edge* p = node[src];
	Edge* newEdge = new Edge(ch);
	if (newEdge == nullptr) return false;
	while (p->next_ != nullptr)
	{
		if (dest == p->dest_ && p->type_ == TT::CHAR && p->ch_ == ch)
			return false;
		p = p->next_;
	}
	newEdge->dest_ = dest;
	edgeNum_++;
	return true;
}


bool Graph::addEdge(const int &src, const int &dest, const int &num)
{
	Edge* p = node[src];
	Edge* newEdge = new Edge(num);
	if (newEdge == nullptr) return false;
	while (p->next_ != nullptr)
	{
		if (dest == p->dest_ && p->type_ == TT::NUM && p->num_ == num)
			return false;
		p = p->next_;
	}
	newEdge->dest_ = dest;
	edgeNum_++;
	return true;
}
bool Graph::addEdge(const int &src, const int &dest, const string &name)
{
	Edge* p = node[src];
	string* s = new string(name);
	if (s == nullptr) return false;
	Edge* newEdge = new Edge(s);
	if (newEdge == nullptr) return false;
	while (p->next_ != nullptr)
	{
		if (dest == p->dest_ && p->type_ == TT::NAME && *(p->s_) == name)
			return false;
		p = p->next_;
	}
	newEdge->dest_ = dest;
	edgeNum_++;
	return true;
}
void Graph::show()const
{
	Edge* p = nullptr;
	for (int i = 0; i < nodeNum_; i++)
	{
		p = node[i]->next_;
		out << "node [" << i << "]:";
		while (p != nullptr)
		{

			out << "   |-- " ;
			if (p->type_ == TT::CHAR)
				out << p->ch_;
			else if (p->type_ == TT::NAME)
				out << *p->s_;
			else if (p->type_ == TT::NUM)
				out << p->num_;
			else out << "Err";
			out << " --> " << p->dest_;
			p = p->next_;
		}
		out << std::endl;
	}
	return;
}
void Graph::showSet(const vector<set<int>>& s)const
{
	Edge* p = nullptr;
	for (int i = 0; i < nodeNum_; i++)
	{
		p = node[i]->next_;
		out << "node [" << i << "]: {";
		for (auto it = s[i].begin(); it != s[i].end(); it++)
			out << *it << " ";
		out << "}" << endl;;
		while (p != nullptr)
		{

			out << "   |-- ";
			if (p->type_ == TT::CHAR)
				out << p->ch_;
			else if (p->type_ == TT::NAME)
				out << *p->s_;
			else if (p->type_ == TT::NUM)
				out << p->num_;
			else out << "Err";
			out << " --> " ;
			out << "{ ";
			for (auto it = s[p->dest_].begin(); it != s[p->dest_].end(); it++)
				out << *it << " ";
			out << "}" << endl;;
			p = p->next_;
		}
		out << std::endl;
	}
	return;
}

void Graph::getTset(set<int>& s)const
{
	Edge* p = nullptr;
	for (int i = 0; i < nodeNum_; i++)
	{
		p = node[i]->next_;
		if (p == nullptr)
			s.insert(i);
	}

}


void Graph::getEset(int index, const int sIndex, vector<set<int>> &s) const
{
	s[sIndex].insert({ index });
	Edge* p = node[index]->next_;
	if (!p) return;

	queue<int> q;
	q.push(index);
	while (!q.empty())
	{
		index = q.front();
		q.pop();
		p = node[index]->next_;
		while (p != nullptr)
		{
			if (p->type_ == TT::NUM && p->num_ == -1)
			{
				if (s[sIndex].find(p->dest_) == s[sIndex].end())
				{
					q.push(p->dest_);
					s[sIndex].insert(p->dest_);
				}
				p = p->next_;
			}
			else
				p = p->next_;
		}
	}
	return;
}

void Graph::getNEset(Graph* dfa, const int& index, vector<set<int>> &s) const
{
	Edge* p = nullptr;
	vector<Edge> tokenList;
	vector<set<int>> newStates;
	for (auto it = s[index].begin(); it != s[index].end(); it++)
	{
		p = node[*it]->next_;
		while (p != nullptr)
		{
			if (p->type_ == TT::NUM && p->num_ == -1)
				p = p->next_;
			else
			{
				int i = 0;
				for (i; i < tokenList.size(); i++)
				{
					if (p->type_ == tokenList[i].type_)
					{

						if (p->type_ == TT::NAME && *p->s_ == *tokenList[i].s_ ||
							p->type_ == TT::CHAR && p->ch_ == tokenList[i].ch_ ||
							p->type_ == TT::NUM && p->num_ == tokenList[i].num_)
						{
							newStates[tokenList[i].dest_].insert(p->dest_);
							getEset(p->dest_, tokenList[i].dest_, newStates);
						}

					}
				}
				if (i >= tokenList.size())
				{
					newStates.push_back(set<int>({ p->dest_ }));
					getEset(p->dest_, newStates.size()-1, newStates);
					Edge edge;
					if (p->type_ == TT::NAME)
						edge = Edge(new string(*p->s_));
					else if (p->type_ == TT::CHAR)
						edge = Edge(p->ch_);
					else if (p->type_ == TT::NUM)
						edge = Edge(p->num_);
					edge.dest_ = newStates.size() - 1;
					tokenList.push_back(edge);
				}
				p = p->next_;
			}
		}
	}
	for (int i = 0; i < newStates.size(); i++)
	{
		int j = 0;
		for (j; j < s.size(); j++)
		{
			if (newStates[i] == s[j])
			{
				for (int k = 0; k < tokenList.size(); k++)
				{
					if (tokenList[k].dest_ == i)
					{
						switch (tokenList[k].type_)
						{
						case TT::NAME:
							dfa->addEdge(index, j, *tokenList[k].s_);
							break;
						case TT::CHAR:
							dfa->addEdge(index, j, tokenList[k].ch_);
							break;
						case TT::NUM:
							dfa->addEdge(index, j, tokenList[k].num_);
							break;
						default:
							break;
						}

						break;
					}

				}
				break;
			}
		}
		if (j >= s.size())
		{
			for (int k = 0; k < tokenList.size(); k++)
			{
				if (tokenList[k].dest_ == i)
				{
					s.push_back(newStates[i]);
					switch (tokenList[k].type_)
					{
					case TT::NAME:
						dfa->addEdge(index, s.size()-1, *tokenList[k].s_);
						break;
					case TT::CHAR:
						dfa->addEdge(index, s.size() - 1, tokenList[k].ch_);
						break;
					case TT::NUM:
						dfa->addEdge(index, s.size() - 1, tokenList[k].num_);
						break;
					default:
						break;
					}
					dfa->addNode();
				}

			}
		}
	}
}

void Graph::modEdge(Graph* ldfa, const vector<set<int>>& s, const int *to) const
{
	Edge* p = nullptr;
	for (int i = 0; i < ldfa->getNodeSize(); i++)
	{
		for (auto it = s[i].begin(); it != s[i].end(); it++)
		{
			p = node[*it]->next_;
			while (p != nullptr)
			{
				if (p->type_ == TT::NAME)
					ldfa->addEdge(i, to[p->dest_], *p->s_);
				else if (p->type_ == TT::NUM)
					ldfa->addEdge(i, to[p->dest_], p->num_);
				else if (p->type_ == TT::CHAR)
					ldfa->addEdge(i, to[p->dest_], p->ch_);
			}
		}
	}
}

long long Graph::cal(int node_, const vector<Edge>* tL, const set<int>* s2) const
{
	long long result = 0;
	Edge* p = nullptr;
	for (int i = 0; i < tL->size(); i++)
	{
		p = node[node_]->next_;
		while (p != nullptr)
		{
			if (p->type_ == (*tL)[i].type_)
			{
				if (p->type_ == TT::CHAR && p->ch_ == (*tL)[i].ch_)
					break;
				else if (p->type_ == TT::NUM && p->num_ == (*tL)[i].num_)
					break; 
				else if (p->type_ == TT::NAME && p->s_ == (*tL)[i].s_)
					break;
			}
			p = p->next_;
		}
		if (p != nullptr)
			if (s2->find(p->dest_) != s2->end())
				result += 2;
			else
				result += 1;
		result *= 3;
	}
	return result;
}