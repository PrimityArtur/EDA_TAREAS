#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

class GraphicPoint {
public:
    GraphicPoint(std::vector<float> data, float xSpacing, float ySpacing, const std::string& xAxisName, const std::string& yAxisName);

    void draw(sf::RenderWindow& window);

private:
    float xRange, yRange, xSpacing, ySpacing;
    std::string xAxisName, yAxisName;
    std::vector<float> data;

    struct Frecuencie {
        float range;
        int frequencie;
    };
    std::vector<Frecuencie> frequencies;

    void calculateFrequencies();

    void drawAxes(sf::RenderWindow& window, float chartWidth, float chartHeight);

    void drawTicks(sf::RenderWindow& window, float chartWidth, float chartHeight, sf::Font& font);

    std::string floatToString(float value, int precision = 0);

};
