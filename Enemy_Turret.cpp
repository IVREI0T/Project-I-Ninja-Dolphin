#include "Application.h"
#include "Enemy_Turret.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

Enemy_Turret::Enemy_Turret(int x, int y) : Enemy(x, y)
{
	walk.SetUp(102, 67, 49, 43, 1, 1, "0");
	walk.speed = 0.2f;

	
	animation_shooting.SetUp(152, 67, 50, 43, 3, 3,"0,1,2");
	animation_shooting.speed = 0.2f;

	animation_hurt.SetUp(102, 67, 49, 43, 1, 1, "0");
	
	base.SetUp(210, 112, 53, 48, 1, 1, "0");

	path.PushBack({ 0, 0 }, 10000, &walk);
	

	collider = App->collision->AddCollider({ 0, 67, 49, 43}, COLLIDER_TYPE::COLLIDER_ENEMY_GROUND, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = GROUND;
	hitpoints = 3;

	
	sdl_clock_start = SDL_GetTicks() + 500 + 850;
}

Enemy_Turret::~Enemy_Turret()
{
	walk.CleanUp();
	base.CleanUp();
}

void Enemy_Turret::Draw(SDL_Texture * sprites)
{
	App->collision->SetPosition(collider, position.x, position.y);
	iPoint fdirection = App->player->GetPos() - position;
	sdl_clock = SDL_GetTicks(); 

	switch (state) {
	case REGULAR:
		if (animation != nullptr) {
			App->render->Blit(type, sprites, position.x, position.y, fdirection, &(animation->GetCurrentFrame()));
			App->render->Blit(2, sprites, position.x - 1, position.y - 2, direction, &(base.GetCurrentFrame()));
		}
		break;

	case SHOOTING:
		App->render->Blit(type, sprites, position.x, position.y, fdirection, &(animation_shooting.GetCurrentFrame()));
		App->render->Blit(2, sprites, position.x -1, position.y - 2, direction, &(base.GetCurrentFrame()));
		if (animation_shooting.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;

	case HURT:
		App->render->Blit(type, sprites, position.x, position.y, fdirection, &(animation_hurt.GetCurrentFrame()));
		App->render->Blit(2, sprites, position.x - 1, position.y - 2, direction, &(base.GetCurrentFrame()));
		if (animation_hurt.Finished()) {
			state = REGULAR;
			animation_hurt.Reset();
		}
		break;
	}
	
}


void Enemy_Turret::Move()
{
	sdl_clock = SDL_GetTicks();
	iPoint turret = App->player->GetPos() - position;
	position = original_position + path.GetCurrentPosition(&animation);
	

	if (sdl_clock >= sdl_clock_start &&  position.y - App->render->camera.y <= SCREEN_HEIGHT * 5 / 8)
	{
		shots++;
		iPoint origin = position;
		origin.x += 15;
		origin.y += animation_shooting.CurrentFrame().h - 20;
		Shoot(origin);
		shot = true;

		if (shots >= 1) {
			sdl_clock_start = sdl_clock + 3167;
			shots = 0;
		}
	}
}



void Enemy_Turret::OnCollision(Collider* collider) {

	if (state != HURT) {
		if (--hitpoints == 0) {

			App->particles->AddParticle(EXPLOSION, position.x - 10, position.y - 10);
			App->particles->AddParticle(TURRET_CRATER, position.x, position.y);
			App->player->AddScore(100, collider->type);
		}
		
		}

		else
			state = HURT;
	}



