#include "Application.h"
#include "Enemy_Bomb.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"

Enemy_Bomb::Enemy_Bomb(int x, int y) : Enemy(x, y)
{
	walk.SetUp(0, 181, 44, 68, 3, 3, "0,1,2");
	walk.speed = 0.2f;

	animation_hurt.SetUp(0, 181, 44, 68, 4, 4, "3,0,3,0,3");
	shadow.SetUp(176, 215, 22, 35, 1, 1, "0");

	path.PushBack({ 0, 0.5f }, 100, &walk);

	collider = App->collision->AddCollider({ 0, 190, 45, 70 }, COLLIDER_TYPE::COLLIDER_ENEMY_AIR, (Module*)App->enemies);

	original_position = position;

	original_y = y;

	type = AIRBORNE;
	hitpoints = 3;

}

Enemy_Bomb::~Enemy_Bomb()
{
	walk.CleanUp();
}

void Enemy_Bomb::Move()
{
	position = original_position + path.GetCurrentPosition(&animation);
}

void Enemy_Bomb::Damaged()
{
	damage.SetUp(196, 182, 55, 70, 1, 1, "0");
}

void Enemy_Bomb::OnCollision(Collider* collider) {
	if (state != HURT) {
		if (--hitpoints == 0) {
			App->particles->AddParticle(EXPLOSION, position.x, position.y);
			App->player->AddScore(50);

			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 1,0 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { -1,0 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 0,1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 0,-1 });

			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 1,1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { -1,1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { 1,-1 });
			App->particles->AddParticle(ENEMYSHOT, position.x, position.y, { -1,-1 });

		}
		else
			state = HURT;
	}

}