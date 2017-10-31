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
	PhysBody *Circle_Body;
	b2Body *BarrBody;
	//Barra inic
	b2PrismaticJoint* barraInici;

	bool start = true;
private:
	float yBarraInicial = 0;
	// Aquests vectors son per la barra

};