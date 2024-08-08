#include <random>
#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define PLAYER_SPEED 5.0f
#define PLAYER_RADIUS 10

#define ENEMY_RADIUS 10.0f
#define ENEMY_SPEED 5.0f
#define MAX_ENEMY_AMOUNT 50

struct Enemy
{
	float speed;
	Color color;
	float radius;
	Vector2 position;
};

Enemy GenerateEnemy();
void PlayerMovement(Vector2 *playerPosition);
void DrawTopEnemies(std::vector<Enemy> *enemies);
void UpdateEnemyPosition(std::vector<Enemy> *enemies);
void CheckBorderCollision(Vector2 *objectPosition, int objectRadius);

int main(void)
{
	SetTargetFPS(60);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Asteroid");
	Vector2 playerPosition = {(float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2};

	std::vector<Enemy> enemies = std::vector<Enemy>();
	while (!WindowShouldClose())
	{
		PlayerMovement(&playerPosition);
		CheckBorderCollision(&playerPosition, PLAYER_RADIUS);

		ClearBackground(BLACK);
		BeginDrawing();

		double time = GetTime();
		if ((int)time % 2 == 0 && enemies.size() < MAX_ENEMY_AMOUNT)
		{
			for (int i = 0; i < 1; i++)
			{
				Enemy enemy = GenerateEnemy();
				enemies.push_back(enemy);
			}
		}

		std::string ss = std::to_string(enemies.size());

		UpdateEnemyPosition(&enemies);
		DrawCircleV(playerPosition, PLAYER_RADIUS, WHITE);
		DrawText(ss.c_str(), 10, 50, 50, GREEN);
		DrawTopEnemies(&enemies);
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

void PlayerMovement(Vector2 *playerPosition)
{
	if (IsKeyDown(KEY_S))
	{
		playerPosition->y += PLAYER_SPEED;
		/* std::cout << "y position: " << playerPosition->y << std::endl; */
	}
	if (IsKeyDown(KEY_W))
	{
		playerPosition->y -= PLAYER_SPEED;
		/* std::cout << "y position: " << playerPosition->y << std::endl; */
	}
	if (IsKeyDown(KEY_D))
	{
		playerPosition->x += PLAYER_SPEED;
		/* std::cout << "x position: " << playerPosition->x << std::endl; */
	}
	if (IsKeyDown(KEY_A))
	{
		playerPosition->x -= PLAYER_SPEED;
		/* std::cout << "x position: " << playerPosition->x << std::endl; */
	}
}

void CheckBorderCollision(Vector2 *objectPosition, int objectRadius)
{
	if (objectPosition->y <= objectRadius)
		objectPosition->y = objectRadius;
	if (objectPosition->y >= (WINDOW_HEIGHT - objectRadius))
		objectPosition->y = (WINDOW_HEIGHT - objectRadius) - 1;

	if (objectPosition->x <= objectRadius)
		objectPosition->x = objectRadius;
	if (objectPosition->x >= (WINDOW_WIDTH - objectRadius))
		objectPosition->x = (WINDOW_WIDTH - objectRadius) - 1;
}

void DrawTopEnemies(std::vector<Enemy> *enemies)
{
	for (int i = 0; i < enemies->size(); i++)
	{
		Enemy target = (*enemies)[i];
		DrawCircleV(target.position, target.radius, target.color);
	}
}

Enemy GenerateEnemy()
{
	float random = rand() % WINDOW_WIDTH;
	Vector2 enemyPosition = {random, (float)(0 + 15)};
	return {ENEMY_SPEED, RED, ENEMY_RADIUS, enemyPosition};
}

void UpdateEnemyPosition(std::vector<Enemy> *enemies)
{
	for (int i = 0; i < enemies->size(); i++)
	{
		(*enemies)[i].position.y += ENEMY_SPEED;
		if ((*enemies)[i].position.y >= 800)
		{
			(*enemies).erase((*enemies).begin() + i);
		}
	}
}