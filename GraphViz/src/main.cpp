#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
void LoadSimpleGraph(SimpleGraph&, ifstream&);
void InitNodePositions(SimpleGraph&);
void UpdateGraph(SimpleGraph&);

const double PI = 3.14159265358979323;
const double K_REPEL = 1e-3;
const double K_ATTRACT = 1e-3;

int main() {
    Welcome();

    string filename;
    double duration;
    SimpleGraph g;

    ifstream file;
    do {
        cout << "Filename of graph to visualize: ";
        getline(cin, filename);
        file.open(filename);
    } while (!file.is_open());

    do {
        cout << "Algorithm runtime duration: ";
        cin >> duration;
    } while (duration < 0);

    LoadSimpleGraph(g, file);

    InitGraphVisualizer(g);

    InitNodePositions(g);
    DrawGraph(g);

    time_t startTime = time(NULL);
    while (difftime(time(NULL), startTime) < duration) {
        UpdateGraph(g);
        DrawGraph(g);
    }

    return 0;
}

void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

void LoadSimpleGraph(SimpleGraph &g, ifstream& file) {
    int n;
    file >> n;
    g.nodes.resize(n);

    size_t n1, n2;
    while (file >> n1 >> n2)
        g.edges.push_back(Edge{.start = n1, .end = n2});
}

void InitNodePositions(SimpleGraph& s) {
    size_t n = s.nodes.size();
    for (size_t i = 0; i < n; i++) {
        s.nodes[i].x = cos(2*PI*i/n);
        s.nodes[i].y = sin(2*PI*i/n);
    }
}

void UpdateGraph(SimpleGraph& s) {
    vector<Node> deltas;
    deltas.resize(s.nodes.size());

    for (size_t i = 0; i < s.nodes.size(); i++) {
        for (size_t j = i + 1; j < s.nodes.size(); j++) {
            Node n1 = s.nodes[i];
            Node n2 = s.nodes[j];

            double f_repel = K_REPEL / sqrt(pow(n2.y - n1.y, 2) + pow(n2.x - n1.x, 2));
            double theta = atan2(n2.y - n1.y, n2.x - n1.x);

            deltas[i].x -= f_repel * cos(theta);
            deltas[i].y -= f_repel * sin(theta);
            deltas[j].x += f_repel * cos(theta);
            deltas[j].y += f_repel * sin(theta);
        }
    }


    for (size_t i = 0; i < s.edges.size(); i++) {
        Edge e = s.edges[i];
        Node n1 = s.nodes[e.start];
        Node n2 = s.nodes[e.end];

        double f_attract = K_ATTRACT * (pow(n2.y - n1.y, 2) + pow(n2.x - n1.x, 2));
        double theta = atan2(n2.y - n1.y, n2.x - n1.x);

        deltas[e.start].x += f_attract * cos(theta);
        deltas[e.start].y += f_attract * sin(theta);
        deltas[e.end].x -= f_attract * cos(theta);
        deltas[e.end].y -= f_attract * sin(theta);
    }

    for (size_t i = 0; i < s.nodes.size(); i++) {
        s.nodes[i].x += deltas[i].x;
        s.nodes[i].y += deltas[i].y;
    }
}
