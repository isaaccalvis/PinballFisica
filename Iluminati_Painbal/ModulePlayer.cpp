#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "p2Point.h"
#include "math.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	Circle_Body = App->physics->CreateCircle(530, 600, 8, b2_dynamicBody);

	PhysBody* actionBarra = App->physics->CreateRectangle(530, 750, 10, 10, b2_dynamicBody);
	PhysBody* baseBarra = App->physics->CreateRectangle(530, 780, 10, 10, b2_staticBody);

	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.Initialize(actionBarra->body, baseBarra->body, baseBarra->body->GetWorldCenter(), b2Vec2(0, 1));
	prismaticJointDef.collideConnected = true;
	prismaticJointDef.lowerTranslation = 0;
	prismaticJointDef.upperTranslation = 1.5f;
	prismaticJointDef.enableLimit = true;
	prismaticJointDef.maxMotorForce = 10;
	prismaticJointDef.motorSpeed = 25.0;
	prismaticJointDef.enableMotor = true;

	barraInici = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJointDef);

	//~Creacio Barra Bona

	Circle_Texture = App->textures->Load("pinball/ball.png");
	Circle_Rect = { 0,0,18,18 };
	LOG("Loading player");
	return true;
}

// Update: draw background
// TXELL SEXY
update_status ModulePlayer::Update()
{
	int x, y;
	Circle_Body->GetPosition(x, y);
	App->renderer->Blit(Circle_Texture, METERS_TO_PIXELS(x), METERS_TO_PIXELS(y), &Circle_Rect);
	b2Vec2 vecGoDownBarr(0, -0.01f);
	b2Vec2 vecGoUpBarr(0, 0.2f);
	b2Vec2 vecGoStartBarr(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && start) {
		start = false;
	}

	b2Vec2 moviment(0, yBarraInicial);
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		yBarraInicial += 0.00001f;
		barraInici->GetBodyA()->ApplyLinearImpulse(moviment, {0,0}, 0);
		barraInici->EnableMotor(false);
	}
	else
		if (yBarraInicial != 0) {
			barraInici->EnableMotor(true);
			yBarraInicial = 0.0f;
		}

	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}