// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cstdlib"
#include "info.h"



int main() {
	{
		liuWeiMing::Graph<int, 9, 13> a;
		for (int i = 1; i <= 8; ++i) a.addNode(i);
		a.addEdge(1, 2, 1, 'A');
		a.addEdge(1, 3, 2, 'B');
		a.addEdge(1, 4, 3, 'C');
		a.addEdge(2, 7, 4, 'D');
		a.addEdge(2, 5, 3, 'E');
		a.addEdge(3, 7, 4, 'F');
		a.addEdge(3, 6, 4, 'G');
		a.addEdge(3, 4, 2, 'H');
		a.addEdge(4, 5, 4, 'I');
		a.addEdge(7, 8, 6, 'J');
		a.addEdge(5, 6, 1, 'K');
		a.addEdge(6, 8, 1, 'L');

		a.addEdge(2, 3, 0, '\0'); // 虚任务

		a.showMainlyPath();
	}
	system("pause");
    return 0;
}

