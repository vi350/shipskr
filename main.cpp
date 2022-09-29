#include <iostream>
#include <SDL.h>
#include "utils.h"
#include "Time.h"
#include <string>
#include "fileManager.h"
#include "Player.h"
#include "ShipGenerator.h"
#include <ctime>



#pragma region SDL STUFF
SDL_Event e;
SDL_Window* window;
SDL_Renderer* renderer;

#pragma region TEXT RENDERING STUFF
SDL_Color nickInputColor = { 23, 255, 123 };
SDL_Color
textColor = { 23, 123, 255 };
SDL_Color createrColor = { 255, 23, 123 };
TTF_Font* font;
#pragma endregion TEXT RENDERING STUFF

#pragma endregion SDL STUFF
FileWithRecords* records = new FileWithRecords();;

#pragma region SceneManagment
enum Scene
{
    Game, Menu, Records, NameInput
};

Scene currentScene = Scene::NameInput;

bool ShowGameScene();
bool ShowMenuScene();
bool ShowInputNameScene();
bool ShowRecordsScene();

#pragma endregion

GameObject* background;

#pragma region MENU SCENE STUFF

// Координаты мыши
int mx, my;
GameObject* menuImage, * tutorialImage, * tutorialButton;
bool tutorialIsActive = false;

#pragma endregion MENU SCENE STUFF

int playerMoveDirection = 0;
Player* player;
string playerName = "_";


ShipGenerator* shipGenerator = new ShipGenerator();


bool Update()
{
    Time::SetNow(SDL_GetPerformanceCounter());
    SDL_RenderClear(renderer);

    if (currentScene == Scene::NameInput && !ShowInputNameScene())
        return false;
    else if (currentScene == Scene::Menu && !ShowMenuScene())
        return false;
    else if (currentScene == Scene::Records && !ShowRecordsScene())
        return false;
    else if (currentScene == Scene::Game && !ShowGameScene())
        return false;

    SDL_RenderPresent(renderer);
    Time::Calculate();
    return true;
}


int main(int argc, char** argv)
{
    srand(time(0));
    TTF_Init();
    window = CreateWindow();
    renderer = CreateRenderer(window);
    font = TTF_OpenFont("../progressPixel.ttf", 40);

    player = new Player(SCREEN_WIDTH / 2);
    background = new GameObject(0, 0, "../Sprites/background.jpg");

    menuImage = new GameObject(5, 5, "../Sprites/menu.png");
    tutorialImage = new GameObject(5, 5, "../Sprites/tutorial.png");
    tutorialButton = new GameObject(0, 6 * 2 + 5, "../Sprites/button.png");


    while (Update()) {
    }

    return 0;
}



bool ShowRecordsScene()
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    currentScene = Scene::Menu;
                    break;
                }
            }
        }
        if (e.type == SDL_QUIT)
            return false;
    }

    string recordString;

    for (int i = 0; i < 10; i++)
    {
        recordString = std::to_string(i + 1) + ": " + records->GetRecordString(i);
        RenderText(recordString, font, textColor, 10, renderer, recordString.length() / 1.5, 2,
                   20,
                   (i * 2 + 1) * tileSize + tileSize / 2.2);
    }

    return true;
}

bool ShowMenuScene()
{
    SDL_GetMouseState(&mx, &my);

    while (SDL_PollEvent(&e))
    {

        if (e.type == SDL_KEYDOWN)
        {

            switch (e.key.keysym.sym)
            {
                case SDLK_SPACE:
                {
                    currentScene = Scene::Game;
                    break;
                }
                case SDLK_r:
                {
                    currentScene = Scene::Records;
                    break;
                }
                case SDLK_ESCAPE:
                {
                    return false;
                    break;
                }
            }
        }
        if (e.type == SDL_QUIT)
            return false;
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (mx > tutorialButton->xPos && mx < tutorialButton->xPos + tileSize * 2
                && my > tutorialButton->yPos && my < tutorialButton->yPos + tileSize * 2 && !tutorialIsActive)
                tutorialIsActive = true;
            else
                tutorialIsActive = false;

        }
    }

    menuImage->Render(15);
    if (tutorialIsActive)
        tutorialImage->Render(15);

    tutorialButton->Render(2);

    return true;
}

void ResetGame(bool setNewRecord = true)
{
    if(setNewRecord)
        records->SetNewRecord(player->points, playerName);
    player->bulletsLeft = player->defaultNumOfBullets;
    player->points = 0;
    player->bullets.clear();
    if (setNewRecord)
        shipGenerator->Reset();
    else
        shipGenerator->ships.clear();
}


bool ShowGameScene()
{
    background->Render(30);
    if (player->bulletsLeft >= 0)
    {

        while (SDL_PollEvent(&e))
        {
            player->GetMoveDirectionFromInput(e);
            if (player->GetShootInput(e))
                player->Shoot();
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                ResetGame();
                currentScene = Scene::Menu;
                return true;
            }
            if (e.type == SDL_QUIT)
                return false;
        }

        if (shipGenerator->ships.size() <= 0)
        {
            ResetGame(false);
            shipGenerator->GenerateNextLevel();
        }

        shipGenerator->UpdateShips(player);

        player->UpdateBullets();
        player->Move();

        player->Render(2);
    }
    else
    {
        string gameOverText = "Game over. " + std::to_string(player->points) + " points.";
        RenderText(gameOverText, font, textColor, 28, renderer, gameOverText.length() / 2, 2,
                   SCREEN_WIDTH / 2 - gameOverText.length() * 8,
                   SCREEN_HEIGHT - SCREEN_HEIGHT / 3);
        gameOverText = "Press space to exit.";
        RenderText(gameOverText, font, textColor, 28, renderer, gameOverText.length() / 2, 2,
                   SCREEN_WIDTH / 2 - gameOverText.length() * 8,
                   SCREEN_HEIGHT - SCREEN_HEIGHT / 7);
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    ResetGame();
                    currentScene = Scene::Menu;
                }
            }
            if (e.type == SDL_QUIT)
                return false;
        }
    }
    return true;
}

bool ShowInputNameScene()
{
    RenderText("Created by student of O716B - Busyrev Victor", font, createrColor, 20, renderer, 12, 1.6,
               SCREEN_WIDTH / 4,
               SCREEN_HEIGHT / 10);

    RenderText("Input your name: (length < 10)", font, textColor, 20, renderer, 12, 1.6,
               SCREEN_WIDTH / 4,
               SCREEN_HEIGHT / 4);
    RenderText(playerName, font, nickInputColor, 20, renderer, playerName.length() / 1.3, 1.8,
               SCREEN_WIDTH / 2 - playerName.length() * 12,
               SCREEN_HEIGHT / 2);

    string navText = "SPACE - continue";

    RenderText(navText, font, textColor, 20, renderer, navText.size() / 1.2, 2,
               SCREEN_WIDTH / 2 - navText.size() * 12,
               SCREEN_HEIGHT - SCREEN_HEIGHT / 4);

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_BACKSPACE:
                {
                    playerName = "_";
                    break;
                }
                case SDLK_SPACE:
                {
                    if (playerName.size() < 10)
                    {
                        if (playerName == "_")
                            playerName = "Default";
                        currentScene = Menu;
                    }
                    else
                        playerName = "_";
                    break;
                }
            }
        }
        else if (e.type == SDL_TEXTINPUT)
        {
            if (playerName == "_")
                playerName = "";
            playerName += e.text.text;
        }
        if (e.type == SDL_QUIT)
            return false;
    }
    return true;
}
