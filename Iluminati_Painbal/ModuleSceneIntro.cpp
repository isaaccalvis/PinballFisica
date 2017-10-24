#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
	r.x = 0;
	r.y = 0;
	r.w = SCREEN_WIDTH;
	r.h = SCREEN_HEIGHT;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	fons = App->textures->Load("pinball/BG.png");


	// Pivot -1, -1
	int bg[100] = {
		129, 777,
		18, 711,
		17, 589,
		83, 519,
		85, 480,
		67, 445,
		52, 411,
		36, 361,
		29, 316,
		25, 270,
		28, 215,
		45, 160,
		72, 118,
		96, 89,
		131, 60,
		167, 38,
		208, 21,
		230, 16,
		260, 13,
		282, 12,
		301, 12,
		329, 13,
		359, 17,
		394, 27,
		436, 46,
		472, 72,
		502, 100,
		520, 130,
		535, 168,
		541, 202,
		544, 801,
		514, 801,
		512, 429,
		509, 423,
		501, 423,
		492, 434,
		473, 451,
		447, 465,
		420, 474,
		420, 517,
		485, 588,
		485, 707,
		377, 774,
		377, 855,
		357, 984,
		318, 1155,
		187, 1436,
		77, 1233,
		105, 916,
		115, 814
	};
	int placE[16] = {
		91, 590,
		91, 660,
		138, 690,
		151, 689,
		153, 682,
		106, 585,
		102, 580,
		92, 582
	};
	int placD[18] = {
		343, 682,
		386, 591,
		392, 583,
		398, 581,
		405, 584,
		405, 661,
		357, 692,
		349, 693,
		343, 690
	};
	int walle[58] = {
		138, 307,
		118, 272,
		104, 231,
		101, 192,
		107, 154,
		124, 116,
		147, 84,
		173, 67,
		206, 53,
		226, 47,
		250, 44,
		260, 50,
		274, 64,
		275, 111,
		297, 152,
		296, 228,
		192, 296,
		177, 280,
		168, 263,
		166, 245,
		172, 234,
		183, 228,
		186, 214,
		182, 205,
		172, 201,
		157, 203,
		144, 216,
		141, 230,
		138, 261
	};
	int walld[42] = {
		426, 374,
		434, 377,
		477, 334,
		480, 325,
		471, 301,
		445, 274,
		404, 237,
		389, 224,
		376, 214,
		344, 211,
		322, 300,
		323, 307,
		327, 309,
		332, 308,
		341, 303,
		352, 303,
		364, 307,
		371, 318,
		373, 331,
		368, 344,
		374, 347
	};
	int wallu[42] = {
		410, 117,
		410, 165,
		411, 178,
		432, 201,
		464, 233,
		486, 257,
		500, 277,
		505, 256,
		507, 240,
		508, 212,
		506, 183,
		499, 157,
		489, 134,
		470, 109,
		453, 93,
		436, 80,
		426, 73,
		418, 70,
		412, 73,
		410, 81,
		410, 99
	};
	int palD[20] = {
		279, 792,
		337, 747,
		347, 745,
		354, 752,
		353, 764,
		337, 773,
		313, 788,
		295, 797,
		285, 803,
		278, 800
	};
	int palE[24] = {
		174, 761,
		215, 792,
		219, 798,
		217, 804,
		210, 804,
		203, 800,
		146, 766,
		142, 759,
		143, 752,
		148, 749,
		155, 749,
		164, 754
	};



	
	BG = App->physics->CreateChain(0, 0, bg, 100, b2_staticBody);
	placaE = App->physics->CreateChain(0, 0, placE, 16, b2_staticBody);
	placaD = App->physics->CreateChain(0, 0, placD, 18, b2_staticBody);
	wallE = App->physics->CreateChain(0, 0, walle, 58, b2_staticBody);
	wallD = App->physics->CreateChain(0, 0, walld, 42, b2_staticBody);
	wallU = App->physics->CreateChain(0, 0, wallu, 42, b2_staticBody);
	palancaD = App->physics->CreateChain(0, 0, palD, 20, b2_staticBody);
	palancaE = App->physics->CreateChain(0, 0, palE, 24, b2_staticBody);


	BallG = App->physics->CreateCircle(357, 110, 24, b2_staticBody);
	BallU = App->physics->CreateCircle(273, 69, 24, b2_staticBody);
	BallD = App->physics->CreateCircle(406, 172, 24, b2_staticBody);
	BallE = App->physics->CreateCircle(299, 161, 24, b2_staticBody);
	BallB = App->physics->CreateCircle(351, 207, 24, b2_staticBody);

	barraL = App->physics->CreateRectangle(51, 645, 9, 77, b2_staticBody, 0);
	barraR = App->physics->CreateRectangle(447, 645, 9, 77, b2_staticBody, 0);

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, SCREEN_HEIGHT + SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(fons);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	/*if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		App->renderer->camera.y -= 4;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->renderer->camera.y += 4;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->renderer->camera.x += 4;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->renderer->camera.x -= 4;*/


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}



	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}


	App->renderer->Blit(fons, 0, 0, &r);
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	//if (true)
	{

	}



	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
