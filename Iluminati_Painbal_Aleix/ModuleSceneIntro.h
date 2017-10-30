#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* BG;
	PhysBody* placaE;
	PhysBody* placaD;
	PhysBody* wallL;
	PhysBody* wallR;
	PhysBody* wallM;
	PhysBody* wallU;
	PhysBody* BallG;
	PhysBody* BallU;
	PhysBody* BallD;
	PhysBody* BallE;
	PhysBody* BallB;
	PhysBody* palancaD;
	PhysBody* palancaE;
	PhysBody* barraL;
	PhysBody* barraR;
	PhysBody* sensor;
	PhysBody* diag_barraR;
	PhysBody* diag_barraL;
	PhysBody* Bridge;

	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* fons;
	SDL_Texture* kicker_R;
	SDL_Texture* kicker_L;
	SDL_Rect r;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
