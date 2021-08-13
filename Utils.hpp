#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Renderer.hpp"
namespace Utils {
	bool collisionCoordsRect(float rX, float rY, float rW, float rH, float pX, float pY) {
		if (rX < pX && rX + rW > pX) {
			if (rY > pY && rY - rH < pY) {
				return true;
			}
		}
		return false;
	}
	/**
	* @param top left coords of and circle
	**/
	bool collisionCoordsCircle(float cX, float cY, float cR, float pX, float pY) {
		float dist = sqrt((pX - (cX + cR)) * (pX - (cX + cR)) + (pY - (cY - cR)) * (pY - (cY - cR)));
		if (dist < cR) return true;
		return false;
	}

	/**
	* @param top left coords of rect and circle, colPointDist is accuracy of detection
	**/
	bool collisionRectCircle(float aX, float aY, float aW, float aH, float cX, float cY, float cR, float colPointDist) {
		//check points of second rect for intersection with circle with given accuracy
		for (float x = aX; x < aX + aW; x += colPointDist) {
			for (float y = aY; y > aY - aH; y -= colPointDist) {
				//Renderer::drawRect(x, y, 0.005f, 0.005f, sf::Color(255, 255, 0, 255), true);
				if (collisionCoordsCircle(cX, cY, cR, x, y) == true) {
					return true;
				}
			}
		}
		return false;
	}

	bool collisionRects(float aX, float aY, float aW, float aH, float bX, float bY, float bW, float bH, float colPointDist) {
		//check points of second rect for intersection with first rect with given accuracy
		for (float x = bX; x < bX + bW; x += colPointDist) {
			for (float y = bY; y > bY - bH; y -= colPointDist) {
				if (collisionCoordsRect(aX, aY, aW, aH, x, y) == true) {
					return true;
				}
			}
		}
		return false;
	}

	sf::Vector2f* getLeftClickPos() {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			float x = sf::Mouse::getPosition().x;
			float y = sf::Mouse::getPosition().y;
			Renderer::toCartesianCoords(&x, &y);
			return new sf::Vector2f(x, y);
		}
		return nullptr;
	}

	float RandomNumber(float Min, float Max)
	{
		return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
	}


	float calcDist1D(float y1, float y2) {
		float dist = 0;
		float a = abs(y1);
		float b = abs(y2);
		if (a >= b) {
			dist = a - b;
		}
		if (b > a) {
			dist = b - a;
		}
		return dist;
	}
};