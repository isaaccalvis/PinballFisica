#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "p2Point.h"
#include "math.h"
#include "ModuleWindow.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start()
{
	pilota_Texture = App->textures->Load("pinball/ball.png");
	BarraInici_Texture = App->textures->Load("pinball/Barra.png");

	Bridge_A = App->audio->LoadFx("pinball/Bridge.wav");
	Bonus_A = App->audio->LoadFx("pinball/Bonus_Ball.wav");
	Tub_A = App->audio->LoadFx("pinball/Tub.wav");
	QuadPiramide_A = App->audio->LoadFx("pinball/Quad_Piramide.wav");
	Triangle_A = App->audio->LoadFx("pinball/Triangle.wav");
	Ball_A = App->audio->LoadFx("pinball/Ball.wav");
	Sticker_A = App->audio->LoadFx("pinball/Start.wav");
	Died_A = App->audio->LoadFx("pinball/Die.wav");

	Bridge = Wall;
	barraInici = App->physics->CreateBarraInici();
	Stickers();
	NewBall(530,600);

	Quad_Started = App->physics->CreateRectangle(527, 415, 34, 5, b2_staticBody, -0.5f);
	Quad_Started->body->SetActive(false);

	return true;
}

update_status ModulePlayer::Update()
{
	currentTime = SDL_GetTicks();

	left_rotation = RADTODEG * left_joint->GetJointAngle();
	right_rotation = RADTODEG * right_joint->GetJointAngle();

	left_joint->SetMotorSpeed(15);
	right_joint->SetMotorSpeed(-15);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		left_joint->EnableMotor(true);
		left_joint->SetMaxMotorTorque(700);
		left_joint->SetMotorSpeed(-15);
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			App->audio->PlayFx(Sticker_A);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		right_joint->EnableMotor(true);
		right_joint->SetMaxMotorTorque(700);
		right_joint->SetMotorSpeed(15);
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		App->audio->PlayFx(Sticker_A);

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
			App->audio->PlayFx(Sticker_A);
		}
	
	if (started && barrier) {
		started = false;
		barrier = false;
		Quad_Started->body->SetActive(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
		Circle_Body->body->ApplyLinearImpulse({0, -3.5f}, {0,0}, true);

	if (die) {
		die = false;
		barrier = true;
		App->audio->PlayFx(Died_A);
		App->physics->world->DestroyBody(Circle_Body->body);
		Quad_Started->body->SetActive(false);
		if (live > 0)
			NewBall(530,600);
		live--;
	}

	if (Sens_L) {
		int _X, _Y;
		Circle_Body->GetPosition(_X, _Y);
		if (_X == 160 && _Y == 211)
			Circle_Body->body->SetLinearVelocity({ 0,0 });
		else if (_X > 160)
			Circle_Body->body->SetLinearVelocity({ -1,0 });
		else if (_X < 160)
			Circle_Body->body->SetLinearVelocity({ 1,0 });
		else if (_Y > 211)
			Circle_Body->body->SetLinearVelocity({ 0,-1 });
		else if (_Y < 211)
			Circle_Body->body->SetLinearVelocity({ 0,1 });
	}

	if (Sens_R) {
		int _X, _Y;
		Circle_Body->GetPosition(_X, _Y);
		if (_X == 345 && _Y == 311)
			Circle_Body->body->SetLinearVelocity({ 0,0 });
		else if (_X > 345)
			Circle_Body->body->SetLinearVelocity({ -1,0 });
		else if (_X < 345)
			Circle_Body->body->SetLinearVelocity({ 1,0 });
		else if (_Y > 311)
			Circle_Body->body->SetLinearVelocity({ 0,-1 });
		else if (_Y < 311)
			Circle_Body->body->SetLinearVelocity({ 0,1 });
	}

	currentTime = SDL_GetTicks() - lastTime;
	if ((Sens_L || Sens_R) && currentTime > 1000) {
		Circle_Body->body->SetGravityScale(1);
		if (Sens_L) {
			Circle_Body->body->ApplyLinearImpulse({ -1.5f, 4 }, { 0,0 }, true);
			Sens_L = false;
		}
		else if (Sens_R) {
			Sens_R = false;
			Circle_Body->body->ApplyLinearImpulse({ -1.3f, 3 }, { 0,0 }, true);

		}
	}

	if (Bridge == Wall) {
		Quad_Bridge = App->physics->CreateRectangle(250, 35, 5, 38, b2_staticBody, -0.7f, nullptr);
		Bridge = Idle;
	}
	else if (Bridge == Destroy) {
		App->physics->world->DestroyBody(Quad_Bridge->body);
		Bridge = Rebuild;
		buildTime = SDL_GetTicks();
	}
	currentTime = SDL_GetTicks() - buildTime;
	if (Bridge == Rebuild && currentTime > 2000)
		Bridge = Wall;
	
	switch (live) {
	case 3:
		App->renderer->Blit(pilota_Texture, 630, 150, &pilotaRect);
	case 2:
		App->renderer->Blit(pilota_Texture, 610, 150, &pilotaRect);
	case 1:
		App->renderer->Blit(pilota_Texture, 590, 150, &pilotaRect);
		break;
	}
	char title[40];
	int n = sprintf_s(title, "2D Physics,     SCORE: %i", Score);
	App->window->SetTitle(title);
	return UPDATE_CONTINUE;
}

bool ModulePlayer::CleanUp()
{

	return true;
}



void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyB == App->scene_intro->sensor) {
		die = true;
	}
	else if (bodyB == App->scene_intro->Triangle_sens) {
		Score += 200;
		App->audio->PlayFx(QuadPiramide_A);
	}

	else if (bodyB == App->scene_intro->Brindge_sens) {
		if (Circle_Body->body->GetLinearVelocity().y > -1.5)
			Circle_Body->body->ApplyLinearImpulse({ 0, -2.5 }, { 0,0 }, true);
		else if(Circle_Body->body->GetLinearVelocity().y > -4)
			Circle_Body->body->ApplyLinearImpulse({ 0, -1.5 }, { 0,0 }, true);
		else
			Circle_Body->body->ApplyLinearImpulse({ 0, -1 }, { 0,0 }, true);
		Score += 300;
		App->audio->PlayFx(Bridge);
		Bridge = BrirgeWall::Destroy;
	}
	else if (bodyB == App->scene_intro->Tub_sens) {
		if (Circle_Body->body->GetLinearVelocity().y > -2)
			Circle_Body->body->ApplyLinearImpulse({ 0, -3 }, { 0,0 }, true);
		else
		Circle_Body->body->ApplyLinearImpulse({ 0, -1.5f }, { 0,0 }, true);
		Score += 400;
		App->audio->PlayFx(Tub_A);

	}
	else if (bodyB == App->scene_intro->L_Ball_sens || bodyB == App->scene_intro->R_Ball_sens) {
		Circle_Body->body->SetGravityScale(0);
		lastTime = SDL_GetTicks();
		Score += 100;
		App->audio->PlayFx(Bonus_A);
		if (bodyB == App->scene_intro->L_Ball_sens)
			Sens_L = true;
		else if (bodyB == App->scene_intro->R_Ball_sens)
			Sens_R = true;
	}

	else if (bodyB == App->scene_intro->BallU) {
		Score += 5;
		App->scene_intro->texturesSenseCollisio[0].temporitzador = 500 + SDL_GetTicks();
		App->audio->PlayFx(Ball_A);
	}
	else if (bodyB == App->scene_intro->BallD) {
		Score += 5;
		App->scene_intro->texturesSenseCollisio[1].temporitzador = 500 + SDL_GetTicks();
		App->audio->PlayFx(Ball_A);
	}
	else if (bodyB == App->scene_intro->BallE) {
		Score += 5;
		App->scene_intro->texturesSenseCollisio[2].temporitzador = 500 + SDL_GetTicks();
		App->audio->PlayFx(Ball_A);
	}
	else if (bodyB == App->scene_intro->BallB) {
		Score += 5;
		App->scene_intro->texturesSenseCollisio[3].temporitzador = 500 + SDL_GetTicks();
		App->audio->PlayFx(Ball_A);
	}

	else if (bodyB == App->scene_intro->BallG) {
		Score += 10;
		App->scene_intro->texturesSenseCollisio[4].temporitzador = 500 + SDL_GetTicks();
		App->audio->PlayFx(Ball_A);
	}
	else if (bodyB == App->scene_intro->Start_sens && barrier)
		started = true;

	else if(bodyB == App->scene_intro->placaD || bodyB == App->scene_intro->placaE)
		App->audio->PlayFx(Triangle_A);

	else if(bodyB == App->scene_intro->Tube_sens)
		Circle_Body->body->ApplyLinearImpulse({ 1, 0 }, { 0,0 }, true);


}

void ModulePlayer::NewBall(int x, int y) {
	Circle_Body = App->physics->CreateCircle(x, y, 8, b2_dynamicBody, 0.1f, pilota_Texture, SDL_Rect{ 0,0,16,16 });
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