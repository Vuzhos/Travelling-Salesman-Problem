#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

#define LIMIT INT_MAX
using namespace std;

struct node
{
	int cost;
	int current_vertice;
	vector<vector<int>> matrix;
	vector<pair<int, int>> edges;
	set<int> visited_vertices;
};

// initialization of adjacent matrix of directed graph
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

// printing graph (matrix)
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

// just soting function for nodes
bool comparing_nodes(node a, node b)
{
	if (a.cost < b.cost)
	{
		return 1;
	}
	return 0;
}

// main algotithm
node branch_and_bound(vector<vector<int>> graph)
{
	// just set of all vertices, cuz we need it
	set<int> all_vertices;
	for (int i = 0; i < size(graph); i++)
	{
		all_vertices.insert(i);
	}

	// vector of nodes - we will write here all nodes
	vector<node> nodes;
	nodes.push_back({ 0, 0, graph, {}, { 0 } });

	// main loop
	while (1)
	{
		// we checked all nodes and did not find solution, so we ran out of nodes with possible edges to go
		if (!size(nodes))
		{
			return {};
		}

		// sorting nodes by their cost
		sort(nodes.begin(), nodes.end(), comparing_nodes);

		//initialization of set difference between all vertices and visitied vertices in node with minimal cost
		set<int> diff;
		set_difference(all_vertices.begin(), all_vertices.end(), nodes[0].visited_vertices.begin(), nodes[0].visited_vertices.end(), inserter(diff, diff.begin()));

		// if difference is empty - we got two situations
		if (size(diff) == 0)
		{
			// we found all edges and need to add last one for the cycle if it exists
			if (size(nodes[0].edges) + 1 == size(graph) and (nodes[0].matrix[nodes[0].current_vertice][0] != LIMIT))
			{
				nodes[0].edges.push_back(make_pair(nodes[0].current_vertice, 0));
				nodes[0].cost += nodes[0].matrix[nodes[0].current_vertice][0];
				// we are adding this node to the list to check on a new iteration that it is minimal
				nodes.push_back(nodes[0]);
			}

			// we checked that cycle is actually minimal (end of algotothm)
			else if (size(nodes[0].edges) == size(graph))
			{
				return nodes[0];
			}
		}

		// creating new nodes to the unvisited vertices
		for (auto& j : diff)
		{
			// just checking if we can go to new vertice
			if (nodes[0].matrix[nodes[0].current_vertice][j] != LIMIT)
			{
				// creating new node from node with minimal cost
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

				// adding new node to vector
				nodes.push_back(tmp);
			}
		}
		// erasing old node that we used to get new ones 
		nodes.erase(nodes.begin());
	}
}

int main()
{
	int answer;
	node cycle;
	vector<vector<int>> graph;
	graph = {};
	bool flag_2 = 0;
	do
	{
		cout << "Which graph will be used?\n1 - test 1\n2 - test 2\n3 - test 3\n4 - test 4\n5 - random graph" << endl;
		cin >> answer;
		cout << endl;
		flag_2 = 0;
		switch (answer)
		{
			case 1:
				// minimal weight is 42
				graph = { {LIMIT, 1, 2, 3, 4}, {14, LIMIT, 15, 16, 5}, {13, 20, LIMIT, 17, 6}, {12, 19, 18, LIMIT, 7}, {11, 10, 9, 8, LIMIT} };
				break;

			case 2:
				// minimal weight is 28
				graph = { {LIMIT, 20, 30, 10, 11}, {15, LIMIT, 16, 4, 2}, {3, 5, LIMIT, 2, 4}, {19, 6, 18, LIMIT, 3}, {16, 4, 7, 16, LIMIT} };
				break;

			case 3:
				// no solution
				graph = { {LIMIT, LIMIT, LIMIT, LIMIT, 1}, {LIMIT, LIMIT, 1, LIMIT, LIMIT}, {LIMIT, LIMIT, LIMIT, 1, LIMIT}, {1, LIMIT, LIMIT, LIMIT, LIMIT}, {LIMIT, LIMIT, LIMIT, LIMIT, LIMIT} };
				break;

			case 4:
				// minimal weight is 7
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
				flag_2 = 1;
		}
	} while (flag_2);

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