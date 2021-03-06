#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "ModuleStageIntro.h"
#include "ModuleStage1.h"
#include "ModuleCollision.h"
#include "ModulePlayer.h"
#include "ModuleTransition.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include "ModuleBonus.h"

Application::Application()
{
	uint i = 0;
	modules[i++] = window = new ModuleWindow();
	modules[i++] = render = new ModuleRender();
	modules[i++] = input = new ModuleInput();
	modules[i++] = textures = new ModuleTextures();
	modules[i++] = audio = new ModuleAudio();
	modules[i++] = fonts = new ModuleFonts();
	modules[i++] = stage1 = new ModuleStage1();
	modules[i++] = intro = new ModuleStageIntro();
	modules[i++] = enemies = new ModuleEnemies();
	modules[i++] = bonus = new ModuleBonus();
	modules[i++] = particles = new ModuleParticles();
	modules[i++] = collision = new ModuleCollision();
	modules[i++] = player = new ModulePlayer();
	modules[i++] = transition = new ModuleTransition();
}	

Application::~Application()
{
	for(int i = 0; i < NUM_MODULES; ++i)
		delete modules[i];
}

bool Application::Init()
{
	bool ret = true;

	//Disabling stopped modules
	stage1->Disable();
	player->Disable();
	bonus->Disable();
	//-

	for(int i = 0; i < NUM_MODULES && ret == true; ++i)
		ret = modules[i]->Init();

	for (int i = 0; i < NUM_MODULES && ret == true; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i) 
		ret = modules[i]->IsEnabled() ? ret = modules[i]->PreUpdate() : UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i) 
		ret = modules[i]->IsEnabled() ? ret = modules[i]->Update() : UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? ret = modules[i]->PostUpdate() : UPDATE_CONTINUE;


	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(int i = NUM_MODULES - 1; i >= 0 && ret == true; --i)
		ret = modules[i]->CleanUp();

	return ret;
}