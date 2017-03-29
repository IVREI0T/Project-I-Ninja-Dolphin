#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleStage6.h"
#include "ModuleStage2.h"
#include "ModuleFadeToBlack.h"

Application::Application()
{
	uint i = 0;
	modules[i++] = window = new ModuleWindow();
	modules[i++] = render = new ModuleRender();
	modules[i++] = input = new ModuleInput();
	modules[i++] = textures = new ModuleTextures();
	modules[i++] = audio = new ModuleAudio();
	modules[i++] = stage2 = new ModuleStage2();
	modules[i++] = stage6 = new ModuleStage6();
	modules[i++] = fade = new ModuleFadeToBlack();
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
	stage2->Disable();
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