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
	circle = NULL;
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

	tube_rect.x = 0;
	tube_rect.y = 0;
	tube_rect.h = 125;
	tube_rect.w = 81;

	circle = App->textures->Load("pinball/wheel.png"); 
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	circle_Reboting_Ilumination = App->textures->Load("pinball/aroPivotsPunts.png");

	// CREAR LES TEXTURES DEL REBOTES CIRCULARS SENSE COLISIO
	for (int i = 0; i < 5; i++) {
		circleTextures_WithouColl[i].textura = circle_Reboting_Ilumination;
		circleTextures_WithouColl[i].texturaRect = { 0,0,60,60 };
	}
	circleTextures_WithouColl[0].x = 242;
	circleTextures_WithouColl[0].y = 39;
	circleTextures_WithouColl[1].x = 378;
	circleTextures_WithouColl[1].y = 141;
	circleTextures_WithouColl[2].x = 271;
	circleTextures_WithouColl[2].y = 130;
	circleTextures_WithouColl[3].x = 323;
	circleTextures_WithouColl[3].y = 176;
	circleTextures_WithouColl[4].x = 329;
	circleTextures_WithouColl[4].y = 79;

	fons = App->textures->Load("pinball/BG.png");
	tube = App->textures->Load("pinball/pon.png");
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
	L_Ball_sens = App->physics->CreateRectangleSensor(168, 219, 7, 7);
	R_Ball_sens = App->physics->CreateRectangleSensor(353, 319, 7, 7);
	Brindge_sens = App->physics->CreateRectangleSensor(85, 170, 28, 5);	
	Tub_sens = App->physics->CreateRectangleSensor(407, 335, 28, 5);
	Tube_sens = App->physics->CreateRectangleSensor(445, 239, 5, 10);

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH/2, SCREEN_HEIGHT + SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT);



	return ret;
}

bool ModuleSceneIntro::CleanUp()
{
	App->textures->Unload(fons);
	App->textures->Unload(kicker_L);
	App->textures->Unload(kicker_R);
	App->textures->Unload(circle);
	App->textures->Unload(tube);
	App->textures->Unload(circle_Reboting_Ilumination);
	return true;
}

update_status ModuleSceneIntro::Update()
{

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->player->NewBall(App->input->GetMouseX(), App->input->GetMouseY());

	App->renderer->Blit(fons, 0, 0, &r);
	App->renderer->Blit(kicker_L, 82, 700, NULL, 1.0f, App->player->left_rotation);
	App->renderer->Blit(kicker_R, 276, 700, NULL, 1.0f, App->player->right_rotation);

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	p2List_item<PhysBody*>* recList = physList.getFirst();

	while (recList != NULL)
	{
		if (recList->data->textureActual != nullptr) {
			int x, y;
			recList->data->GetPosition(x, y);
			SDL_Rect re{ 0, 0, 400, 400 };
			App->renderer->Blit(recList->data->textureActual, x + 1, y, &recList->data->textureRect, 1.0f);
		}
		recList = recList->next;
	}

	App->renderer->Blit(tube, 370, 250, &tube_rect, 1.0f);

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

	for (int i = 0; i < 5; i++) {
		if (circleTextures_WithouColl[i].temporitzador > SDL_GetTicks())
			App->renderer->Blit(circleTextures_WithouColl[i].textura, circleTextures_WithouColl[i].x, circleTextures_WithouColl[i].y, &circleTextures_WithouColl[i].texturaRect);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
}
