#pragma once
#include <iostream>
#include "Bat.hpp"
#include "Utils.hpp"
class Ball {
public:
	Ball(float iX, float iY) {
		this->x = iX;
		this->y = iY;
		velocity = 0.8f;
		xMotion = -0.02f;
		yMotion = -0.02f;
		maxVel = abs(xMotion) + abs(yMotion);
	}

private:
	float x;
	float y;
	float radius = 0.05f;
	float velocity;
	float xMotion;
	float yMotion;
	float maxVel;
public:

	void move() {
		if (speedMultAfterReset < 1.0f) {
			speedMultAfterReset += 0.01f;
		}
		x += xMotion * velocity * speedMultAfterReset;
		y += yMotion * velocity * speedMultAfterReset;
		/*if (x > 0.95f - this->radius * 2) {
			xMotion = -xMotion;
		}
		else if (x < -0.95f) {
			xMotion = -xMotion;
		}*/
		if (y > 0.95f) {
			yMotion = -yMotion;
		}
		else if (y < -0.95f + this->radius * 2) {
			yMotion = -yMotion;
		}
		//ensure that the ball doesn't get too stuck by low angles of momentum
	}

	void collision(Bat* bat) {
		float nextX = x + xMotion * velocity;
		float nextY = y + yMotion * velocity;
		if (bat->getCollisionPossible() == true) {
			if (Utils::collisionRectCircle(bat->getX(), bat->getY(), bat->getW(), bat->getH(), 
				nextX, nextY, this->radius, 0.01f) == true) {
				this->changeDirection();
				bat->setCollisionPossible(false);
			}
		}
	}

	void changeDirection() {
		xMotion = -xMotion;
		if (xMotion > 0) {
			xMotion += (rand() % 5) / 100.0f;
		}
		if (xMotion < 0) {
			xMotion -= (rand() % 5) / 100.0f;
		}
		if (yMotion > 0) {
			yMotion += (rand() % 5) / 100.0f;
		}
		if (yMotion < 0) {
			yMotion -= (rand() % 5) / 100.0f;
		}
		this->keepVelocity();
	}

	float speedMultAfterReset = 1.0f;
	void resetMotion() {
		xMotion = (rand() % 10 - 5.0f) / 100.0f;
		yMotion = (rand() % 10 - 5.0f) / 100.0f;
		speedMultAfterReset = 0.25f;
		this->keepVelocity();
	}

	void keepVelocity() {
		//velocity should be the same as before, only direction should change

		float diff = maxVel - abs(xMotion) - abs(yMotion);
		if (xMotion > 0) {
			//if v is too big, we have a negative and vice versa.
			xMotion += diff / 2;
		}
		if (xMotion < 0) {
			//if v is too big, we add by double negation. Else we increase v by subtraction
			xMotion -= diff / 2;
		}
		if (yMotion > 0) {
			//if v is too big, we have a negative and vice versa.
			yMotion += diff / 2;
		}
		if (yMotion < 0) {
			//if v is too big, we add by double negation. Else we increase v by subtraction
			yMotion -= diff / 2;
		}
		if (xMotion < 0.01f && xMotion >= 0) xMotion = 0.01f;
		if (xMotion > -0.01f && xMotion <= 0) xMotion = -0.01f;
		if (yMotion < 0.01f && yMotion >= 0) yMotion = 0.01f;
		if (yMotion > -0.01f && yMotion <= 0) yMotion = -0.01f;
	}

	inline float getX() const { return x; }
	inline void setX(float x) { this->x = x; }

	inline float getY() const { return y; }
	inline void setY(float y) { this->y = y; }

	inline float getR() const { return radius; }
	inline void setR(float r) { this->radius = r; }

	inline float getVel() const { return velocity; }

	inline float getXmotion() const { return xMotion; }
	inline float getYmotion() const { return yMotion; }
};