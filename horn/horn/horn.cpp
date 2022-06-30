#define WIDTH 320
#define HEIGHT 150
#include "horn.h"
#include<iostream>
#include<unordered_map>
#include<vector>
#include<QLineEdit>//提示消息
#include <QInputDialog>
#include <QDesktopWidget>
#include <QApplication>
#include<qdebug.h>
#include<sstream>
#include<QPainter>
#include<stack>
#include<queue>
#include<QPushButton>
using namespace std;

vector<string> lines;
unordered_map<string, int> Variable;//存储变量
unordered_map<string, int> Constant;//存储常量
vector<clause> HornClause[MAX_NUM];//存储horn子句(所有)
pair<clause, int> SingleClause[MAX_NUM];//存储horn子句(=1)
int ClauseNum = 1, SingleClauseNum = 1;//每种长度的子句的数量
int BaseClauseNum;
int VariableNum = 0;//变量数量
int fa[MAX_NUM];//并查集
pair<int, int> parents[MAX_NUM];//记录双亲结点
extern unordered_map<int, int> mp;
bool isregulation = false;

MyScrollArea::MyScrollArea(QWidget* parent)
	: QWidget(parent) {}

MyScrollArea::~MyScrollArea() {}

void MyScrollArea::paintEvent(QPaintEvent* event) {    //滑动窗口的绘图事件  
	QPainter painter(this);                            //将父类设置为MyScrollArea窗口

	painter.setPen(QPen(Qt::black, 2));

	queue<int> sss;
	int f, m;
	for (int i = 1; i <= ClauseNum; i++) {
		string k;
		display(i, HornClause[i], k);
		//cout << k;
		tie(f, m) = parents[i];
		//cout << f << "+++++++" << m << endl;
	}
	int x;
	sss.push(ClauseNum);
	vector<int> treelayer[200];
	int zeronumber = 1;
	int maxlayer;
	for (int i = 0; zeronumber != 0; i++) {
		queue<int> sss1;
		zeronumber = 0;
		while (!sss.empty()) {
			treelayer[i].push_back(sss.front());
			x = sss.front();
			if (x != 0)
				zeronumber++;
			tie(f, m) = parents[x];
			sss1.push(f);
			sss1.push(m);
			sss.pop();
		}
		sss = sss1;
		maxlayer = i;
	}
	int height = 0;
	painter.drawText(QRect(10, 15 + (height) * 100, 1000, 300), "the tree as follow:");
	height++;
	for (int i = maxlayer - 1; i >= 0; i--) {
		int weight = 0;
		int count = 0;
		for (int j = 0; j < treelayer[i].size(); j++) {
			string horn = "";
			if (mp[treelayer[i][j]])
				display(mp[treelayer[i][j]], HornClause[treelayer[i][j]], horn);
			else
				display(treelayer[i][j], HornClause[treelayer[i][j]], horn);
			int length = HornClause[treelayer[i][j]].size();
			if (treelayer[i][j] != 0) {
				tie(f, m) = parents[treelayer[i][j]];
				if (f != 0) {
					painter.drawLine(10 + (weight)*WIDTH + 50, (height)*HEIGHT - 50, 10 + (count++) * WIDTH + 40, (height - 1) * HEIGHT - 30);
				}
				if (m != 0) {
					painter.drawLine(10 + (weight)*WIDTH + 50, (height)*HEIGHT - 50, 10 + (count++) * WIDTH + 40, (height - 1) * HEIGHT - 30);
				}
				painter.drawRect(10 + (weight)*WIDTH, (height)*HEIGHT - 50, length * 85 + 55, 20);
				painter.drawText(QRect(10 + (weight++) * WIDTH, (height)*HEIGHT - 50, 1000, 300), horn.c_str());
			}
			//painter.drawLine(x + count_num * 100 + 45, y + 90, x + number_ * 100 + 45, y + height0);
			//cout << treelayer[i][j] << ' ';
		}
		height++;
		cout << endl;
	}


}

void horn2::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	painter.drawText(QRect(10, 15, 1000, 300), "Base Clause as follow:");
	int i;
	for (i = 1; i <= BaseClauseNum; i++)
	{
		string bs;
		display(i, HornClause[i], bs);
		painter.drawText(QRect(10, 15 + i * 30, 1000, 300), bs.c_str());
	}
	if (isregulation)
	{
		painter.drawText(QRect(10, 60 + BaseClauseNum * 30, 1000, 300), "Resolution AVAILABLE");
		painter.drawText(QRect(10, 100 + BaseClauseNum * 30, 1000, 300), "the result as follow:");
		for (i = 1; i < lines.size(); i++)
		{
			painter.drawText(QRect(10, 100 + BaseClauseNum * 30 + i * 30, 1000, 300), lines[i].c_str());
		}
	}
	else
		painter.drawText(QRect(10, 60 + BaseClauseNum * 30, 1000, 300), "Resolution INAVAILABLE!!!");
	painter.setPen(QPen(Qt::black, 2));

	queue<int> sss;
	int f, m;
	for (int i = 1; i <= ClauseNum; i++) {
		tie(f, m) = parents[i];
		//cout << f << "+++++++" << m << endl;
	}
	int x;
	sss.push(ClauseNum);
	vector<int> treelayer[200];
	int zeronumber = 1;
	int maxlayer;
	for (int i = 0; zeronumber != 0; i++) {
		queue<int> sss1;
		zeronumber = 0;
		while (!sss.empty()) {
			treelayer[i].push_back(sss.front());
			x = sss.front();
			if (x != 0)
				zeronumber++;
			tie(f, m) = parents[x];
			sss1.push(f);
			sss1.push(m);
			sss.pop();
		}
		sss = sss1;
		maxlayer = i;
	}
}


horn2::horn2(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	/*QLineEdit* aEdit1 = new QLineEdit;
	aEdit1->resize(200, 10);*/
	resize(600, 900);
	QDesktopWidget* deskdop = QApplication::desktop();
	move((deskdop->width() - this->width()) / 2, (deskdop->height() - this->height()) / 2);
	QPushButton* btn = new QPushButton;                 //提交按钮，按下之后生成演示界面
	btn->setText("tree");                            //设置按钮上面的文字 
	btn->setParent(this);                               //将窗口设置为按钮的父类
	btn->move(400, 50);                                //设置按钮的位置
	btn->resize(100, 40);
	connect(btn, &QPushButton::clicked, [=]() {   //当按下another按钮时，开始演示搜索树界面，其中如果新建的界面在槽函数结束之后会被系统收回，就改为将界面放入堆中
		new_win = new horn();
		new_win->show();
		});
	//aEdit1->setParent(this);
	bool isOK;
	setFixedSize(frameGeometry().width(), frameGeometry().height());
	QInputDialog* input_VC = new QInputDialog;//输入变量和常量
	QString text_VAC = input_VC->getMultiLineText(this, "input variables and constant",
		"input variables in the first line ,input constant in the second line:", "x %\nA B C %",
		&isOK);

	QInputDialog* input1 = new QInputDialog;//输入horn子句
	QString texthorn = input1->getMultiLineText(this, "input horn",
		"input string as follow:                ", "Kill(C,A)|Kill(B,A)|Kill(A,A)\nHate(x,A)|!Kill(x,A)\n!Hate(C,x)|!Hate(A,x)\nHate(A,A)\nHate(A,C)\nRicher(x,A)|Hate(B,x)\n!Richer(x,A)|!Hate(B,x)\nHate(A,x)|!Hate(B,x)\n!Hate(A,x)|Hate(B,x)\n!Hate(A,A)|!Hate(A,B)|!Hate(A,C)\n!Hate(B,A)|!Hate(B,B)|!Hate(B,C)\n!Hate(C,A)|!Hate(C,B)|!Hate(C,C)\n!Richer(x,A)|!Kill(x,A)\n%",
		&isOK);


	QInputDialog* input2 = new QInputDialog;//输入需要判断的子句
	QString textjudge = input2->getMultiLineText(this, "input judge",
		"input string as follow:                ", "Kill(A,A) %",
		&isOK);
	string s_horn = texthorn.toStdString();
	string s_judge = textjudge.toStdString();
	string s_vc = text_VAC.toStdString();

	//cout << s_horn << endl << s_judge << endl << s_vc << endl;
	string ans;


	stringstream iostr_horn(s_horn);
	stringstream iostr_judge(s_judge);
	stringstream iostr_vc(s_vc);

	//iostr >> s_judge;

	ReadinVariable(iostr_vc);//读入变量
	ReadinConstant(iostr_vc);//读入常量
	ReadinHorn(iostr_horn);//读入horn子句
	ReadinJudge(iostr_judge);//读入判读句
	ans = ClauseResolution();//子句归结
	//cout << ans;
	stringstream iostr_ans(ans);
	string perline;
	iostr_ans >> perline;
	for (int i = 0; perline != "%"; i++) {
		lines.push_back(perline);
		iostr_ans >> perline;
	}
}


horn::horn(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(1200, 800);
	QScrollArea* pScroll = new QScrollArea(this);
	this->setCentralWidget(pScroll);
	MyScrollArea* pTempWnd = new MyScrollArea(pScroll);
	pScroll->setWidget(pTempWnd);
	pTempWnd->setFixedSize(10000, 5000);
	QVBoxLayout* pLay = new QVBoxLayout(pTempWnd);
	pTempWnd->setLayout(pLay);
}


/*
测试数据如下：
x %
A B C %
Kill(C,A)|Kill(B,A)|Kill(A,A)
Hate(x,A)|!Kill(x,A)
!Hate(C,x)|!Hate(A,x)
Hate(A,A)
Hate(A,C)
Richer(x,A)|Hate(B,x)
!Richer(x,A)|!Hate(B,x)
Hate(A,x)|!Hate(B,x)
!Hate(A,x)|Hate(B,x)
!Hate(A,A)|!Hate(A,B)|!Hate(A,C)
!Hate(B,A)|!Hate(B,B)|!Hate(B,C)
!Hate(C,A)|!Hate(C,B)|!Hate(C,C)
!Richer(x,A)|!Kill(x,A)
%
Kill(A,A)
*/


void ReadinVariable(stringstream& ss)//读入变量
{
	string s;
	//cout << "请输入所有变量名(变量名间以空格隔开，以空格加%结束)" << endl;//输入提示
	//cout << "本题可以直接复制下面的示例：" << endl << "x %" << endl;
	while (ss >> s && s != "%")
	{
		Variable[s] = ++VariableNum;;//用于判断是否为变量
	}
	//cout << "***";
}
void ReadinConstant(stringstream& ss)//读入常量
{
	string s;
	//cout << "请输入所有常量名(常量名间以空格隔开，以空格加%结束)" << endl;//输入提示
	//cout << "本题可以直接复制下面的示例：" << endl << "A B C %" << endl;
	while (ss >> s && s != "%")
	{
		Constant[s] = 1;//用于判断是否为常量
		//cout << s << endl;
		//cout << "+++" << endl;
	}
}
void split(string s, clause& c)
{
	int len = s.length(), i = 0;
	if (s[0] == '!')c.flag = -1, i++;
	else c.flag = 1;
	for (; i < len && s[i] != '('; i++)
	{
		c.action += s[i];
	}
	while (i < len - 1)
	{
		string rat = "";
		for (i = i + 1; i < len - 1 && s[i] != ','; i++)
		{
			rat += s[i];
		}
		c.ration.push_back(rat);
	}
}
void ReadinHorn(stringstream& ss)//读入horn子句
{
	string s, s_part;
	int flag = 0;
	/*cout << "请输入所有horn子句(每行一句，以换行加%结束)" << endl;
	cout << "本题可以直接复制下面的示例：" << endl;
	cout << "Kill(C,A)|Kill(B,A)|Kill(A,A)" << endl;
	cout << "Hate(x,A)|!Kill(x,A)" << endl;
	cout << "!Hate(C,x)|!Hate(A,x)" << endl;
	cout << "Hate(A,A)" << endl;
	cout << "Hate(A,C)" << endl;
	cout << "Richer(x,A)|Hate(B,x)" << endl;
	cout << "!Richer(x,A)|!Hate(B,x)" << endl;
	cout << "Hate(A,x)|!Hate(B,x)" << endl;
	cout << "!Hate(A,x)|Hate(B,x)" << endl;
	cout << "!Hate(A,A)|!Hate(A,B)|!Hate(A,C)" << endl;
	cout << "!Hate(B,A)|!Hate(B,B)|!Hate(B,C)" << endl;
	cout << "!Hate(C,A)|!Hate(C,B)|!Hate(C,C)" << endl;
	cout << "!Richer(x,A)|!Kill(x,A)" << endl;
	cout << "%" << endl;*/
	while (ss >> s && s != "%")
	{
		flag = 0;
		s_part = "";
		for (char& c : s)
		{
			if (c == '|')
			{
				flag = 1;
				clause pair_part;
				split(s_part, pair_part);
				HornClause[ClauseNum].push_back(pair_part);
				s_part = "";
			}
			else
				s_part += c;
		}
		clause pair_part;
		split(s_part, pair_part);
		if (!flag)
		{
			SingleClause[SingleClauseNum++] = { pair_part,ClauseNum };
		}
		HornClause[ClauseNum++].push_back(pair_part);
	}
}
void ReadinJudge(stringstream& ss)//读入判读句
{
	string s, s_part = "";
	/*cout << "请输入需要判断的句子" << endl;
	cout << "本题可以直接复制下面的示例：" << endl << "Kill(A,A)" << endl;*/
	ss >> s;
	for (char& c : s)
	{
		if (c == '|')
		{
			if (s_part[0] == '!')s_part = s_part.substr(1);
			else s_part = "!" + s_part;
			clause pair_part;
			split(s_part, pair_part);
			SingleClause[SingleClauseNum++] = { pair_part,ClauseNum };
			HornClause[ClauseNum++] = { pair_part };
			s_part = "";
		}
		else
			s_part += c;
	}
	if (s_part[0] == '!')s_part = s_part.substr(1);
	else s_part = "!" + s_part;
	clause pair_part;
	split(s_part, pair_part);
	SingleClause[SingleClauseNum++] = { pair_part,ClauseNum };
	HornClause[ClauseNum++] = { pair_part };
	BaseClauseNum = ClauseNum - 1;
}
int find(int x)//并查集的find函数
{
	if (fa[x] != x)fa[x] = find(fa[x]);
	return fa[x];
}
void display(int id, vector<clause>& newclause, string& ss)//输出horn子句
{
	string s;
	int len = newclause.size();
	for (int j = 0; j < len; j++)
	{
		clause c = newclause[j];
		if (j)s += '|';
		if (c.flag == -1)s += '!';
		s += c.action + '(';
		int sz = c.ration.size();
		for (int i = 0; i < sz; i++)
		{
			if (i)s += ',';
			s += c.ration[i];
		}
		s += ')';
	}
	//cout << "(" << id << ")" << s << endl;
	ss += "(";
	ss += to_string(id);
	ss += ")";
	ss += s;
	ss += '\n';
}

bool Repeat(vector<clause>& newclause)//判断推导出的子句是否已经在子句中
{
	int sz = 0;
	for (int i = 1; i < ClauseNum; i++)
	{
		if (newclause.size() == HornClause[i].size())
		{
			sz = newclause.size();
			for (int j = 0; j < sz; j++)
			{
				clause c1 = newclause[j];
				clause c2 = HornClause[i][j];
				int f = 1, len = c1.ration.size();
				if (c1.flag != c2.flag || c1.action != c2.action)
					continue;
				for (int k = 0; k < len; k++)
				{
					if (c1.ration[k] != c2.ration[k])
					{
						f = 0;
						break;
					}
				}
				if (f == 1)return true;
			}
		}
	}
	return false;
}
string ClauseResolution()//子句归结
{
	int id = 0, answer_flag = 0;
	//trans_num = 0;
	string clausestr = "";
	string ansstr = "";
	string cc = "";
	for (int i = 1; i < ClauseNum; i++)
	{
		display(++id, HornClause[i], cc);
	}
	//cout << "进行归结" << endl;
	for (int sc = 1; sc < SingleClauseNum; sc++)
	{
		answer_flag = 0;
		clause c1 = SingleClause[sc].first;
		for (int i = 1; i < ClauseNum; i++)
		{
			int match_flag = 0, num = 0;
			vector<string> v2c(2 * VariableNum + 1, "");
			for (auto& c2 : HornClause[i])
			{
				num++;
				if (c1.action == c2.action && c1.ration.size() == c2.ration.size() && c1.flag + c2.flag == 0)
				{
					for (int x = 1; x <= 2 * VariableNum; x++)
						fa[x] = x, v2c[x] = "";
					match_flag = 1;
					int sz = c1.ration.size();
					for (int j = 0; j < sz; j++)
					{
						int v1 = Variable[c1.ration[j]];
						int v2 = Variable[c2.ration[j]];
						if (v1 == 0 && v2 == 0)//对应都是常量
						{
							if (c1.ration[j] != c2.ration[j])
								match_flag = 0;
						}
						else if (v1 == 1 && v2 == 0)//对应分别是变量和常量
						{
							int fa1 = find(Variable[c1.ration[j]]);
							if (v2c[fa1] != "" && v2c[fa1] != c2.ration[j])
								match_flag = 0;
							v2c[fa1] = c2.ration[j];
						}
						else if (v1 == 0 && v2 == 1)//对应分别是常量和变量
						{
							int fa2 = find(Variable[c2.ration[j]] + VariableNum);
							if (v2c[fa2] != "" && v2c[fa2] != c1.ration[j])
								match_flag = 0;
							v2c[fa2] = c1.ration[j];
						}
						else//对应都是变量，这种情况很难
						//例如 Action(x,x,y,z,A)和!Action(y,z,x,x,x)之间匹配
						{
							int fa1 = find(Variable[c1.ration[j]]);
							int fa2 = find(Variable[c2.ration[j]] + VariableNum);
							if (fa1 != fa2)
							{
								if (v2c[fa1] != "" && v2c[fa2] != "" && v2c[fa1] != v2c[fa2])
									match_flag = 0;
								else if (v2c[fa1] != "")
									v2c[fa2] = v2c[fa1];
								fa[fa1] = fa2;
							}
						}
						if (match_flag == 0)break;
					}
					if (match_flag)break;
				}
			}
			if (match_flag)
			{
				int _num = 0;
				vector<clause> newclause;//生成推导后的子句
				for (auto& c : HornClause[i])
				{
					clause newc = c;
					_num++;
					if (_num == num)continue;
					int len = newc.ration.size();
					for (int k = 0; k < len; k++)
					{
						int fak = find(Variable[newc.ration[k]] + VariableNum);
						if (v2c[fak] != "")newc.ration[k] = v2c[fak];
					}
					newclause.push_back(newc);
				}
				parents[ClauseNum] = { i,SingleClause[sc].second };
				if (newclause.size() == 0)
				{
					//cout << "(" << ++id << ")" << "NIL" << endl;
					clausestr += "(";
					clausestr += char(++id + '0');
					clausestr += ")NIL\n";
					clause c;
					c.action = "NIL";
					c.flag = 0;
					newclause.push_back(c);
					HornClause[ClauseNum] = newclause;
					answer_flag = 1;
					break;
				}
				if (Repeat(newclause))continue;
				if (newclause.size() == 1)
					SingleClause[SingleClauseNum++] = { newclause[0],ClauseNum };
				HornClause[ClauseNum++] = newclause;
				display(++id, newclause, clausestr);
			}
		}
		if (answer_flag == 1)
			break;
	}
	if (answer_flag)
	{
		//cout << "可以推断" << endl;
		isregulation = true;
		ansstr += "yes\n";
		Simplify(ClauseNum);
		SeeAns(ansstr);
		ansstr += "%";
		//cout << ansstr << endl;
	}
	else
	{
		//cout << "无法确定" << endl;
		isregulation = false;
		ansstr += "no\n";
		ansstr += "%";
	}
	return ansstr;
}
