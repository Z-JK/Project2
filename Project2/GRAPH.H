#pragma once
#ifndef _GRAPH_H_
#define _GRAPH_H_


#include "GLOBALS.H"
using std::string;

struct Edge
{
	int dest_;
	TT type_;
	union
	{
		int num_;
		char ch_;
		string *s_;
	};
	Edge* next_;
	Edge() :dest_(-1), type_(TT::NUM), next_(nullptr) { num_ = -1; }
	Edge(const int& num) :dest_(-1), type_(TT::NUM), next_(nullptr) { num_ = num; }
	Edge(const char& ch) :dest_(-1), type_(TT::CHAR), next_(nullptr) { ch_ = ch; }
	Edge(string *const s) :dest_(-1), type_(TT::NAME), next_(nullptr) { s_ =  s; }
	~Edge() { if (s_ != nullptr) delete s_; };
};

class Graph
{
	int nodeNum_;
	int edgeNum_;
	std::vector<Edge*> node;
public:	
	Graph() : nodeNum_(0), edgeNum_(0), node(std::vector<Edge*>(0)) {/* s0 表示开始， s1 表示结束*/ };
	int getNodeSize()const;
	int addNode();
	bool addEdge(const int &src, const int &dest, const char &ch);
	bool addEdge(const int &src, const int &dest, const int &num);
	bool addEdge(const int &src, const int &dest, const string &name);
	void show()const;
	void showSet(const vector<set<int>>& s)const;
	void getTset(set<int> &s)const;
	void getEset(int index, const int sIndex, vector<set<int>>& s) const;
	void getNEset(Graph* dfa, const int &index, vector<set<int>> &s) const;
	void modEdge(Graph* ldfa, const vector<set<int>>& s, const int* to) const;
	long long cal(int node_, const vector<Edge> *tL, const set<int> *s2) const;

};

#endif // !_GRAPH_H_

