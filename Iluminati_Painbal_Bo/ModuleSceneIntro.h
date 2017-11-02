#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class TextureNoCollition 
{
public:
	TextureNoCollition() {}
	~TextureNoCollition() {}

	SDL_Texture* textura;
	SDL_Rect texturaRect;
	int x, y;
	uint temporitzador = 0;
};

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
	p2List<PhysBody*> physList;

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
	PhysBody* diag_barraR;
	PhysBody* diag_barraL;
	PhysBody* Bridge;

	PhysBody* sensor;
	PhysBody* Triangle_sens;
	PhysBody* Brindge_sens;
	PhysBody* L_Ball_sens;
	PhysBody* R_Ball_sens;
	PhysBody* Start_sens;
	PhysBody* Tub_sens;
	PhysBody* Tube_sens;


	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* fons;
	SDL_Texture* kicker_R;
	SDL_Texture* kicker_L;
	SDL_Rect r;
	SDL_Rect tube_rect;
	SDL_Texture* circle_Reboting_Ilumination;
	SDL_Texture* tube;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	TextureNoCollition circleTextures_WithouColl[5];
};
