#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "p2Point.h"
#include "math.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	Circle_Texture = App->textures->Load("pinball/ball.png");
	BarraInici_Texture = App->textures->Load("pinball/Barra.png");
	//////////////////////////////////////////////////////////////////////////////


	Stickers();
	NewBall();


	//Creacio Barra	
	SDL_Rect rectBarraAux = { 0,0,10, 37 };
	PhysBody* actionBarra = App->physics->CreateRectangle(529, 750, 25, 10, b2_dynamicBody,0, BarraInici_Texture, rectBarraAux);
	PhysBody* baseBarra = App->physics->CreateRectangle(530, 780, 10, 10, b2_staticBody);

	b2PrismaticJointDef prismaticJointDef;
	prismaticJointDef.Initialize(actionBarra->body, baseBarra->body, baseBarra->body->GetWorldCenter(), b2Vec2(0, 1));
	prismaticJointDef.collideConnected = true;
	prismaticJointDef.lowerTranslation = 0;
	prismaticJointDef.upperTranslation = 1.5f;
	prismaticJointDef.enableLimit = true;
	prismaticJointDef.maxMotorForce = 28;
	prismaticJointDef.motorSpeed = 38.0;
	prismaticJointDef.enableMotor = true;

	barraInici = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJointDef);
	//~CreacioBarra


	Circle_Rect = { 0,0,18,18 };
	LOG("Loading player");
	return true;
}

// Update: draw background
// TXELL SEXY
update_status ModulePlayer::Update()
{

	left_rotation = RADTODEG * left_joint->GetJointAngle();
	right_rotation = RADTODEG * right_joint->GetJointAngle();

	left_joint->SetMotorSpeed(15);
	right_joint->SetMotorSpeed(-15);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		left_joint->EnableMotor(true);
		left_joint->SetMaxMotorTorque(700);
		left_joint->SetMotorSpeed(-15);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		right_joint->EnableMotor(true);
		right_joint->SetMaxMotorTorque(700);
		right_joint->SetMotorSpeed(15);
	}

	// MOVIMENT BARRA INICIAL
	b2Vec2 moviment(0, yBarraInicial);
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		yBarraInicial += 0.00001f;
		barraInici->GetBodyA()->ApplyLinearImpulse(moviment, { 0,0 }, 0);
		barraInici->EnableMotor(false);
	}
	else
		if (yBarraInicial != 0) {
			barraInici->EnableMotor(true);
			yBarraInicial = 0.0f;
		}
	////////////////////////////
	int x, y;
	Circle_Body->GetPosition(x, y);
	App->renderer->Blit(Circle_Texture, METERS_TO_PIXELS(x), METERS_TO_PIXELS(y), &Circle_Rect);
	b2Vec2 vecGoDownBarr(0, -0.01f);
	b2Vec2 vecGoUpBarr(0, 0.2f);
	b2Vec2 vecGoStartBarr(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && start) {
		start = false;
		Circle_Body->body->ApplyLinearImpulse({0, -3.5f}, {0,0}, true);
		//Line join
	}

	if (die) {
		die = false;
		App->physics->world->DestroyBody(Circle_Body->body);
		if (live > 0)
			NewBall();
		start = true;
		live--;

	}
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}


void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyB == App->scene_intro->sensor) {
		die = true;
		//LOG("DIE");
	}
}

void ModulePlayer::NewBall() {
	Circle_Body = App->physics->CreateCircle(530, 600, 8, b2_dynamicBody,0.1f);
	Circle_Body->listener = this;
}

void ModulePlayer::Stickers() {
	int x = 153;
	int y = 756;
	int diameter = 10;

	b2BodyDef body_l;
	body_l.type = b2_staticBody;
	body_l.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* left_ball = App->physics->world->CreateBody(&body_l);

	b2CircleShape shape_l;
	shape_l.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture_l;
	fixture_l.shape = &shape_l;
	left_ball->CreateFixture(&fixture_l);

	// Set Left Joint bodies
	body_FliperL = App->physics->world->CreateBody(&body_l);
	Fliper_L = App->physics->CreateLeftSticker();

	// Set Left Joint
	b2RevoluteJointDef joint_left_def;
	joint_left_def.bodyA = body_FliperL;
	joint_left_def.bodyB = Fliper_L->body;
	joint_left_def.collideConnected = true;
	joint_left_def.localAnchorA.Set(PIXEL_TO_METERS(0), 0);
	joint_left_def.localAnchorB.Set(PIXEL_TO_METERS(-40), PIXEL_TO_METERS(-2));
	joint_left_def.referenceAngle = 30 * DEGTORAD;
	joint_left_def.enableLimit = true;
	joint_left_def.lowerAngle = -42 * DEGTORAD;
	joint_left_def.upperAngle = 0 * DEGTORAD;
	joint_left_def.enableMotor = true;
	left_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&joint_left_def);

	// Right Circle
	x = 343;
	b2BodyDef body_r;
	body_r.type = b2_staticBody;
	body_r.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* right_ball = App->physics->world->CreateBody(&body_r);

	b2CircleShape shape_r;
	shape_r.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture_r;
	fixture_r.shape = &shape_r;
	right_ball->CreateFixture(&fixture_r);

	// Set Right Joint bodies
	body_FliperR = App->physics->world->CreateBody(&body_r);
	Fliper_R = App->physics->CreateRightSticker();

	// Set Right Joint

	b2RevoluteJointDef joint_right_def;
	joint_right_def.bodyA = body_FliperR;
	joint_right_def.bodyB = Fliper_R->body;
	joint_right_def.collideConnected = true;
	joint_right_def.localAnchorA.Set(PIXEL_TO_METERS(0), 0);
	joint_right_def.localAnchorB.Set(PIXEL_TO_METERS(40), PIXEL_TO_METERS(-2));
	joint_right_def.referenceAngle = -30 * DEGTORAD;
	joint_right_def.enableLimit = true;
	joint_right_def.lowerAngle = 0 * DEGTORAD;
	joint_right_def.upperAngle = 42 * DEGTORAD;
	joint_right_def.enableMotor = true;
	right_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&joint_right_def);
}