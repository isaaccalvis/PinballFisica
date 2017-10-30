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
	Circle_Body = App->physics->CreateCircle(530, 1600, 8, b2_dynamicBody);

	//Creacio Barra
	//b2BodyDef baseBarr;
	//baseBarr.type = b2_staticBody;
	//baseBarr.position.Set(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));
	//b2PolygonShape shapePolygon;
	//shapePolygon.SetAsBox(PIXEL_TO_METERS(10), PIXEL_TO_METERS(30), b2Vec2(0, 0), 0);
	//b2Body *bodyBarr = App->physics->world->CreateBody(&baseBarr);
	//BarrBody = bodyBarr;
	//b2FixtureDef fixtureBarr;
	//fixtureBarr.friction = 0.3f;
	//fixtureBarr.density = 50;
	//fixtureBarr.shape = &shapePolygon;
	//bodyBarr->CreateFixture(&fixtureBarr);
	//~CreacioBarra

	//Creacio Barra Bona
	PhysBody* actionBarra = App->physics->CreateRectangle(530, 750, 10, 10);
	PhysBody* baseBarra = App->physics->CreateRectangle(530, 780, 10, 10, b2_staticBody);

	b2DistanceJointDef distanceJointDef;
	distanceJointDef.Initialize(actionBarra->body, baseBarra->body, actionBarra->body->GetWorldCenter(), baseBarra->body->GetWorldCenter());
	distanceJointDef.collideConnected = true;
	
	distanceJointDef.dampingRatio = 0.5f;
	distanceJointDef.frequencyHz = 4.0f;
	barraInici = (b2DistanceJoint*)App->physics->world->CreateJoint(&distanceJointDef);

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
	//printf_s("%f\n", BarrBody->GetPosition().y);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && start) {
		start = false;
		Circle_Body->body->ApplyLinearImpulse({0, -5}, {0,0}, true);
		//Line join
	}

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		yBarraInicial += 0.1f;
	//	if (BarrBody->GetPosition().y < 16.3f)
	//		BarrBody->SetTransform(BarrBody->GetPosition() - vecGoDownBarr, 0);
	}
	else if (BarrBody->GetPosition().y > PIXEL_TO_METERS(770)) {
		yBarraInicial -= 0.3f;
	//	BarrBody->SetTransform(BarrBody->GetPosition() - vecGoUpBarr, 0);
	}
	else {
	//	BarrBody->SetTransform(vecGoStartBarr, 0);
	}
	b2Vec2 moviment(0, yBarraInicial);
	barraInici->GetBodyA()->ApplyForce(moviment, { 0, 0 }, true);
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}