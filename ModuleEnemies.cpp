#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleTextures.h"
#include "Enemy.h"
#include "Enemy_LightAirship.h"
#include "Enemy_LightTank.h"
#include "Enemy_BonusAirship.h"
#include "Enemy_Bomb.h"
#include "Enemy_Kamikaze.h"
#include "Enemy_Box.h"
#include "Enemy_Turret.h"
#include "Enemy_MoonAirship.h"
#include "Enemy_Boss.h"
#include "Enemy_RotatoryTank.h"

#define SPAWN_MARGIN 300

ModuleEnemies::ModuleEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	sprites = App->textures->Load("revamp_spritesheets/enemy_spritesheet.png");

	return true;
}

update_status ModuleEnemies::PreUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].y * SCREEN_SIZE > App->render->camera.y * SCREEN_SIZE - SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].y * SCREEN_SIZE);
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before render is available
update_status ModuleEnemies::Update()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(sprites);

	return UPDATE_CONTINUE;
}

update_status ModuleEnemies::PostUpdate()
{
	// check camera position to decide what to spawn
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->to_delete || enemies[i]->position.y * SCREEN_SIZE > App->render->camera.y + (App->render->camera.h * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("DeSpawning 9enemy at %d", enemies[i]->position.y * SCREEN_SIZE);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	App->textures->Unload(sprites);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		queue[i].type = NO_TYPE;
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = SCREEN_WIDTH / 2 + (x - STAGE_WIDTH / 2);
			queue[i].y = y - STAGE_HEIGHT + SCREEN_HEIGHT;
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleEnemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::LIGHTAIRSHIP:
			enemies[i] = new Enemy_LightAirship(info.x, info.y);
			break;
		case ENEMY_TYPES::LIGHTTANK:
		case ENEMY_TYPES::LIGHTTANK_2:
		case ENEMY_TYPES::LIGHTTANK_3:
			enemies[i] = new Enemy_LightTank(info.x, info.y, info.type);
			break;
		case ENEMY_TYPES::BONUSAIRSHIP:
			enemies[i] = new Enemy_BonusAirship(info.x, info.y);
			break;
		case ENEMY_TYPES::BOMB:
			enemies[i] = new Enemy_Bomb(info.x, info.y);
			break;
		case ENEMY_TYPES::KAMIKAZE:
			enemies[i] = new Enemy_Kamikaze(info.x, info.y);
			break;
		case ENEMY_TYPES::BOX:
			enemies[i] = new Enemy_Box(info.x, info.y);
			break;
		case ENEMY_TYPES::TURRET:
			enemies[i] = new Enemy_Turret(info.x, info.y);
			break;
		case ENEMY_TYPES::MOONAIRSHIP:
			enemies[i] = new Enemy_MoonAirship(info.x, info.y);
			break;
		case ENEMY_TYPES::BOSS:
		case ENEMY_TYPES::BOSS_2:
			enemies[i] = new Enemy_Boss(info.x, info.y, info.type);
			break;
		case ENEMY_TYPES::ROTATORYTANK:
			enemies[i] = new Enemy_RotatoryTank(info.x, info.y);
			break;

		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2);
			if (enemies[i]->hitpoints == 0) {
				delete enemies[i];
				enemies[i] = nullptr;
				break;
			}
		}
	}
}