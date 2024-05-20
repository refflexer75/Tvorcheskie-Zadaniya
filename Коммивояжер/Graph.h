#pragma once

#include "helper_functions.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class Graph {
protected:
    int _max_size;
    int _size;
    int** _adjacency;

    void _nullify();

public:
    Graph(int max_size);
    Graph(const Graph& g);
    ~Graph();

    int max_size() const;
    int size() const;
    int adjacency(int i, int j) const; // матрица смежности (весов)
    vector<int> line(int i) const;     // строка матрицы весов
    vector<int> column(int j) const;   // столбец матрицы весов

    void add_node();         // добавить вершину
    void delete_node(int n); // удалить вершину

    void edit_edge(int n1, int n2, int weight, bool symmetric = true);
    void add_edge(int n1, int n2, int weight = 1);
    void delete_edge(int n1, int n2, bool symmetric = true);

    void read(istream& in);
    void show(ostream& out) const;

    vector<int> neighborhood(int n) const;
    vector<int> dijkstra(int n) const;

    vector<int> salesman() const;
};


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void Graph::_nullify() {
    this->_size = 0;

    for (int i = 0; i < _max_size; ++i)
        for (int j = 0; j < _max_size; ++j)
            this->_adjacency[i][j] = -1;
}

Graph::Graph(int max_size) {
    this->_max_size = max_size;

    this->_adjacency = new int* [max_size];
    for (int i = 0; i < max_size; ++i)
        this->_adjacency[i] = new int[max_size];

    this->_nullify();
};

Graph::Graph(const Graph& g) {
    int max_size = g.max_size();
    this->_max_size = max_size;

    this->_adjacency = new int* [max_size];
    for (int i = 0; i < max_size; ++i)
        this->_adjacency[i] = new int[max_size];

    this->_size = g.size();

    for (int i = 0; i < max_size; ++i)
        for (int j = 0; j < max_size; ++j)
            this->_adjacency[i][j] = g.adjacency(i, j);
}

Graph::~Graph() {
    for (int i = 0; i < this->_size; ++i)
        delete[] this->_adjacency[i];

    delete[] this->_adjacency;
}

int Graph::max_size() const { return this->_max_size; }

int Graph::size() const { return this->_size; }

int Graph::adjacency(int i, int j) const { return this->_adjacency[i][j]; }

vector<int> Graph::line(int i) const {
    vector<int> res(this->_size, 0);

    if (i >= this->size())
        return res;

    for (int j = 0; j < this->_size; ++j) {
        res[j] = this->_adjacency[i][j];
    }

    return res;
}

vector<int> Graph::column(int j) const {
    vector<int> res(this->_size, 0);

    if (j >= this->size())
        return res;

    for (int i = 0; i < this->_size; ++i) {
        res[i] = this->_adjacency[i][j];
    }

    return res;
}

void Graph::add_node() {
    int& size = this->_size;

    size++;
    for (int i = 0; i < size; ++i) {
        this->_adjacency[size][i] = -1;
        this->_adjacency[i][size] = -1;
    }
}

void Graph::delete_node(int n) {
    int& size = this->_size;

    for (int i = n; i < size - 1; ++i)
        for (int j = 0; j < n; ++j)
            this->_adjacency[i][j] = this->_adjacency[i + 1][j];

    for (int j = n; j < size - 1; ++j)
        for (int i = 0; i < n; ++i)
            this->_adjacency[i][j] = this->_adjacency[i][j + 1];

    for (int i = n; i < size - 1; ++i)
        for (int j = n; j < size - 1; ++j)
            this->_adjacency[i][j] = this->_adjacency[i + 1][j + 1];

    for (int i = 0; i < size; ++i) {
        this->_adjacency[size][i] = -1;
        this->_adjacency[i][size] = -1;
    }

    size--;
}

void Graph::edit_edge(int n1, int n2, int weight, bool symmetric) {
    if (symmetric) {
        this->_adjacency[n1][n2] = weight;
        this->_adjacency[n2][n1] = weight;
    }
    else {
        this->_adjacency[n1][n2] = weight;
    }
}

void Graph::add_edge(int n1, int n2, int weight) {
    edit_edge(n1, n2, weight);
}

void Graph::delete_edge(int n1, int n2, bool symmetric) {
    edit_edge(n1, n2, -1, symmetric);
}

void Graph::read(istream& in) {
    this->_nullify();

    string line;
    vector<int> line_splited;

    getline(in, line);
    this->_size = stoi(line);

    for (int i = 0; i < this->_size; ++i) {
        getline(in, line);
        line_splited = line_to_vec(line);
        line_splited.resize(this->_size);
        for (int j = 0; j < this->_size; ++j) {
            if (line_splited[j] == 0)
                this->_adjacency[i][j] = -1;
            else
                this->_adjacency[i][j] = line_splited[j];
        }
    }
}

void Graph::show(ostream& out) const {
    int size = this->_size;

    out << size << endl;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - 1; ++j)
            if (this->_adjacency[i][j] < 0)
                out << "0 ";
            else
                out << this->_adjacency[i][j] << " ";

        if (this->_adjacency[i][size - 1] < 0)
            out << "0" << endl;
        else
            out << this->_adjacency[i][size - 1] << endl;
    }
}

vector<int> Graph::neighborhood(int n) const {
    vector<int> res;

    if (n >= this->size())
        return res;

    for (int i = 0; i < this->_size; ++i)
        if (this->_adjacency[n][i] >= 0)
            res.push_back(i);

    return res;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

vector<int> Graph::dijkstra(int n) const {
    int size = this->_size;

    vector<int> res(size, 0);
    vector<bool> is_used(size, false);

    int current = n;
    for (int i = 0; i < size - 1; ++i) {
        is_used[current] = true;

        for (auto node : this->neighborhood(current)) {
            bool c1 = (is_used[node] == false);
            bool c2 = (res[node] == 0);
            bool c3 = ((res[current] + this->_adjacency[current][node]) < res[node]);

            if (c1 && (c2 || c3))
                res[node] = res[current] + this->_adjacency[current][node];
        }

        int candidate = 0;
        while (is_used[candidate])
            candidate++;

        int min = candidate;

        for (int node = min + 1; node < size; node++) {
            bool c1 = (is_used[node] == false);
            bool c2 = (res[node] > 0);
            bool c3 = (res[node] < res[min]);

            if (c1 && c2 && c3)
                min = node;
        }

        current = min;
    }
    return res;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int find_min_salesman(vector<int>& a)
{
    int pos = 0;

    while (a[pos] < 0) pos++;

    for (int i = 1; i < a.size(); ++i)
        if (a[i] >= 0 && a[i] < a[pos])
            pos = i;

    return pos;
}

void lines_reduction(Graph& g)
{
    for (int i = 0; i < g.size(); ++i) {
        vector<int> line = g.line(i);
        int pos = find_min_salesman(line);
        if (pos >= g.size())
            continue;

        int min = g.adjacency(i, pos);

        for (int j = 0; j < g.size(); ++j) {
            if (g.adjacency(i, j) < 0)
                continue;

            if (g.adjacency(i, j) >= 0)
                g.edit_edge(i, j, g.adjacency(i, j) - min, false);
        }
    }
}

void columns_reduction(Graph& g)
{
    for (int j = 0; j < g.size(); ++j) {
        vector<int> column = g.column(j);
        int pos = find_min_salesman(column);
        if (pos >= g.size())
            continue;

        int min = g.adjacency(pos, j);

        for (int i = 0; i < g.size(); ++i) {
            if (g.adjacency(i, j) < 0)
                continue;

            if (g.adjacency(i, j) >= 0)
                g.edit_edge(i, j, g.adjacency(i, j) - min, false);
        }
    }
}

pair<int, int> zeros_score(Graph& g)
{
    vector<vector<int>> e;
    for (int i = 0; i < g.size(); ++i)
        for (int j = 0; j < g.size(); ++j) {
            if (g.adjacency(i, j) != 0)
                continue;

            vector<int> line = g.line(i);
            line[j] = -1;
            vector<int> column = g.column(j);
            column[i] = -1;

            int pos1 = find_min_salesman(line);
            int pos2 = find_min_salesman(column);
            int min1 = 0;
            int min2 = 0;
            if (pos1 < g.size())
                min1 = line[pos1];
            if (pos2 < g.size())
                min2 = column[pos2];

            vector<int> note = { min1 + min2, i, j };
            e.push_back(note);
        }

    int pos = 0;
    for (int i = 0; i < e.size(); ++i)
        if (e[i][0] > e[pos][0])
            pos = i;

    pair<int, int> res = { e[pos][1], e[pos][2] };
    return res;
}

int path_lenght(const Graph& g, const vector<int>& path)
{
    int res = 0;

    for (int i = 0; i < path.size() - 1; ++i)
        res += g.adjacency(path[i], path[i + 1]);

    return res;
}

vector<int> Graph::salesman() const
{
    vector<int> res;

    Graph opt = *this;

    for (int iter = 0; iter < opt.size() - 2; ++iter) {
        lines_reduction(opt);

        columns_reduction(opt);

        pair<int, int> max_zero = zeros_score(opt);

        int i0 = max_zero.first;
        int j0 = max_zero.second;

        for (int i = 0; i < opt.size(); ++i) {
            opt.delete_edge(i, j0, false);
            opt.delete_edge(i0, i, false);
        }

        res.push_back(max_zero.first);
        res.push_back(max_zero.second);
    }

    res.push_back(res[0]);

    return res;
}
