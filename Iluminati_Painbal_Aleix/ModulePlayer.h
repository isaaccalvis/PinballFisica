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
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void NewBall();
	void Stickers();


	SDL_Texture* Circle_Texture;
	SDL_Rect Circle_Rect;

public:

	PhysBody* Fliper_R;
	PhysBody* Fliper_L;
	b2Body* body_FliperR;
	b2Body* body_FliperL;
	b2RevoluteJoint* left_joint;
	b2RevoluteJoint* right_joint;

	PhysBody *Circle_Body;
	b2Body *BarrBody;
	bool start = true;
	int live = 3;
	bool die = false;

	float left_rotation = 0;
	float right_rotation = 0;
private:
	// Aquests vectors son per la barra

};