#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"

using namespace sf;

class actor {
private:
	b2Body* bodyActor;
	RectangleShape* figActor;

	b2AABB dimensiones;
	b2Vec2 posicion;
public:
	actor(b2Body* cuerpo, RectangleShape* figura);

	void dibujar(RenderWindow& ventana);

	float rad2deg(float radianes);
};