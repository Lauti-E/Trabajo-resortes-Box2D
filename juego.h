#pragma once
#include "actor.h"

class juego {
private:
	RenderWindow* ventana;
	View* camara;

	Event* evento;

	Time* tiempo1;
	Clock* reloj;

	float fps, tiempoFrame, tiempo2;

	b2World* mundo;

	//Cuerpo

	b2Body* body[6];
	b2BodyDef bodyDef[6];
	b2Fixture* fixBody[6];
	b2FixtureDef fixDefBody[6];

	//Suelo

	b2Body* bodySuelo;
	b2BodyDef bodyDefSuelo;
	b2Fixture* fixSuelo;
	b2FixtureDef fixDefSuelo;

	RectangleShape* figBody[6];
	RectangleShape* figSuelo;

	actor* actBody[6];
	actor* actSuelo;

	//Resortes

	b2DistanceJoint* jointBody[5];
	b2DistanceJointDef jointDefBody[5];

public:
	juego(Vector2i resolucion, std::string titulo);

	void setCamara();
	void iniciarFisica();
	void iniciarImg();
	void actualizarFisica();
	void procesarEventos();
	void gameLoop();
	void dibujar();
};
