#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

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

ModuleSceneIntro::~ModuleSceneIntro() {}

bool ModuleSceneIntro::Start()
{
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	circles_Texture_Type1Col = App->textures->Load("pinball/Barra.png");

	fons = App->textures->Load("pinball/BG.png");
	kicker_L = App->textures->Load("pinball/kicker_L.png");
	kicker_R = App->textures->Load("pinball/kicker_R.png");

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
	int plac_L[16] = {
		91, 590,
		91, 660,
		138, 690,
		151, 689,
		153, 682,
		106, 585,
		102, 580,
		92, 582
	};
	int plac_R[18] = {
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
	int wall_L[58] = {
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
	int wall_R[10] = {
		424, 371,
		435, 376,
		481, 329,
		475, 306,
		452, 276
	};
	int WallMiddle[26] = {
		340, 225,
		322, 302,
		327, 310,
		337, 303,
		355, 303,
		367, 311,
		373, 323,
		372, 337,
		375, 345,
		396, 304,
		425, 243,
		401, 226,
		365, 214
	};
	int wall_U[42] = {
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
	int brindge[80] = {
		236, 16,
		205, 22,
		179, 31,
		155, 41,
		130, 56,
		116, 71,
		99, 90,
		86, 103,
		76, 118,
		66, 135,
		58, 152,
		52, 171,
		49, 192,
		50, 220,
		50, 239,
		53, 260,
		58, 278,
		61, 290,
		66, 310,
		72, 325,
		86, 359,
		91, 348,
		78, 317,
		66, 282,
		59, 254,
		56, 223,
		57, 200,
		60, 176,
		66, 156,
		72, 140,
		81, 123,
		93, 108,
		104, 92,
		117, 79,
		132, 61,
		156, 45,
		179, 34,
		205, 25,
		224, 20,
		237, 18
	};

	BG = App->physics->CreateChain(0, 0, bg, 100, b2_staticBody, 0.1f);
	placaE = App->physics->CreateChain(0, 0, plac_L, 16, b2_staticBody, 2);
	placaD = App->physics->CreateChain(0, 0, plac_R, 18, b2_staticBody, 2);
	wallL = App->physics->CreateChain(0, 0, wall_L, 58, b2_staticBody, 0.1f);
	wallR = App->physics->CreateChain(0, 0, wall_R, 10, b2_staticBody, 0.1f);
	wallU = App->physics->CreateChain(0, 0, wall_U, 42, b2_staticBody, 0.1f);
	wallM = App->physics->CreateChain(0, 0, WallMiddle, 26, b2_staticBody, 0.1f);
	Bridge = App->physics->CreateChain(0, 0, brindge, 80, b2_staticBody, 0.1f);


	BallG = App->physics->CreateCircle(357, 110, 24, b2_staticBody, 1);
	BallU = App->physics->CreateCircle(273, 69, 24, b2_staticBody, 1);
	BallD = App->physics->CreateCircle(406, 172, 24, b2_staticBody, 1);
	BallE = App->physics->CreateCircle(299, 161, 24, b2_staticBody, 1);
	BallB = App->physics->CreateCircle(351, 207, 24, b2_staticBody, 1);

	barraL = App->physics->CreateRectangle(51, 645, 9, 77, b2_staticBody, 0);
	barraR = App->physics->CreateRectangle(447, 645, 9, 77, b2_staticBody, 0);
	diag_barraR = App->physics->CreateRectangle(390, 714, 9, 125, b2_staticBody, 1);
	diag_barraL = App->physics->CreateRectangle(100, 714, 9, 125, b2_staticBody, -1);

	Triangle_sens = App->physics->CreateRectangle(51, 343, 26, 20, b2_staticBody, 0);
	Start_sens = App->physics->CreateRectangleSensor(525, 400, 20, 5);
	L_Ball_sens = App->physics->CreateRectangleSensor(183, 220, 5, 15);
	R_Ball_sens = App->physics->CreateRectangleSensor(353, 319, 15, 15);
	Brindge_sens = App->physics->CreateRectangleSensor(85, 170, 28, 5);
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, SCREEN_HEIGHT + SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->textures->Unload(fons);
	App->textures->Unload(kicker_L);
	App->textures->Unload(kicker_R);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		App->renderer->camera.y -= 4;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->renderer->camera.y += 4;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->renderer->camera.x += 4;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->renderer->camera.x -= 4;
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->player->NewBall(App->input->GetMouseX(), App->input->GetMouseY());
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	// PRINTAR EL MAPA
	App->renderer->Blit(fons, 0, 0, &r);
	App->renderer->Blit(kicker_L, 82, 700, NULL, 1.0f, App->player->left_rotation);
	App->renderer->Blit(kicker_R, 276, 700, NULL, 1.0f, App->player->right_rotation);

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* recList = physList.getFirst();

	while (recList != NULL)
	{
		if (recList->data->texturaActual != nullptr) {
			int x, y;
			recList->data->GetPosition(x, y);
			printf_s("%i \n", recList->data->texturaRect);
			SDL_Rect re{ 0, 0, 400, 400 };
			App->renderer->Blit(recList->data->texturaActual, x + 1, y, &recList->data->texturaRect);
		}
		recList = recList->next;
	}

	//p2List_item<PhysBody*>* c = circles.getFirst();
	//while(c != NULL)
	//{
	//	if (c->data->texturaActual != nullptr) {
	//		App->renderer->Blit(c->data->texturaActual, c->data->body->GetPosition().x, c->data->body->GetPosition().y, &c->data->texturaRect);
	//	}
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
	//		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

	//c = boxes.getFirst();
	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
	//	if(ray_on)
	//	{
	//		int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
	//		if(hit >= 0)
	//			ray_hit = hit;
	//	}
	//	c = c->next;
	//}

	//c = ricks.getFirst();
	//while(c != NULL)
	//{
	//	int x, y;
	//	c->data->GetPosition(x, y);
	//	App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
	//	c = c->next;
	//}

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
