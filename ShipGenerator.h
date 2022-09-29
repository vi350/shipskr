#pragma once 
#include "EnemyShip.h"
#include "Player.h"
#include <vector>

class ShipGenerator
{
private:
	int m_currentLevel = 0;
public:
	std::vector<EnemyShip*> ships;

	ShipGenerator() = default;
	
	void GenerateNextLevel()
	{
		ships.clear();
		m_currentLevel++;
		for (int i = 0; i < m_currentLevel + 1 && i < 10; i++)
			ships.push_back(new EnemyShip(i, 1, 2));
		for(int i = m_currentLevel + 1; i < m_currentLevel + 5 && i < 10; i++)
			ships.push_back(new EnemyShip(i, 2, 1));
		for(int i = m_currentLevel + 5; i < 10; i++)
			ships.push_back(new EnemyShip(i, 3, 0));
	}

	void Reset()
	{
		ships.clear();
		m_currentLevel = 0;
	}


	void UpdateShips(Player *player)
	{
		// Points left text
		string bulletsLeftString = std::to_string(player->points) + " points";
		RenderText(bulletsLeftString, font, textColor, 28, renderer, bulletsLeftString.length() / 2.8, 1.1, 20, 90);

		for (auto ship : ships)
		{
			ship->Render();
			ship->UpdateHorizontalMove();
			for(auto bullet : player->bullets)
				if (ship->CheckCollisionWithObject(bullet))
				{
					player->points += ship->level + 1;
					ships.erase(std::remove(ships.begin(), ships.end(), ship), ships.end());
					player->bullets.erase(std::remove(player->bullets.begin(), player->bullets.end(), bullet), player->bullets.end());
				}
		}
	}
};