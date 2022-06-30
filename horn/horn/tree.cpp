#include<iostream>
#include<vector>
#include<sstream>
#include<unordered_map>
#include<stack>
#include"horn.h"
using namespace std;

extern vector<clause> HornClause[MAX_NUM];//存储horn子句(所有)
extern int ClauseNum;
extern pair<int, int> parents[MAX_NUM];//记录双亲结点
extern int BaseClauseNum;
stack<pair<vector<clause>, pair<int, int>>> sta;
stack<int> sta1;
unordered_map<int, int> mp;

void Simplify(int end)//去除无关步骤
{
	int p1 = parents[end].first, p2 = parents[end].second;
	sta.push({ HornClause[end],parents[end] });
	sta1.push(end);
	if (parents[p1].first != 0)Simplify(p1);
	if (parents[p2].first != 0)Simplify(p2);//递归
}
void SeeAns(string& ansstr)
{
	int k = BaseClauseNum;
	while (!sta1.empty())
	{
		mp[sta1.top()] = ++k;
		sta1.pop();
	}
	k = BaseClauseNum;
	while (!sta.empty())
	{
		stringstream ss1, ss2;
		display(++k, sta.top().first, ansstr);
		ansstr = ansstr.substr(0, ansstr.size() - 1);
		ansstr += ".........(";
		if (mp[sta.top().second.first])
			ss1 << mp[sta.top().second.first];//加上父母结点。
		else
			ss1 << sta.top().second.first;
		ansstr += ss1.str();
		ansstr += ")(";
		if (mp[sta.top().second.second])
			ss2 << mp[sta.top().second.second];
		else
			ss2 << sta.top().second.second;
		ansstr += ss2.str();
		ansstr += ")\n";
		sta.pop();
	}
}
