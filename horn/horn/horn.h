#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_horn.h"
#include <QScrollArea>
#include <QVBoxLayout>
using namespace std;

struct clause
{
	int flag;//记录子句是正还是非
	string action;//记录子句前置词
	vector<string> ration;//记录子句包含的常量和变量
	clause()
	{
		flag = 0;
		action = "";
		ration.clear();
	}
};
const int MAX_NUM = 1e5 + 10;//最多有horn子句
void ReadinVariable(stringstream& ss);//读入变量
void ReadinConstant(stringstream& ss);//读入常量
void ReadinHorn(stringstream& ss);//读入horn子句
void ReadinJudge(stringstream& ss);//读入判读句
string ClauseResolution();//子句归结
int find(int x);//并查集find函数
void display(int id, vector<clause>& newclause, string& s);//输出
bool Repeat(vector<clause>& newclause);//判断是否出现重复的判断

void Simplify(int end);//去除无关步骤
void SeeAns(string& ansstr);//输出最终结果


class horn : public QMainWindow
{
	Q_OBJECT

public:
	horn(QWidget* parent = Q_NULLPTR);

private:
	Ui::hornClass ui;
};

class horn2 : public QMainWindow
{
	Q_OBJECT

public:
	horn2(QWidget* parent = Q_NULLPTR);
	void paintEvent(QPaintEvent* event);
private:
	Ui::hornClass ui;
	horn* new_win;
};



class MyScrollArea : public QWidget                //新界面：滑动窗口界面，此界面嵌套在窗口2界面当中
{                                                  //其中包含了构造函数和其中的绘图事件函数
	Q_OBJECT

public:
	MyScrollArea(QWidget* parent = Q_NULLPTR);
	~MyScrollArea();
	void paintEvent(QPaintEvent* event);
private:
};
