#include<iostream>
#include<stdio.h>
#include<cmath> 
using namespace std;

int open_cnt = 0;
int open_node_cnt;//open表节点个数 
int close_cnt = 0;
int noresoult = 0;
struct Node {
	int a[3][3];
	int x, y;
	int f, g, h;
	int flag; //上一次移动方向 
	Node *father;
}start, End;
struct Open_Close {
	int f;
	Node *np;
}open[10000], close[10000];
bool isable() {//判断是否有解，逆序数之和奇偶性相同，有解
	int s[9], e[9];
	int tf = 0, ef = 0;
	int k = 0;
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			s[k] = start.a[i][j];
		    e[k] = End.a[i][j];
			k++;
		}
	}
	for (int i = 0; i<9; i++) {
		for (int j = 0; j<i; j++) {
			if (s[i]>s[j] && s[j] != 0) tf += 1;
			if (e[i]>e[j] && e[j] != 0) ef += 1;
		}
	}
	if ((tf % 2 == 1 && ef % 2 == 1) || (tf % 2 == 0 && ef % 2 == 0)) return true;
	else return false;
}
int a_start_h(Node *node) {  //求 h（） 
	int old_x, old_y, End_x, End_y;
	int h = 0;
	for (int k = 1; k<9; k++) {
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				if (node->a[i][j] == k) {  //相应开始点的下表
					old_x = i;
					old_y = j;
				}
				if (End.a[i][j] == k) {   //相应目标的结点下标
					End_x = i;
					End_y = j;
				}
			}
		}
		h += abs(old_x - End_x) + abs(old_y - End_y); //计算h
	}
	return h;
}
void input() {               //输入 
	cout << "=====输入起始图====="<<endl;
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			cin >> start.a[i][j];
			if (start.a[i][j] == 0) {
				start.x = i;
				start.y = j;
			}
		}
	}
	cout << endl;
	cout << "=====输入目标图====="<<endl;
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			cin >> End.a[i][j];
			if (End.a[i][j] == 0) {
				End.x = i;
				End.y = j;
			}
		}
	}
	cout << endl;
	start.g = 0;
	start.h = a_start_h(&start);
	start.f = start.g + start.h;
}
int show(Node *node) {               //显示 
	Node *p = node;
	if (p == &start) return 1;
	else show(p->father);
	cout << "==============\n";
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			cout << p->a[i][j] << " ";
		}
		printf("\n");
	}
	cout << "==============\n\n";
}


bool isend(Node *node) {         //判断是否为目标节点 
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			if (node->a[i][j] != End.a[i][j])
				return false;
		}
	}
	return true;
}


void sort(Open_Close *open) {      //open表排序 
	int min = 99999, min_flag = 0;
	Open_Close temp;
	for (int i = 0; i <= open_cnt; i++) {      
		if (min>open[i].f&&open[i].f>0) {
			min = open[i].f;
			min_flag = i;
		}
	}
	temp = open[min_flag];            
	open[min_flag] = open[0];
	open[0] = temp;
}



void move(int flag, Node *node) {   //向四个方向扩展 
	int temp;
	if (flag == 1 && node->x>0) {     //turn left
		Node *n = new Node();
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x - 1][node->y];
		n->a[node->x - 1][node->y] = 0;
		n->x = node->x - 1;
		n->y = node->y;
		n->flag = 3;
		n->father = node;
		n->g = node->g + 1;             //  求 g（） 
		n->h = a_start_h(n);
		n->f = n->g + n->h;  //  求 f（） 
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //添加到open表
		open[open_cnt].f = n->f;  
	}

	else if (flag == 2 && node->y<2) {     //go up
		Node *n = new Node();
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x][node->y + 1];
		n->a[node->x][node->y + 1] = 0;
		n->x = node->x;
		n->y = node->y + 1;
		n->flag = 4;
		n->father = node;
		n->g = node->g + 1;             //  求 g（） 
		n->h = a_start_h(n);
		n->f = n->g + n->h;            //  求 f（）
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //添加到open表
		open[open_cnt].f = n->f;   
	}
	else if (flag == 3 && node->x<2) {    //turn right
		Node *n = new Node();
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x + 1][node->y];
		n->a[node->x + 1][node->y] = 0;
		n->x = node->x + 1;
		n->y = node->y;
		n->flag = 1;
		n->father = node;
		n->g = node->g + 1;             //  求 g（） 
		n->h = a_start_h(n);
		n->f = n->g + n->h;//  求 f（）
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //添加到open表
		open[open_cnt].f = n->f;   
	}
	else if (flag == 4 && node->y>0) {    //go down
		Node *n = new Node();
		for (int i = 0; i<3; i++) {
			for (int j = 0; j<3; j++) {
				n->a[i][j] = node->a[i][j];
			}
		}
		n->a[node->x][node->y] = node->a[node->x][node->y - 1];
		n->a[node->x][node->y - 1] = 0;
		n->x = node->x;
		n->y = node->y - 1;
		n->flag = 2;
		n->father = node;
		n->g = node->g + 1;             //  求 g（） 
		n->h = a_start_h(n);
		n->f = n->g + n->h;//  求 f（） 
		open_cnt++;
		open_node_cnt++;
		open[open_cnt].np = n;        //添加到open表
		open[open_cnt].f = n->f;  
	}
}
void expand(Node *node) {    //节点扩展    
	for (int i = 1; i<5; i++) {
		if (i != node->flag) move(i, node);
	}
}


int main() {
	input();
	open[0].np = &start;//start放入open表			
	open_node_cnt = 1;
	if (isable()) {
		while (true) {//open表不为空 
			if (isend(open[0].np)) {
				cout << "\n路径：\n";
				show(open[0].np);
				cout << open[0].np->g << endl;
				break;
			}
			expand(open[0].np);//扩展最优节点的子节点 
			open[0].np = NULL;
			open[0].f = -1;
			open_node_cnt--;
			sort(open);   //open表排序
		}
	}
	else cout << "无解";
	system("pause");
	return(0);
}
