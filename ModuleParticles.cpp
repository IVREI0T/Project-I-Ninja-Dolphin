#include <math.h>
#include <stdlib.h>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleEnemies.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		active[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");

	destroy_b_air = App->audio->LoadSFX("sfx/destroy_b_air.wav");
	graphics = App->textures->Load("revamp_spritesheets/particles_spritesheet.png");

	autoattack.anim.SetUp(0, 0, 5, 14, 4, 4, "0,1,2,3");
	autoattack.anim.loop = false;
	autoattack.anim.speed = 0.3f;
	autoattack.life = 1500;
	autoattack.speed = { 0, -14};
	autoattack.fx = App->audio->LoadSFX("sfx/shot_regular.wav");
	
	
	explosion.anim.SetUp(0, 14, 69, 66, 8, 8, "0,1,2,3,4,5,6,7");
	explosion.anim.loop = false;
	explosion.anim.speed = 0.19f;
	explosion.life = 700;
	explosion.speed = { 0, 0};
	explosion.fx = destroy_b_air;
	

	enemyshot.anim.SetUp(20, 0, 8, 8, 4, 4, "0,1,2,3");
	enemyshot.anim.loop = true;
	enemyshot.anim.speed = 0.3f;
	enemyshot.life = 3000;
	enemyshot.speed = { 0, 5 + App->game_loops/2 };

	crater.anim.SetUp(0, 157, 66, 60, 3, 3, "0,1,2");
	crater.anim.loop = true;
	crater.anim.speed = 0.2f;
	crater.life = 8000;
	crater.speed = { 0,0 };

	big_explosion.anim.SetUp(0, 216, 135, 126, 4, 8, "0,1,2,3,4,5,6,7");
	big_explosion.anim.loop = false;
	big_explosion.anim.speed = 0.2f;
	big_explosion.life = 700;
	big_explosion.speed = { 0,0 };
	big_explosion.fx = destroy_b_air;

	laserattack.anim.SetUp(91, 126, 3, 30, 3, 3, "0,1,2");
	laserattack.anim.loop = true;
	laserattack.speed = { 0, -14 };
	laserattack.life = 1500;
	laserattack.fx = App->audio->LoadSFX("sfx/shot_laser.wav");

	light_explosion.anim.SetUp(0, 874, 64, 60, 7, 12, "0,1,2,3,4,5,6,7,8,9,10,11");
	light_explosion.anim.loop = false;
	light_explosion.anim.speed = 0.2f;
	light_explosion.life = 600;
	light_explosion.fx = destroy_b_air;

	turret_crater.anim.SetUp(199, 157, 54, 48, 3, 3, "0,1,2");
	turret_crater.anim.loop = true;
	turret_crater.anim.speed = 0.2f;
	turret_crater.life = 8000;

	missile.anim.SetUp(90, 93, 11, 30, 3, 3, "0,1,2");
	missile.anim.loop = true;
	missile.anim.speed = 0.2f;
	missile.acceleration = { 0,-1 };
	missile.life = 6000;
	missile.speed = { 0, -2 };

	bombshot.anim.SetUp(168, 106, 12, 29, 2, 2, "0,1");
	bombshot.anim.loop = true;
	bombshot.anim.speed = 0.2f;
	bombshot.life = 1000;
	bombshot.speed = { 0,-4 };
	bombshot.fx = App->audio->LoadSFX("sfx/bomb_deploy.wav");
	
	bombexplosion.anim.SetUp(639, 0, 342, 310, 4, 14, "0,1,2,3,4,5,6,7,8,9,10,11,10,11,10,11,10,11,10,11,10,11,10,11,12,13");
	bombexplosion.anim.loop = false;
	bombexplosion.anim.speed = 0.15f;
	bombexplosion.life = 2888;
	bombexplosion.speed = { 0,0 };
	bombexplosion.fx = App->audio->LoadSFX("sfx/bomb_explosion.wav");

	laserattbig.anim.SetUp(100, 124, 10, 31, 3, 3, "0,1,2");
	laserattbig.anim.loop = true;
	laserattbig.speed = { 0, -14 };
	laserattbig.life = 1500;

	bigasslaser.anim.SetUp(130, 89, 37, 33, 1, 1, "0");
	bigasslaser.anim.loop = true;
	bigasslaser.speed = { 0, -14 };
	bigasslaser.life = 1500;

	player1_explosion.anim.SetUp(0, 470, 115, 101, 4, 8, "0,1,2,3,4,5,6,7");
	player1_explosion.anim.loop = false;
	player1_explosion.anim.speed = 0.25f;
	player1_explosion.life = 465;
	player1_explosion.speed = { 0,0 };
	player1_explosion.fx = App->audio->LoadSFX("sfx/player_explosion.wav");

	player2_explosion.anim.SetUp(0, 672, 115, 101, 4, 8, "0,1,2,3,4,5,6,7");
	player2_explosion.anim.loop = false;
	player2_explosion.anim.speed = 0.25f;
	player2_explosion.life = 465;
	player2_explosion.speed = { 0,0 };
	player2_explosion.fx = App->audio->LoadSFX("sfx/player_explosion2.wav");

	player1_pieces.anim.SetUp(81, 0, 10, 8, 4, 4, "0,1,2,3");
	player1_pieces.anim.speed = 0.0f;
	player1_pieces.speed = { 0,-7 };
	player1_pieces.life = 1500;

	player2_pieces.anim.SetUp(127, 0, 10, 8, 4, 4, "0,1,2,3");
	player2_pieces.anim.speed = 0.0f;
	player2_pieces.speed = { 0,-7 };
	player2_pieces.life = 1500;

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);
	autoattack.anim.CleanUp();
	laserattack.anim.CleanUp();
	laserattbig.anim.CleanUp();
	bigasslaser.anim.CleanUp();
	explosion.anim.CleanUp();
	enemyshot.anim.CleanUp();
	crater.anim.CleanUp();
	big_explosion.anim.CleanUp();
	light_explosion.anim.CleanUp();
	turret_crater.anim.CleanUp();
	bombshot.anim.CleanUp();
	bombexplosion.anim.CleanUp();
	player1_explosion.anim.CleanUp();
	player2_explosion.anim.CleanUp();
	player1_pieces.anim.CleanUp();
	player2_pieces.anim.CleanUp();
	missile.anim.CleanUp();

	App->audio->FreeSFX(player2_explosion.fx);
	App->audio->FreeSFX(player1_explosion.fx);
	App->audio->FreeSFX(bombexplosion.fx);
	App->audio->FreeSFX(bombshot.fx);
	App->audio->FreeSFX(laserattack.fx);
	App->audio->FreeSFX(autoattack.fx);
	App->audio->FreeSFX(destroy_b_air);
	
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (active[i] != nullptr)
		{
			delete active[i];
			active[i] = nullptr;
		}
	}

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* p = active[i];

		if (p == nullptr)
			continue;

		if (p->Update() == false || p->to_delete == true)
		{
			switch (p->type) {
			default:
				break;
			case BOMBSHOT:
				AddParticle(BOMB_EXPLOSION, p->position.x + p->anim.CurrentFrame().w / 2 - bombexplosion.anim.CurrentFrame().w / 2, p->position.y + p->anim.CurrentFrame().h / 2 - bombexplosion.anim.CurrentFrame().h / 2, { 999,999 }, p->collider->type == COLLIDER_PLAYER_BOMB);
				break;
			}
			App->collision->EraseCollider(p->collider);
			p->collider = nullptr;
			delete p;
			active[i] = nullptr;
		}
		else if (SDL_GetTicks() >= p->born)
		{
			iPoint view = { 0,1 };
			if (p->speed != iPoint(0, 0))
				view = p->speed;
			App->render->Blit(p->layer, graphics, p->position.x, p->position.y, view, &(p->anim.GetCurrentFrame()));
			if (p->fx_played == false)
			{
				p->fx_played = true;
				// Play particle fx here
				if (p->fx != NULL)
				{
					App->audio->PlaySFX(p->fx);
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(particle_type type, int x, int y, fPoint direction, bool player1, Uint32 delay)
{
	
	Particle* p = nullptr;
	switch (type) {
	case AUTOSHOT:
		p = new Particle(autoattack);
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, this);
		p->layer = 5;
		break;

	case LASERSHOT:
		p = new Particle(laserattack);
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, this);
		p->layer = 5;
		break;

	case LASERBIGSHOT:
		p = new Particle(laserattbig);
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, this);
		p->layer = 5;
		break;

	case BIGASSLASER:
		p = new Particle(bigasslaser);
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, this);
		p->layer = 5;
		break;

	case MISSILE:
		p = new ACParticle(missile);
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, this);
		p->layer = 5;
		break;

	case EXPLOSION:
		App->input->ShakeController(1, 500, 0.1f);
		App->input->ShakeController(2, 500, 0.1f);
		p = new Particle(explosion);
		p->layer = 5;
		break;

	case ENEMYSHOT:
		p = new Particle(enemyshot);
		p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_ENEMY_SHOT, this);
		p->layer = 5;
		break;

	case CRATER:
		p = new Particle(crater);
		p->layer = 2;
		break;

	case BIG_EXPLOSION:
		App->input->ShakeController(1, 500, 0.3f);
		App->input->ShakeController(2, 500, 0.3f);
		p = new Particle(big_explosion);
		p->layer = 6;
		break;

	case LIGHT_EXPLOSION:
		App->input->ShakeController(1, 500, 0.1f);
		App->input->ShakeController(2, 500, 0.1f);
		p = new Particle(light_explosion);
		p->layer = 6;
		break;

	case TURRET_CRATER:
		p = new Particle(turret_crater);
		p->layer = 2;
		break;
		
	case BOMBSHOT:
		p = new Particle(bombshot);
		p->layer = 6;
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, this);
		break;

	case BOMB_EXPLOSION:
		p = new Particle(bombexplosion);
		p->layer = 6;
		if (player1)
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_BOMB, this);
		else
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_BOMB, this);
		break;

	case PLAYER_EXPLOSION:
		if (player1) {
			p = new Particle(player1_explosion);
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, nullptr);
		}
		else {
			p = new Particle(player2_explosion);
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, nullptr);
		}
		p->layer = 6;
		break;

	case PLAYER_BITS:
		if (player1) {
			p = new Particle(player1_pieces);
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER_SHOT, nullptr);
		}
		else {
			p = new Particle(player2_pieces);
			p->collider = App->collision->AddCollider(p->anim.CurrentFrame(), COLLIDER_TYPE::COLLIDER_PLAYER2_SHOT, nullptr);
		}
		p->layer = 5;
		srand(SDL_GetTicks());
		p->anim.SetFrame(rand() % 4);
		break;
	}

	if (direction.x != 999 && direction.y != 999) {
		direction.Normalize();
		direction = direction * p->speed.Length();
		p->speed.x = direction.x;
		p->speed.y = direction.y;
	}

	p->type = type;
	p->born = SDL_GetTicks() + delay;
	p->position.x = x;
	p->position.y = y;

	bool found = false;

	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i) 		{
		if (active[i] == nullptr) 			{
			found = true;
			active[i] = (Particle*)p;
			break;
		}
	}

	if (!found) {
		LOG("Overwriting old particles");

		if (active[last_particle] != nullptr) {
			Particle* temp = active[last_particle];
			App->collision->EraseCollider(temp->collider);
			temp->collider = nullptr;
			delete temp;
			active[last_particle] = nullptr;
		}

		active[last_particle++] = p;
		if (last_particle >= MAX_ACTIVE_PARTICLES)
			last_particle = 0;		
	}
}

// -------------------------------------------------------------
void ModuleParticles::OnCollision(Collider* c1, Collider* c2) {
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
		if (active[i] != nullptr && active[i]->collider == c1) {
			active[i]->to_delete = true;
			break;
		}
	}
}
// -------------------------------------------------------------

Particle::Particle()
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) :
	anim(p.anim), position(p.position), speed(p.speed),
	fx(p.fx), born(p.born), life(p.life)
{}

bool Particle::Update()
{
	bool ret = true;
	if (born <= SDL_GetTicks()) {
		if (life > 0) {
			if ((SDL_GetTicks() - born) > life)
				ret = false;
		}
		else
			if (anim.Finished()) {
				ret = false;
				to_delete = true;
			}

		position.x += speed.x;
		position.y += speed.y;

		App->collision->SetPosition(collider, position.x, position.y);
	}

	return ret;
}

bool ACParticle::Update()
{
	bool ret = true;

	if (sdl_acc == 0)
		sdl_acc = born + 10;

	if (born <= SDL_GetTicks()) {
		if (life > 0) {
			if ((SDL_GetTicks() - born) > life)
				ret = false;
		}
		else
			if (anim.Finished()) {
				ret = false;
				to_delete = true;
			}

		position.x += speed.x;
		position.y += speed.y;

		if (sdl_acc <= SDL_GetTicks())
		{
			++iterations;
			speed = speed + acceleration;
			acceleration = acceleration * (1 + iterations / 10);
			sdl_acc = SDL_GetTicks() + 100;
		}

		App->collision->SetPosition(collider, position.x, position.y);
	}

	return ret;
}
