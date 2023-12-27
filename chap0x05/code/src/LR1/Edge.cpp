#include "LR1/Edge.h"

Edge::Edge(const Edge &e)
{
	Start = e.Start;
	End = e.End;
	trans = e.trans;
}

const Node &Edge::get_Start() const
{
	return Start;
}

const Node &Edge::get_End() const
{
	return End;
}

const string &Edge::get_trans() const
{
	return trans;
}

Node &Edge::get_Start()
{
	return Start;
}

Node &Edge::get_End()
{
	return End;
}

string &Edge::get_trans()
{
	return trans;
}

void Edge::set_Start(const Node &s)
{
	Start = s;
}

void Edge::set_End(const Node &e)
{
	End = e;
}

void Edge::set_trans(const string &t)
{
	trans = t;
}