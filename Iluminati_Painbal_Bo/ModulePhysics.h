#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -9.8f

#define PIXELS_PER_METER 50.0f
#define METER_PER_PIXEL 0.02f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL){}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	SDL_Texture* textureActual = nullptr;
	SDL_Rect textureRect;

	int width, height;
	b2Body* body;
	Module* listener;

};

class ModulePhysics : public Module, public b2ContactListener
{
public:

	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type, float res = 0, SDL_Texture* textura = nullptr, SDL_Rect rectTextura = { 0,0,0,0 });
	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType type, float angle = 0, SDL_Texture* textura = nullptr, SDL_Rect rectTextura = {0,0,0,0});
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type, float res);
	PhysBody* CreateLeftSticker();
	PhysBody* CreateRightSticker();
	b2PrismaticJoint* CreateBarraInici();

	void BeginContact(b2Contact* contact);

	b2World* world;

private:

	bool debug;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

};