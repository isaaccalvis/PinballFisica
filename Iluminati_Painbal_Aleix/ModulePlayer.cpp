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

	/*int palD[20] = {
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

	*/

	//////////////////////////////////////////////////////////////////////////////
	// Creacio Pilota
	/*b2BodyDef baseCircle;
	baseCircle.type = b2_dynamicBody;
	baseCircle.bullet = true;
	baseCircle.position.Set(PIXEL_TO_METERS(530), PIXEL_TO_METERS(600));
	b2CircleShape shapeCircle;
	shapeCircle.m_radius = PIXEL_TO_METERS(9);
	b2Body *body = App->physics->world->CreateBody(&baseCircle);
	Circle_Body = body;
	body->SetBullet(true);
	b2FixtureDef fixture;
	fixture.shape = &shapeCircle;
	fixture.density = 2;
	fixture.restitution = 0.7f;
	body->CreateFixture(&fixture);*/
	// ~Creacio Pilota

	Stickers();
	NewBall();


	//Creacio Barra
	b2BodyDef baseBarr;
	baseBarr.type = b2_staticBody;
	baseBarr.position.Set(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));
	b2PolygonShape shapePolygon;
	shapePolygon.SetAsBox(PIXEL_TO_METERS(10), PIXEL_TO_METERS(30), b2Vec2(0, 0), 0);
	b2Body *bodyBarr = App->physics->world->CreateBody(&baseBarr);
	BarrBody = bodyBarr;
	b2FixtureDef fixtureBarr;
	fixtureBarr.friction = 0.3f;
	fixtureBarr.density = 50;
	fixtureBarr.shape = &shapePolygon;
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


	////////////////////////////
	int x, y;
	Circle_Body->GetPosition(x, y);
	App->renderer->Blit(Circle_Texture, METERS_TO_PIXELS(x), METERS_TO_PIXELS(y), &Circle_Rect);
	b2Vec2 vecGoDownBarr(0, -0.01f);
	b2Vec2 vecGoUpBarr(0, 0.2f);
	b2Vec2 vecGoStartBarr(PIXEL_TO_METERS(530), PIXEL_TO_METERS(770));
	printf_s("%f\n", BarrBody->GetPosition().y);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && start) {
		start = false;
		Circle_Body->body->ApplyLinearImpulse({0, -5}, {0,0}, true);
		//Line join
	}
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





	if (die) {
		die = false;
		App->physics->world->DestroyBody(Circle_Body->body);
		if (live > 0)
			NewBall();
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