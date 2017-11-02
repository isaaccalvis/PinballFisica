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
	void NewBall(int x, int y);
	void Stickers();

public:
	PhysBody* Quad_Started;
	PhysBody* Quad_Bridge;

	PhysBody* Fliper_R;
	PhysBody* Fliper_L;
	b2Body* body_FliperR;
	b2Body* body_FliperL;
	b2RevoluteJoint* left_joint;
	b2RevoluteJoint* right_joint;

	PhysBody *Circle_Body;
	int live = 3;
	bool die = false;
	bool started = false;
	bool barrier = true;
	
	
	bool Sens_L = false;
	bool Sens_R = false;
	uint currentTime, lastTime, buildTime;

	int Bridge;
	enum BrirgeWall
	{
		Wall,
		Destroy,
		Idle,	
		Rebuild,
	};

	int Score;

	//Barra inic
	b2PrismaticJoint* barraInici;
	SDL_Texture* BarraInici_Texture;
	SDL_Texture* pilota_Texture;

	float left_rotation = 0;
	float right_rotation = 0;
private:
	// Aquests vectors son per la barra
	float yBarraInicial = 0;

};