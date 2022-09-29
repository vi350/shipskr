#pragma once
#include "GameObject.h"
#include "Time.h"
#include "Bullet.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

extern SDL_Color textColor;
extern TTF_Font* font;

class Player : public GameObject
{
private:
	int m_Speed = 10;
	const std::string m_TexturePath = "../Sprites/playerShip.png";
	bool m_FacesRight;
public:
	vector<Bullet*> bullets;

	const int defaultNumOfBullets = 10;
	int bulletsLeft = 10;
	int points = 0;

	Player(int xPos)
	{
		this->xPos = xPos;
		yPos = SCREEN_HEIGHT - tileSize * 3.5;
		texture = LoadTexture(m_TexturePath, renderer);
	}

	int lastMoveDirection = 0;

	void GetMoveDirectionFromInput(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_a: lastMoveDirection = -1; return;
				case SDLK_d: lastMoveDirection = 1; return;
			}
		}
		else if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_a: case SDLK_d: lastMoveDirection = 0; return;
			}
		}
	}

	bool GetShootInput(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
			return true;
		return false;
	}

	void UpdateBullets()
	{
		for (auto bullet : bullets)
		{
			bullet->UpdateYPos();
			bullet->Render();
			
			if (bullet->yPos < 0)
				bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
		}
	}

	void Shoot()
	{
		bullets.push_back(new Bullet(xPos / tileSize, yPos / tileSize));
		bulletsLeft--;
	}

	void Move()
	{
		if(lastMoveDirection != 0)
			m_FacesRight = lastMoveDirection == 1;

		if (xPos > 0 || xPos < SCREEN_WIDTH - tileSize)
			xPos += m_Speed * Time::GetDeltaTime() * 50 * lastMoveDirection;
	}

	void Render(int sizeInc_ = 1) override
	{
		// Bullets left text
		string bulletsLeftString = std::to_string(bulletsLeft) + " bullets";
		RenderText(bulletsLeftString, font, textColor, 28, renderer, bulletsLeftString.length() / 2.8, 1.1, 20, 60);

		if (!m_FacesRight)
			RenderFlipped(sizeInc_);
		else
			GameObject::Render(sizeInc_);
	}
};