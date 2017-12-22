#pragma once
//#ifndef info.h
//#define info.h
#include <algorithm>
#include <queue>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <set>
#include <iomanip>
using namespace std;
namespace liuWeiMing {
	template<typename T, const int MAX_NODE_SIZE = 10000, const int MAX_EDGE_SIZE = 50000> //默认最大100000个节点
	class Graph {
	private:
		struct Edge {
			int u, v, tonext;
			T w;
			char name;
		} *e;
		int num, *first, *tim;
		bool *exit, *in;
		T *dis;
		pair<T, bool> getMinDistance_SPFA(int from, int to);
	public:
		Graph() {
			e = new Edge[MAX_EDGE_SIZE];
			first = new int[MAX_NODE_SIZE];
			exit = new bool[MAX_NODE_SIZE];
			dis = new T[MAX_NODE_SIZE];
			tim = new int[MAX_NODE_SIZE];
			in = new bool[MAX_NODE_SIZE];
			//memset(exit, false, sizeof exit);
			//memset(first, -1, sizeof first); 
			//指针不能直接sizeof			
			memset(exit, false, sizeof(exit[0]) * MAX_NODE_SIZE); //不能 * sizeof(bool *) = 4，指针大小就是4
			memset(first, -1, (sizeof (first[0])) * MAX_NODE_SIZE);
			num = 0;
		}
		~Graph() {
			printf("本次使用完毕\n"); //内敛函数，写少一点，建议放在同一行
			delete[] e;
			delete[] first;
			delete[] exit;
			delete[] dis;
			delete[] tim;
			delete[] in;
		}
		bool addNode(int u);
		bool delNode(int u);
		bool addEdge(int u, int v, T w, char name = '\0');
		void showMinDistance(int from, int to);
		void showMainlyPath();
	};
	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	bool Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::addNode(int u) {
		if (exit[u]) {
			printf("节点%d插入失败，节点已存在\n", u);
			return false;
		} else {
			printf("节点%d插入成功\n", u);
			exit[u] = true;
			return true;
		}
		
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	bool Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::delNode(int u) {
		if (!exit[u]) {
			printf("删除节点失败，节点不存在\n");
			return false;
		} else {
			printf("删除节点成功\n");
			exit[u] = false;
			return true;
		}
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	bool Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::addEdge(int u, int v, T w, char name) {
		if (!exit[u] || !exit[v]) {
			printf("插入边失败，其中一个节点不存在!!!\n");
			return false;
		}
		e[num].u = u, e[num].v = v, e[num].w = w, e[num].name = name, e[num].tonext = first[u];
		first[u] = num++;
		return true;
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	pair<T, bool> Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::getMinDistance_SPFA(int from, int to) {
		/*
		 res information
		 0, true 代表存在负环
		 -1, true 代表起点被删除
		 -2, true 代表无法到达终点
		 ans, false 代表ok
		 */
		if (!exit[from]) return make_pair(-1, true);
		for (int i = 0; i < MAX_NODE_SIZE; ++i) {
			dis[i] = (T)0x3f3f3f3f;
			tim[i] = 0;
			in[i] = false;
		}
		queue<int> que;
		while (!que.empty()) que.pop();
		que.push(from), in[from] = true, dis[from] = 0, tim[from]++;
		bool can = false;
		while (!que.empty()) {
			int u = que.front();
			if (tim[u] > MAX_NODE_SIZE) return make_pair(0, true);
			que.pop();
			if (u == to) can = true;
			for (int i = first[u]; ~i; i = e[i].tonext) {
				//cout << u << " " << e[i].v << endl;
				if (!exit[e[i].v]) continue;
				if (dis[e[i].v] > dis[e[i].u] + e[i].w) {
					dis[e[i].v] = dis[e[i].u] + e[i].w;
					if (!in[e[i].v]) {
						que.push(e[i].v);
						in[e[i].v] = true;
						tim[e[i].v]++;
					}
				}
			}
			in[u] = false;
		}
		if (!can) return make_pair(-2, true);
		return make_pair(dis[to], false);
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	void Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::showMinDistance(int from, int to) {
		pair<T, bool> res = getMinDistance_SPFA(from, to);
		if (res.second == false) {
			cout << from << "去" << to << "的最短距离是：" << res.first << endl;
		} else {
			if (res.first == 0) {
				cout << "路径存在负环" << endl;
			} else if (res.first == -1) {
				cout << "不存在起点" << endl;
			} else {
				cout << "无法到达终点" << endl;
			}
		}
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	void Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::showMainlyPath() { //求图的关键路径
		//ES，对于顶点来说，最早开始时间，拓扑一下取max
		//LF, 对于顶点来说，最晚结束时间，用stack保存拓扑序列，反向拓扑
		//
		T *ES = new T[MAX_NODE_SIZE];
		int *in = new int[MAX_NODE_SIZE];
		memset(in, false, sizeof(in[0]) * MAX_NODE_SIZE);
		for (int i = 0; i < MAX_NODE_SIZE; ++i) {
			for (int j = first[i]; ~j; j = e[j].tonext) {
				if (!exit[e[j].v]) continue;
				in[e[j].v]++;
			}
		}
		queue<int> que;
		while (!que.empty()) que.pop();
		for (int i = 0; i < MAX_NODE_SIZE; ++i) {
			if (!exit[i]) continue;
			if (in[i] == 0) {
				que.push(i);
			}
			ES[i] = (T)0;
		}
		stack<int> st;
		while (!st.empty()) st.pop();
		while (!que.empty()) { // 拓扑求ES
			int cur = que.front();
			que.pop();
			st.push(cur);
			for (int i = first[cur]; ~i; i = e[i].tonext) {
				if (!exit[e[i].v]) continue;
				in[e[i].v]--;
				ES[e[i].v] = max(ES[e[i].v], ES[cur] + e[i].w);
				if (in[e[i].v] == 0) que.push(e[i].v);
			}
		}
		T *ES_EDGE = new T[MAX_EDGE_SIZE];
		for (int i = 0; i < MAX_NODE_SIZE; ++i) {
			if (!exit[i]) continue;
			for (int j = first[i]; ~j; j = e[j].tonext) {
				ES_EDGE[j] = ES[i];
			}
		}
		// 求LF
		T *LF = new T[MAX_NODE_SIZE];
		for (int i = 0; i < MAX_NODE_SIZE; ++i) LF[i] = (T)0x3f3f3f3f;
		LF[st.top()] = ES[st.top()];
		while (!st.empty()) {
			int cur = st.top();
			st.pop();
			for (int i = first[cur]; ~i; i = e[i].tonext) {
				LF[cur] = min(LF[cur], LF[e[i].v] - e[i].w);
			}
		}
		T *LF_EDGE = new T[MAX_EDGE_SIZE];
		T *EF_EDGE = new T[MAX_EDGE_SIZE];
		T *LS_EDGE = new T[MAX_EDGE_SIZE];
		set<pair<char, int>> ss;
		for (int i = 0; i < MAX_NODE_SIZE; ++i) {
			if (!exit[i]) continue;
			for (int j = first[i]; ~j; j = e[j].tonext) {
				if (!exit[e[j].v]) continue;
				if (e[j].name != '\0') ss.insert(make_pair(e[j].name, j));
				LF_EDGE[j] = LF[e[j].v];
				EF_EDGE[j] = ES[i] + e[j].w;
				LS_EDGE[j] = LF[e[j].v] - e[j].w;
			}
		}
		int wide = 4;
		cout << "活动：";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(4) << it->first;
		}
		cout << endl;

		cout << "ES：";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << ES_EDGE[it->second];
		}
		cout << endl;

		cout << "EF：";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << EF_EDGE[it->second];
		}
		cout << endl;
		
		cout << "LS：";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << LS_EDGE[it->second];
		}
		cout << endl;

		cout << "LF：";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << LF_EDGE[it->second];
		}
		cout << endl;


		cout << "SL：";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << LS_EDGE[it->second] - ES_EDGE[it->second];
		}
		cout << endl;
		

		cout << "关键路径是：";

		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			if ((int)(LS_EDGE[it->second] - ES_EDGE[it->second]) == 0) {
				cout << e[it->second].name << "-->";
			}
		}
		cout << endl;
		delete[] ES;
		delete[] LF;
		delete[] ES_EDGE;
		delete[] LF_EDGE;
		delete[] LS_EDGE;
		delete[] EF_EDGE;
		delete[] in;
	}
}

//#endif // !info.h
