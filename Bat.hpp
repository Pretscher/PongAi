#pragma once

class Bat {
public:
	Bat(float iX, float iY, float iW, float iH, int iPlayer) {
		this->x = iX;
		this->y = iY;
		this->w = iW;
		this->h = iH;
		this->player = iPlayer;
		collisionPossible = true;
	}

	void move(float dest) {
		if (dest > 1.0f) dest = 1.0f;
		if (dest < (-1.0f + this->h)) dest = -1.0f + this->h;
		if (y + velocity > dest && y < dest) {
			y = dest;
			return;
		}
		if (y - velocity < dest && y > dest) {
			y = dest;
			return;
		}
		if (dest > y) {
			y += velocity;
			return;
		}
		if (dest < y) {
			y -= velocity;
			return;
		}
	}

private:
	float x;
	float y;
	float w;
	float h;
	float velocity = 0.025f;
	int player;
	bool collisionPossible;

public:
    inline float getX() const { return x; }
	inline void setX(float x) { this->x = x; }

	inline float getY() const { return y; }
	inline void setY(float y) { this->y = y; }

	inline float getW() const { return w; }
	inline void setW(float w) { this->w = w; }

	inline float getH() const { return h; }
	inline void setH(float h) { this->h = h; }

	inline int getPlayer() const { return player; }
	inline void setPlayer(int player) { this->player = player; }

	inline bool getCollisionPossible() const { return collisionPossible; }
	inline void setCollisionPossible(bool collisionPossible) { this->collisionPossible = collisionPossible; }
};