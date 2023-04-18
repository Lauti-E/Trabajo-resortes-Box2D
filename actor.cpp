#include "actor.h"

actor::actor(b2Body* cuerpo, RectangleShape* figura) {

	bodyActor = cuerpo;
	figActor = figura;

	posicion = bodyActor->GetPosition();

	dimensiones.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	dimensiones.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);

	for (b2Fixture* i = bodyActor->GetFixtureList();i;i = i->GetNext()) {
		dimensiones = i->GetAABB(0);
	}

	figActor->setSize({ dimensiones.GetExtents().x * 2, dimensiones.GetExtents().y * 2 });

	figActor->setOrigin(figActor->getSize().x / 2.f, figActor->getSize().y / 2.f);

	figActor->setPosition(posicion.x, posicion.y);
}

void actor::dibujar(RenderWindow& ventana) {

	posicion = bodyActor->GetPosition();

	figActor->setPosition(posicion.x, posicion.y);

	figActor->setRotation(rad2deg(bodyActor->GetAngle()));

	ventana.draw(*figActor);
}

float actor::rad2deg(float radianes) {

	//3.14 ______ 180
	//radian _____ ?

	return radianes * 180 / b2_pi;
}