#pragma once

#include "GameObject.h"
#include "Time.h"
#include <string>
#include "CollisionUtils.h"


class EnemyShip : public GameObject
{
private:
	const int m_Speed = 3;
	int m_MoveDirection = 1;
public:
	const int level;

	EnemyShip(int xPos, int yPos, int level) : GameObject(xPos, yPos, "../Sprites/shipLvl" + std::to_string(level + 1) + ".png"), level(level) {}

	inline bool CheckCollisionWithObject(GameObject* gameObject) { return CheckCollision(this, 1, gameObject, 1); }

	void UpdateHorizontalMove()
	{
		xPos += Time::GetDeltaTime() * m_Speed * (level + 1) * 35 * m_MoveDirection;
		if (xPos > SCREEN_WIDTH - tileSize || xPos < 0)
			m_MoveDirection *= -1;

        if (xPos <=0) {
            xPos++;
        }

        else if (xPos >= SCREEN_WIDTH-100){
            xPos--;
        }
	}
};