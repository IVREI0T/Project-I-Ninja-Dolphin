#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Globals.h"
#include "SDL\include\SDL.h"

#define PLAYER_SPEED 3;
struct SDL_Texture;

enum player_state {
	idle = 0,
	left,
	right,
	forward,
	stop

};

class ModulePlayer : public Module {
public:

	ModulePlayer();
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	SDL_Texture* player;
	SDL_Rect player_sprite;
	SDL_Rect player_sprite_left;
	SDL_Rect player_sprite_right;
	int player_x, player_y;
	player_state state;
};

#endif