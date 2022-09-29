#pragma once
#include "GameObject.h"
#include "Time.h"
#include "CollisionUtils.h"


class Bullet : public GameObject
{
private:
	float m_ySpeed = 25;
public:
	Bullet(int xPos, int yPos) : GameObject(xPos, yPos, "../Sprites/bullet.png") {};

	inline void UpdateYPos() { yPos -= Time::GetDeltaTime() * m_ySpeed * 50; }
};