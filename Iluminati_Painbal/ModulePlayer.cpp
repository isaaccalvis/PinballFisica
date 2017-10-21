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
	x = 2;
	y = 2;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	// Creacio Pilota
	b2BodyDef baseCircle;
	baseCircle.type = b2_dynamicBody;
	baseCircle.position.Set(x, y);
	b2CircleShape shapeCircle;
	shapeCircle.m_radius = PIXEL_TO_METERS(9);
	b2Body *body = App->physics->world->CreateBody(&baseCircle);
	Circle_Body = body;
	b2FixtureDef fixture;
	fixture.shape = &shapeCircle;
	fixture.density = 1;
	body->CreateFixture(&fixture);
	// ~Creacio Pilota

	Circle_Texture = App->textures->Load("pinball/ball.png");
	Circle_Rect = { 0,0,18,18 };
	LOG("Loading player");
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	App->renderer->Blit(Circle_Texture, METERS_TO_PIXELS(Circle_Body->GetPosition().x) - 9, METERS_TO_PIXELS(Circle_Body->GetPosition().y) - 9, &Circle_Rect);
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}