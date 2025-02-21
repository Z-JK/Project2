#pragma once
#ifndef _REXP_H_
#define _REXP_H_

#include "GLOBALS.H"
#include "GRAPH.H"
#include "SCANNER.H"
#include "PARSE.H"
using std::string;

struct Rexp {
	string name_;
	int code_;
	int len_;

	string rexp_;
	vector<Edge> tokenList;
	Graph* nfa_;
	Graph* dfa_;
	vector<set<int>> statesSet;
	vector<set<int>> ldfaStatesSet;
	set<int> s2;
	Graph* ldfa_;
	Rexp() :name_(""), code_(0), len_(0), rexp_(""), tokenList(0), nfa_(nullptr), dfa_(nullptr), statesSet(vector<set<int>> (0)), ldfa_(nullptr),
			s1(set<int>()), s2(set<int>()){ tokenList.push_back(Edge(-1)); };
};

extern std::vector<Rexp*> Rexps;
extern std::map<string, int> names;
Rexp* getRexp(void);
#endif // !_REXP_H_
