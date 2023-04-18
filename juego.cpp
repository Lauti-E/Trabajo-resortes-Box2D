#include "juego.h"

juego::juego(Vector2i resolucion, std::string titulo) {

	fps = 60;
	tiempoFrame = 1 / 60.f;
	tiempo2 = 0;

	ventana = new RenderWindow(VideoMode(resolucion.x, resolucion.y), titulo);
	ventana->setFramerateLimit(fps);

	setCamara();
	iniciarFisica();
	iniciarImg();

	evento = new Event;

	tiempo1 = new Time;
	reloj = new Clock;

	for (int i = 0;i < 6;i++) {
		actBody[i] = new actor(body[i], figBody[i]);
	}

	actSuelo = new actor(bodySuelo, figSuelo);

	gameLoop();
}

void juego::setCamara() {

	camara = new View({ 12.5f, 12.5f }, { 25.f, 25.f });

	camara->move(49.f, 77.f);

	ventana->setView(*camara);
}
void juego::iniciarFisica() {

	//Suelo
	mundo = new b2World(b2Vec2(0.f, 9.81f));

	bodyDefSuelo.type = b2_staticBody;
	bodyDefSuelo.position = b2Vec2(50.f, 103.50f);

	bodySuelo = mundo->CreateBody(&bodyDefSuelo);

	b2PolygonShape shapeSuelo;
	shapeSuelo.SetAsBox(50.f, 2.5f);

	fixDefSuelo.shape = &shapeSuelo;
	fixDefSuelo.restitution = 0.f;
	fixDefSuelo.friction = 0.3f;
	fixDefSuelo.density = 1.f;

	fixSuelo = bodySuelo->CreateFixture(&fixDefSuelo);

	//Cuerpo

	//Posicionando objetos
	bodyDef[0].position = b2Vec2(60.f, 88.30f); //Cabeza

	bodyDef[1].position = b2Vec2(60.f, 90.f); //Torso

	bodyDef[2].position = b2Vec2(60.95f, 89.50f); //Brazo derecho

	bodyDef[3].position = b2Vec2(59.05f, 89.50f); //Brazo izquierdo

	bodyDef[4].position = b2Vec2(60.95f, 92.f); //Pierna derecha

	bodyDef[5].position = b2Vec2(59.05f, 92.f); //Pierna izquierda


	for (int i = 0;i < 6;i++) {
		bodyDef[i].type = b2_dynamicBody;
		body[i] = mundo->CreateBody(&bodyDef[i]);
	}

	b2PolygonShape shapeBody[6];

	//Dimensiones del cuerpo
	shapeBody[0].SetAsBox(0.25, 0.30); //Cabeza
	shapeBody[1].SetAsBox(0.62, 1.25); //Torso
	shapeBody[2].SetAsBox(0.150, 0.75); //Brazo derecho
	shapeBody[3].SetAsBox(0.150, 0.75); //Brazo izquierdo
	shapeBody[4].SetAsBox(0.210, 0.75); //Pierna derecha
	shapeBody[5].SetAsBox(0.210, 0.75); //Pierna izquierda

	for (int i = 0;i < 6;i++) {
		fixDefBody[i].shape = &shapeBody[i];
		fixDefBody[i].density = 1.0f;
		fixDefBody[i].friction = 0.3f;
		fixDefBody[i].restitution = 0.3f;
		fixBody[i] = body[i]->CreateFixture(&fixDefBody[i]);
	}

	//Resortes

	//Resorte cabeza - torso.
	jointDefBody[0].Initialize(body[0], body[1], b2Vec2(body[0]->GetPosition().x, body[0]->GetPosition().y + 0.23),
		b2Vec2(body[1]->GetPosition().x, body[1]->GetPosition().y - 1.23));

	//Resorte torso - brazo derecho.
	jointDefBody[1].Initialize(body[1], body[2], b2Vec2(body[1]->GetPosition().x + 0.60, body[1]->GetPosition().y - 1.23),
		b2Vec2(body[2]->GetPosition().x - 0.148, body[2]->GetPosition().y - 0.73));

	//Resorte torso - brazo izquierdo.
	jointDefBody[2].Initialize(body[1], body[3], b2Vec2(body[1]->GetPosition().x - 0.60, body[1]->GetPosition().y - 1.23),
		b2Vec2(body[3]->GetPosition().x + 0.148, body[3]->GetPosition().y - 0.73));

	//Resorte torso - pierna derecha.
	jointDefBody[3].Initialize(body[1], body[4], b2Vec2(body[1]->GetPosition().x + 0.60, body[1]->GetPosition().y + 1.23),
		b2Vec2(body[4]->GetPosition().x - 0.208, body[4]->GetPosition().y - 0.73));

	//Resorte torso - pierna izquierda.
	jointDefBody[4].Initialize(body[1], body[5], b2Vec2(body[1]->GetPosition().x - 0.60, body[1]->GetPosition().y + 1.23),
		b2Vec2(body[5]->GetPosition().x + 0.208, body[5]->GetPosition().y - 0.73));

	for (int i = 0;i < 5;i++) {
		jointDefBody[i].dampingRatio = 0.3f;
		jointDefBody[i].frequencyHz = 4.0f;
		jointDefBody[i].length = 0.010f;
		jointDefBody[i].collideConnected = true;
		jointBody[i] = (b2DistanceJoint*)mundo->CreateJoint(&jointDefBody[i]);
	}
}

void juego::iniciarImg() {

	figSuelo = new RectangleShape;
	figSuelo->setFillColor(Color::Blue);

	for (int i = 0;i < 6;i++) {
		figBody[i] = new RectangleShape;
	}

	//Color cabeza.
	figBody[0]->setFillColor(Color::Cyan);
	//Color torso.
	figBody[1]->setFillColor(Color::Red);
	//Color brazo derecho e izquierdo.
	figBody[2]->setFillColor(Color::White);
	figBody[3]->setFillColor(Color::White);
	//Color pierna derecha e izquierda.
	figBody[4]->setFillColor(Color::Magenta);
	figBody[5]->setFillColor(Color::Magenta);
}

void juego::actualizarFisica() {

	mundo->Step(tiempoFrame, 8, 8);

	mundo->ClearForces();
}

void juego::procesarEventos() {

	while (ventana->pollEvent(*evento)) {

		switch (evento->type) {
		case Event::Closed:
			exit(1);
			break;
		}
	}
}

void juego::gameLoop() {

	while (ventana->isOpen()) {

		*tiempo1 = reloj->getElapsedTime();

		if (tiempo2 + tiempoFrame < tiempo1->asSeconds()) {

			tiempo2 = tiempo1->asSeconds();

			ventana->clear();

			procesarEventos();

			actualizarFisica();

			dibujar();

			ventana->display();
		}
	}
}

void juego::dibujar() {

	actSuelo->dibujar(*ventana);

	for (int i = 0;i < 6;i++) {
		actBody[i]->dibujar(*ventana);
	}
}