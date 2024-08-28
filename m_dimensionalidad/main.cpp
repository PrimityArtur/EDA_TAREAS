#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "GraphicPoint.h"
#include <thread>

struct Point;
struct Distant;
struct Graph;
struct Node;
struct Adjacent;

typedef std::vector<Point> Vect_point;
typedef std::vector<float> VectD;
typedef std::vector<Adjacent> Vect_adj;
typedef std::vector<Distant> Vect_dist;
typedef std::vector<Node*> Vect_node;
typedef std::vector<std::thread> Vect_th;

//estructura de un punto
struct Point {
    VectD axis;
};

//estructura para la distancia con otro punto adyacente
struct Adjacent {
    Node *adjacent;
    double distant;
};

//Nodo para almacenar un punto con todas las distancias con sus nodos adyacentes
struct Node {
    Point point;
    Vect_adj adjacents;

    Node(Point point) : point(point) {}
};

//estructura de grafo para manipular todos los puntos y sus adyacencias
struct Graph {
    Vect_node points; // almacena todos los puntos
    VectD distants; //almacenar todas las distancias unicas entre los nodos

    //calcula la distancia entre dos puntos
    double euclidianDistance(Point *a, Point *b) {
        double sum = 0;
        for (int i = 0; i < a->axis.size(); i++) 
            sum += std::pow(a->axis[i] - b->axis[i], 2);
        
        return std::sqrt(sum);
    }

    //agrega todas las distancias adyacentes del punto agregado
    void newPointAddAdjacents(Node* node) {
        for (Node* i : points) {
            double distant = euclidianDistance(&node->point, &i->point);

            node->adjacents.push_back({ i, distant} );
            i->adjacents.push_back({ node, distant });
            distants.push_back(distant);
        }
    }

    //agrega un punto
    void addPoint(Point point) {
        Node* node = new Node(point);

        newPointAddAdjacents(node);
        points.push_back(node);
    }

    //Para observar todas las distancias unicas
    void printGraphDistants() {
        int e = 0;
        for (Node *n :points) {
            std::cout << "Punt "<< e++ << ": " << std::endl;
          
            for (Adjacent adj : n->adjacents) 
                std::cout << "    "  << adj.distant << ", " << std::endl;
            std::cout << "\n\n\n";
        }
    }

    //retorna el vector de distancias
    VectD getDists() {
        return distants;
    }

    ~Graph() {
        for (Node* n : points)
            delete n;
    }
};

//genera un punto de 'dim' dimenciones
VectD generate_point(int& dim, std::uniform_real_distribution<>& axi, std::mt19937& gen) {
    VectD v(dim);
    std::generate(v.begin(), v.end(), [&]() { return axi(gen); });
    
    return v;
}

//genera puntos random
void generate_points(Graph &dim, int d, int points = 100, int a = 0, int b = 1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> axi(a, b);

    for (int i = 0; i < points; i++) 
        dim.addPoint( Point{ generate_point(d, axi, gen) } );
}

// funcion para graficar los resultados
void game(VectD data, int xSpacing, int ySpacing, int dim) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "BDistancia entre puntos con dimension " + std::to_string(dim));
    
    GraphicPoint chart(data, xSpacing, ySpacing, "Distancia", "Frecuencia");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        chart.draw(window);
        window.display();
    }
}

int main() {
    Vect_th t;

    Graph dim10;
    generate_points(dim10, 10);
    t.push_back(std::thread(game, dim10.getDists(), 20, 8, 10));

    Graph dim50;
    generate_points(dim50, 50);
    t.push_back(std::thread(game, dim50.getDists(), 20, 8, 50));

    Graph dim100;
    generate_points(dim100, 100);
    t.push_back(std::thread(game, dim100.getDists(), 20, 8, 100));

    Graph dim500;
    generate_points(dim500, 500);
    t.push_back(std::thread(game, dim500.getDists(), 50, 8, 500));

    Graph dim1000;
    generate_points(dim1000, 1000);
    t.push_back(std::thread(game, dim1000.getDists(), 100, 8, 1000));

    Graph dim2000;
    generate_points(dim2000, 2000);
    t.push_back(std::thread(game, dim2000.getDists(), 100, 8, 2000));

    Graph dim5000;
    generate_points(dim5000, 5000);
    t.push_back(std::thread(game, dim5000.getDists(), 200, 8, 5000));

    //threads para graficas cada uno de los resultados
    for (int i = 0; i < t.size(); i++) {
        t[i].join();
    }

    return 0;
}


