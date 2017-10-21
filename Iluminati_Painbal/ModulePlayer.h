#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Box2D\Box2D\Box2D.h"

class ModulePlayer : public Module
{
public:

	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	SDL_Texture* Circle_Texture;
	SDL_Rect Circle_Rect;

public:
	float x, y;
	b2Body *Circle_Body;
};