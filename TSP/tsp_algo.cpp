#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

#define LIMIT INT_MAX
using namespace std;

// структура ветви
struct node
{
	// стоимость пройденного пути
	int cost;
	// последняя пройденная вершина
	int current_vertice;
	// матрица смежности с учетом пройденного пути
	vector<vector<int>> matrix;
	// вектор всех взятых ребер (ребро - пара вершиин; первая - начало, вторая - конец)
	vector<pair<int, int>> edges;
	// множество всех пройденных вершин
	set<int> visited_vertices;
};

// инициализация графа (матрицы) случайными значениями
void init_random_DG(vector<vector<int>>& graph, int n)
{
	for (int i = 0; i < n; i++)
	{
		graph.push_back({});
		for (int j = 0; j < n; j++)
		{
			graph[i].push_back(LIMIT);
			if (i != j)
			{
				graph[i][j] = rand() % 1000;
			}
		}
	}
}

// вывод графа (матрицы) в консоль
void print_graph(vector<vector<int>>& graph)
{
	for (int i = 0; i < size(graph); i++)
	{
		for (int j = 0; j < size(graph); j++)
		{
			if (graph[i][j] == LIMIT)
			{
				cout << "inf" << ' ';
			}
			else
			{
				cout << graph[i][j] << ' ';
			}
		}
		cout << endl;
	}
}

// вспомогательная функция для стандартной sort для сортировки ветвей по стоимости
bool comparing_nodes(const node& a, const node& b)
{
	if (a.cost < b.cost)
	{
		return 1;
	}
	return 0;
}

// основной алгоритм
node branch_and_bound(const vector<vector<int>>& graph)
{
	// множество всех вершин графа
	set<int> all_vertices;
	for (int i = 0; i < size(graph); i++)
	{
		all_vertices.insert(i);
	}

	// вектор ветвей
	vector<node> nodes;
	nodes.push_back({ 0, 0, graph, {}, { 0 } });

	// основной цикл
	while (1)
	{
		// проверка на наличие незакрытых ветвей (если их нет, значит цикл не существует)
		if (!size(nodes))
		{
			return {};
		}

		// сортировка ветвей по их стоимостям (для дальнейшего использования минимальой)
		sort(nodes.begin(), nodes.end(), comparing_nodes);

		// инициализируем множество, являющееся разностью множества всех вершин и множества всех посещенных вершин (минимальной ветви)
		set<int> diff;
		set_difference(all_vertices.begin(), all_vertices.end(), nodes[0].visited_vertices.begin(), nodes[0].visited_vertices.end(), inserter(diff, diff.begin()));

		// множество всех вершин и множество всех посещенных вершин совпадают
		if (size(diff) == 0)
		{
			// все вершины обошли, осталось добавить конечное ребро, соединяющее последнюю посещенную вершину с самой первой
			if (size(nodes[0].edges) + 1 == size(graph) and (nodes[0].matrix[nodes[0].current_vertice][0] != LIMIT))
			{
				nodes[0].edges.push_back(make_pair(nodes[0].current_vertice, 0));
				nodes[0].cost += nodes[0].matrix[nodes[0].current_vertice][0];
				// мы добавляем эту ветвь в вектор ветвей, чтобы на следующих итерациях проверить - является ли полученанная ветвь минимальным циклом
				nodes.push_back(nodes[0]);
			}

			// минимальной ветвью оказалась та, которая содержит в себе весь цикл, следовательно это искомый цикл
			else if (size(nodes[0].edges) == size(graph))
			{
				return nodes[0];
			}
		}

		// создаем новые ветви добавляя в минимальную непосещенные вершины
		for (auto& j : diff)
		{
			// проверка на достижимость (конечный вес) непосещенной вершины 
			if (nodes[0].matrix[nodes[0].current_vertice][j] != LIMIT)
			{
				node tmp;
				tmp = nodes[0];
				tmp.cost += nodes[0].matrix[nodes[0].current_vertice][j];
				tmp.current_vertice = j;
				tmp.edges.push_back(make_pair(nodes[0].current_vertice, j));
				for (int i = 0; i < size(graph); i++)
				{
					tmp.matrix[i][j] = LIMIT;
					tmp.matrix[nodes[0].current_vertice][i] = LIMIT;
				}
				tmp.matrix[j][nodes[0].current_vertice] = LIMIT;
				tmp.visited_vertices.insert(j);

				nodes.push_back(tmp);
			}
		}
		// удаляем старую ветвь, которую рассмотрели
		nodes.erase(nodes.begin());
	}
}

int main()
{
	int answer;
	node cycle;
	vector<vector<int>> graph;
	graph = {};
	bool graph_is_chosen;
	do
	{
		cout << "Which graph will be used?\n1 - test 1\n2 - test 2\n3 - test 3\n4 - test 4\n5 - random graph" << endl;
		cin >> answer;
		cout << endl;
		graph_is_chosen = true;
		switch (answer)
		{
		case 1:
			// минимальный вес 42
			graph = { {LIMIT, 1, 2, 3, 4}, {14, LIMIT, 15, 16, 5}, {13, 20, LIMIT, 17, 6}, {12, 19, 18, LIMIT, 7}, {11, 10, 9, 8, LIMIT} };
			break;

		case 2:
			// минимальный вес 28
			graph = { {LIMIT, 20, 30, 10, 11}, {15, LIMIT, 16, 4, 2}, {3, 5, LIMIT, 2, 4}, {19, 6, 18, LIMIT, 3}, {16, 4, 7, 16, LIMIT} };
			break;

		case 3:
			// цикл не существует
			graph = { {LIMIT, LIMIT, LIMIT, LIMIT, 1}, {LIMIT, LIMIT, 1, LIMIT, LIMIT}, {LIMIT, LIMIT, LIMIT, 1, LIMIT}, {1, LIMIT, LIMIT, LIMIT, LIMIT}, {LIMIT, LIMIT, LIMIT, LIMIT, LIMIT} };
			break;

		case 4:
			// минимальный вес 7
			graph = { {LIMIT, 2, 1}, {10, LIMIT, 4}, {1, 1, LIMIT} };
			break;

		case 5:
			int n;
			cout << "Write number of vertices in this graph: ";
			cin >> n;
			cout << endl;
			init_random_DG(graph, n);
			break;

		default:
			cout << "Wrong number - try again!" << endl;
			graph_is_chosen = false;
		}
	} while (!graph_is_chosen);

	cout << "Graph: " << endl;
	print_graph(graph);
	cout << endl;

	cycle = branch_and_bound(graph);
	if (size(cycle.edges))
	{
		cout << "Cost of the cycle: " << cycle.cost << endl;
		cout << "Edges are: ";
		for (auto& i : cycle.edges)
		{
			cout << '(' << i.first << ", " << i.second << ") ";
		}
		cout << endl;
	}
	else
	{
		cout << "No solutions!" << endl;
	}

	return 0;
}