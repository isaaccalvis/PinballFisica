#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Box2D\Box2D\Box2D.h"

enum BrirgeWall
{
	Wall,
	Destroy,
	Idle,
	Rebuild,
};

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
	PhysBody *Circle_Body;

	b2Body* body_FliperR;
	b2Body* body_FliperL;

	b2RevoluteJoint* left_joint;
	b2RevoluteJoint* right_joint;
	b2PrismaticJoint* barraInici;

	SDL_Texture* BarraInici_Texture;
	SDL_Texture* pilota_Texture;

	SDL_Rect pilotaRect = { 0,0,19,19 };

	int live = 3;
	int Score = 0;
	bool die = false;
	bool started = false;
	bool barrier = true;
	
	bool Sens_L = false;
	bool Sens_R = false;
	int Bridge;
	int Ball_A, Bonus_A, Bridge_A, Tub_A, Triangle_A, QuadPiramide_A, Sticker_A, Died_A;

	uint currentTime, lastTime, buildTime;

	float left_rotation = 0;
	float right_rotation = 0;
	
private:

	float yBarraInicial = 0;

};