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
	template<typename T, const int MAX_NODE_SIZE = 10000, const int MAX_EDGE_SIZE = 50000> //Ĭ�����100000���ڵ�
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
			//ָ�벻��ֱ��sizeof			
			memset(exit, false, sizeof(exit[0]) * MAX_NODE_SIZE); //���� * sizeof(bool *) = 4��ָ���С����4
			memset(first, -1, (sizeof (first[0])) * MAX_NODE_SIZE);
			num = 0;
		}
		~Graph() {
			printf("����ʹ�����\n"); //����������д��һ�㣬�������ͬһ��
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
			printf("�ڵ�%d����ʧ�ܣ��ڵ��Ѵ���\n", u);
			return false;
		} else {
			printf("�ڵ�%d����ɹ�\n", u);
			exit[u] = true;
			return true;
		}
		
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	bool Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::delNode(int u) {
		if (!exit[u]) {
			printf("ɾ���ڵ�ʧ�ܣ��ڵ㲻����\n");
			return false;
		} else {
			printf("ɾ���ڵ�ɹ�\n");
			exit[u] = false;
			return true;
		}
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	bool Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::addEdge(int u, int v, T w, char name) {
		if (!exit[u] || !exit[v]) {
			printf("�����ʧ�ܣ�����һ���ڵ㲻����!!!\n");
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
		 0, true ������ڸ���
		 -1, true ������㱻ɾ��
		 -2, true �����޷������յ�
		 ans, false ����ok
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
			cout << from << "ȥ" << to << "����̾����ǣ�" << res.first << endl;
		} else {
			if (res.first == 0) {
				cout << "·�����ڸ���" << endl;
			} else if (res.first == -1) {
				cout << "���������" << endl;
			} else {
				cout << "�޷������յ�" << endl;
			}
		}
	}

	template<typename T, int MAX_NODE_SIZE, int MAX_EDGE_SIZE>
	void Graph<T, MAX_NODE_SIZE, MAX_EDGE_SIZE>::showMainlyPath() { //��ͼ�Ĺؼ�·��
		//ES�����ڶ�����˵�����翪ʼʱ�䣬����һ��ȡmax
		//LF, ���ڶ�����˵���������ʱ�䣬��stack�����������У���������
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
		while (!que.empty()) { // ������ES
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
		// ��LF
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
		cout << "���";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(4) << it->first;
		}
		cout << endl;

		cout << "ES��";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << ES_EDGE[it->second];
		}
		cout << endl;

		cout << "EF��";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << EF_EDGE[it->second];
		}
		cout << endl;
		
		cout << "LS��";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << LS_EDGE[it->second];
		}
		cout << endl;

		cout << "LF��";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << LF_EDGE[it->second];
		}
		cout << endl;


		cout << "SL��";
		for (set<pair<char, int>>::iterator it = ss.begin(); it != ss.end(); ++it) {
			cout << setw(wide) << LS_EDGE[it->second] - ES_EDGE[it->second];
		}
		cout << endl;
		

		cout << "�ؼ�·���ǣ�";

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
