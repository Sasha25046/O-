#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Point {
    double x, y;
    int id; 
};

double cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

struct Node {
    Point p;
    Node *prev, *next;
    Node(Point pt) : p(pt), prev(nullptr), next(nullptr) {}
};

void draw_triangle(ofstream& file, Point a, Point b, Point c) {
    file << "<polygon points=\""
         << a.x << "," << a.y << " "
         << b.x << "," << b.y << " "
         << c.x << "," << c.y
         << "\" fill=\"#FFD700\" fill-opacity=\"0.1\" stroke=\"#FFD700\" stroke-width=\"0.5\" />\n";
}

int main() {
    srand(time(nullptr));
    int n;
    
    cout << "Введіть кількість вершин (мінімум 4): ";
    if (!(cin >> n) || n < 4) return 1;

    double size = 800.0;
    Point center = {size / 2.0, size / 2.0};

    vector<Point> vertices;
    for (int i = 0; i < n; ++i) {
        double angle = (2.0 * M_PI * i) / n;
        double r = 150.0 + (rand() % 200);
        vertices.push_back({center.x + cos(angle) * r, center.y + sin(angle) * r, i});
    }

    Node* head = new Node(vertices[0]);
    Node* curr = head;
    for (int i = 1; i < n; ++i) {
        Node* newNode = new Node(vertices[i]);
        curr->next = newNode;
        newNode->prev = curr;
        curr = newNode;
    }
    curr->next = head;
    head->prev = curr;

    ofstream file("star_triangulation.svg");
    file << "<svg width=\"" << size << "\" height=\"" << size << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    file << "<rect width=\"100%\" height=\"100%\" fill=\"#121212\" />\n";

    curr = head;
    int remaining = n;
    int watchdog = 0; 

    while (remaining > 2 && watchdog < n * 10) {
        watchdog++;
        Node* p_node = curr->prev;
        Node* n_node = curr->next;

        if (cross_product(p_node->p, curr->p, n_node->p) > 0) {
            draw_triangle(file, p_node->p, curr->p, n_node->p);
            
            p_node->next = n_node;
            n_node->prev = p_node;
            
            Node* toDelete = curr;
            curr = n_node; 
            delete toDelete;
            remaining--;
        } else {
            curr = curr->next;
        }
    }

    file << "<polygon points=\"";
    for (const auto& p : vertices) file << p.x << "," << p.y << " ";
    file << "\" fill=\"none\" stroke=\"#7CFF01\" stroke-width=\"2\" />\n";

    for (const auto& p : vertices)
        file << "<circle cx=\"" << p.x << "\" cy=\"" << p.y << "\" r=\"3\" fill=\"white\" />\n";

    file << "</svg>";
    file.close();

    if (remaining <= 2 && curr) {
        if (curr->next != curr) delete curr->next;
        delete curr;
    }

    cout << "Готово! \n";
    return 0;
}