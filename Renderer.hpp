#pragma once
#pragma once
#include <SFML/Graphics.hpp>
class Renderer {
public:
    static sf::RenderWindow* currentWindow;

    static void init(sf::RenderWindow* window);
    static void loopBeforeDrawing();
    static void loopAfterDrawing();

    static void fromCartesianCoords(float* x, float* y);
    static void toCartesianCoords(float* x, float* y);
    static void fromCartesianBounds(float* x, float* y);
    static void toCartesianBounds(float* x, float* y);

    static void drawRect(float x, float y, float width, float height, sf::Color c, bool fill);
    static void drawLine(float x1, float y1, float x2, float y2);
    static void createRect(float x, float y, float width, float height, sf::Color c, bool fill);
    static void drawCircle(float x, float y, float radius, sf::Color c, bool fill);

    static bool rectsChanged;
    static sf::Vector2f* getMousePos();
};