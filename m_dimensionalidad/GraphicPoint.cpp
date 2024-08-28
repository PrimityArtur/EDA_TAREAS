#include "GraphicPoint.h"


GraphicPoint::GraphicPoint(std::vector<float> data, float xSpacing, float ySpacing, const std::string& xAxisName, const std::string& yAxisName)
    : data(data), xSpacing(xSpacing), ySpacing(ySpacing),
    xAxisName(xAxisName), yAxisName(yAxisName) {
    calculateFrequencies();
}

void GraphicPoint::draw(sf::RenderWindow& window) {
    float chartWidth = window.getSize().x - 100;
    float chartHeight = window.getSize().y - 100;
    float barWidth = chartWidth / frequencies.size();

    drawAxes(window, chartWidth, chartHeight);

    for (int i = 0; i < frequencies.size(); ++i) {
        float normalizedHeight = (frequencies[i].frequencie / yRange) * chartHeight;
        sf::RectangleShape bar(sf::Vector2f(barWidth - xSpacing, -normalizedHeight));
        bar.setPosition(70 + i * barWidth, chartHeight + 50);
        bar.setFillColor(sf::Color::Blue);
        bar.setOutlineThickness(0.5);
        bar.setOutlineColor(sf::Color::White);
        window.draw(bar);
    }
}

void GraphicPoint::calculateFrequencies() {

    std::sort(data.begin(), data.end());
    xRange = data.back();
    xSpacing = xRange / xSpacing;

    float i = 0, e = 0;
    frequencies.push_back(Frecuencie({ float(xSpacing * (i + 1)), 0 }));
    while (true)
    {
        for (; e < data.size(); e++) {
            if (data[e] <= xSpacing * (i + 1)) {
                frequencies[i].frequencie++;
            }
            else break;
        }
        if (xSpacing * (i + 2) <= xRange) {
            frequencies.push_back(Frecuencie({ float(xSpacing * (i + 2)), 0 }));
            i++;
        }
        else break;
    }

    for (Frecuencie a : frequencies) {
        std::cout << a.frequencie << ", " << a.range << std::endl;
    }

    yRange = std::max_element(frequencies.begin(), frequencies.end(), [](Frecuencie& a, Frecuencie& b) {return a.frequencie < b.frequencie; })->frequencie;
    ySpacing = yRange / ySpacing;

    std::cout << xRange << " " << yRange << " " << xSpacing << " " << ySpacing << std::endl;
}

void GraphicPoint::drawAxes(sf::RenderWindow& window, float chartWidth, float chartHeight) {
    sf::Font font;
    font.loadFromFile("./arial.ttf");

    sf::RectangleShape xAxis(sf::Vector2f(chartWidth, 2));
    xAxis.setPosition(70, chartHeight + 50);
    xAxis.setFillColor(sf::Color::Black);

    sf::RectangleShape yAxis(sf::Vector2f(2, chartHeight));
    yAxis.setPosition(70, 50);
    yAxis.setFillColor(sf::Color::Black);

    window.draw(xAxis);
    window.draw(yAxis);

    sf::Text xAxisLabel(xAxisName, font, 14);
    xAxisLabel.setPosition(chartWidth / 2, chartHeight + 80);
    xAxisLabel.setFillColor(sf::Color::Black);
    window.draw(xAxisLabel);

    sf::Text yAxisLabel(yAxisName, font, 14);
    yAxisLabel.setPosition(0, chartHeight / 2 + 10);
    yAxisLabel.setFillColor(sf::Color::Black);
    yAxisLabel.setRotation(-90);
    window.draw(yAxisLabel);

    drawTicks(window, chartWidth, chartHeight, font);
}

void GraphicPoint::drawTicks(sf::RenderWindow& window, float chartWidth, float chartHeight, sf::Font& font) {
    int xTicks = frequencies.size();
    int aux = 8;
    for (int i = 1; i <= aux; ++i) {
        sf::RectangleShape tick(sf::Vector2f(1, -chartHeight));
        tick.setPosition(70 + i * (chartWidth / aux), chartHeight + 50);
        tick.setFillColor(sf::Color::Black);
        window.draw(tick);

        sf::Text label(floatToString(i * xSpacing * xTicks / aux, 2), font, 15);
        label.setPosition(45 + i * (chartWidth / aux), chartHeight + 60);
        label.setFillColor(sf::Color::Black);
        window.draw(label);
    }

    int yTicks = static_cast<int>(yRange / ySpacing);
    for (int i = 1; i <= yTicks; ++i) {
        sf::RectangleShape tick(sf::Vector2f(chartWidth, 1));
        tick.setPosition(70, chartHeight + 50 - i * (chartHeight / yTicks));
        tick.setFillColor(sf::Color::Black);
        window.draw(tick);

        sf::Text label(floatToString(i * ySpacing, 2), font, 15);
        label.setPosition(20, chartHeight + 45 - i * (chartHeight / yTicks));
        label.setFillColor(sf::Color::Black);
        window.draw(label);
    }
}

std::string GraphicPoint::floatToString(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}
