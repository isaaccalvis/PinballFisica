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
	// Creacio Pilota
	b2BodyDef baseCircle;
	baseCircle.type = b2_dynamicBody;
	baseCircle.position.Set(PIXEL_TO_METERS(530), PIXEL_TO_METERS(600));
	b2CircleShape shapeCircle;
	shapeCircle.m_radius = PIXEL_TO_METERS(9);
	b2Body *body = App->physics->world->CreateBody(&baseCircle);
	Circle_Body = body;
	b2FixtureDef fixture;
	fixture.shape = &shapeCircle;
	fixture.density = 1;
	body->CreateFixture(&fixture);
	// ~Creacio Pilota

	//Creacio Barra
	b2BodyDef baseBarr;
	baseBarr.type = b2_staticBody;
	baseBarr.position.Set(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));
	b2PolygonShape shapePolygon;
	shapePolygon.SetAsBox(PIXEL_TO_METERS(10), PIXEL_TO_METERS(30), b2Vec2(0, 0), 0);
	b2Body *bodyBarr = App->physics->world->CreateBody(&baseBarr);
	BarrBody = bodyBarr;
	b2FixtureDef fixtureBarr;
	fixtureBarr.shape = &shapePolygon;
	fixtureBarr.density = 1;
	bodyBarr->CreateFixture(&fixtureBarr);
	//~CreacioBarra

	Circle_Texture = App->textures->Load("pinball/ball.png");
	Circle_Rect = { 0,0,18,18 };
	LOG("Loading player");
	return true;
}

// Update: draw background
// TXELL SEXY
update_status ModulePlayer::Update()
{
	App->renderer->Blit(Circle_Texture, METERS_TO_PIXELS(Circle_Body->GetPosition().x) - 9, METERS_TO_PIXELS(Circle_Body->GetPosition().y) - 9, &Circle_Rect);
	b2Vec2 vecGoDownBarr(0, -0.01f);
	b2Vec2 vecGoUpBarr(0, 0.5f);
	b2Vec2 vecGoStartBarr(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));
	printf_s("%f\n", BarrBody->GetPosition().y);
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		if (BarrBody->GetPosition().y < 16.3f)
			BarrBody->SetTransform(BarrBody->GetPosition() - vecGoDownBarr, 0);
	}
	else if (BarrBody->GetPosition().y > PIXEL_TO_METERS(770)) {
		BarrBody->SetTransform(BarrBody->GetPosition() - vecGoUpBarr, 0);
	}
	else {
		BarrBody->SetTransform(vecGoStartBarr, 0);
	}
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}