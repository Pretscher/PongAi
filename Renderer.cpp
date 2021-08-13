#include "Renderer.hpp"
#include <iostream>

//Call this-----------------------------------------------------------------------------------------------------------
sf::RenderWindow* Renderer::currentWindow;
std::vector<sf::RectangleShape*>* lastRectCoords;
bool Renderer::rectsChanged;

void Renderer::init(sf::RenderWindow* window) {
    Renderer::currentWindow = window;
    window->setPosition(sf::Vector2i(0, 0));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
}


void Renderer::loopBeforeDrawing() {

}

void Renderer::loopAfterDrawing() {

}


//Drawing functions----------------------------------------------------------------------------------------------------

void Renderer::drawRect(float x, float y, float width, float height, sf::Color c, bool fill) {
    Renderer::fromCartesianBounds(&width, &height);
    sf::RectangleShape* square = new sf::RectangleShape(sf::Vector2f(width, height));

    square->setOutlineColor(c);
    if (fill == true) {
        square->setFillColor(c);
    }
    else {
        square->setFillColor(sf::Color(0, 0, 0, 0));
    }

    Renderer::fromCartesianCoords(&x, &y);
    square->move(x, y);
    Renderer::currentWindow->draw(*square);
    delete square;
}

void Renderer::drawCircle(float x, float y, float radius, sf::Color c, bool fill) {
    float unusedHelp = 0;
    Renderer::fromCartesianBounds(&radius, &unusedHelp);
    sf::CircleShape* circle = new sf::CircleShape(radius);
    circle->setOutlineColor(c);
    if (fill == true) {
        circle->setFillColor(c);
    }
    else {
        circle->setFillColor(sf::Color(0, 0, 0, 0));
    }
    Renderer::fromCartesianCoords(&x, &y);
    circle->move(x, y);
    Renderer::currentWindow->draw(*circle);
    delete circle;
}

void Renderer::drawLine(float x1, float y1, float x2, float y2) {
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(x1, y1)),
        sf::Vertex(sf::Vector2f(x2, y2))
    };

    Renderer::currentWindow->draw(line, 2, sf::Lines);
}

//Coordinate System Math----------------------------------------------------------------------------------------

void Renderer::fromCartesianCoords(float* x, float* y) {
    auto size = Renderer::currentWindow->getSize();
    *x = (float)(((*x + 1.0f) / 2.0f) * ((float)size.x));
    *y = (float)(((1.0f - *y) / 2.0f) * ((float)size.y));
}

void Renderer::toCartesianCoords(float* x, float* y) {
    auto size = Renderer::currentWindow->getSize();
    *x = (float)((*x * 2.0f / (float)size.x) - 1.0f);
    *y = (float)((-*y * 2.0f / ((float)size.y)) + 1.0f);
}

void Renderer::fromCartesianBounds(float* x, float* y) {
    auto size = Renderer::currentWindow->getSize();
    *x = (float)((*x / 2.0f) * ((float)size.x));
    *y = (float)((*y / 2.0f) * ((float)size.y));
}

void Renderer::toCartesianBounds(float* x, float* y) {
    auto size = Renderer::currentWindow->getSize();
    *x = (float)((*x * 2.0f) / ((float)size.x));
    *y = (float)((*y * 2.0f) / ((float)size.y));
}

sf::Vector2f* Renderer::getMousePos() {
    auto pos = sf::Mouse::getPosition();
    auto offset = currentWindow->getPosition();
    pos.x -= offset.x;
    pos.y -= offset.y;
    float retX = pos.x;
    float retY = pos.y;
    toCartesianCoords(&retX, &retY);
    return new sf::Vector2f(retX, retY);
}