#include "PokeballSystem.h"
#include "../../ska/Graphic/DeleterComponent.h"

#define POWER 5

PokeballSystem::PokeballSystem(ska::EntityManager& entityManager) : 
System<std::unordered_set<ska::EntityId>, PokeballComponent, ska::PositionComponent>(entityManager) {
	m_sprite.load("."FILE_SEPARATOR"Sprites"FILE_SEPARATOR"Fight"FILE_SEPARATOR"pokeball.png", 4, 1, 4);
	m_sprite.setDelay(200);
	m_openPokeball.load("."FILE_SEPARATOR"Sprites"FILE_SEPARATOR"Fight"FILE_SEPARATOR"pokeball-openned.png", 1, 1, 1);
	m_vortex.load("."FILE_SEPARATOR"Sprites"FILE_SEPARATOR"Fight"FILE_SEPARATOR"pokeball-aura.png", 2, 1, 2, false, DEFAULT_T_RED, DEFAULT_T_GREEN, DEFAULT_T_BLUE, 100);
	m_vortex.setDelay(400);
}

PokeballSystem::~PokeballSystem() {

	

}

void PokeballSystem::refresh() {
	for (ska::EntityId entityId : m_processed) {
		PokeballComponent& pokec = m_entityManager.getComponent<PokeballComponent>(entityId);
		ska::PositionComponent& pc = m_entityManager.getComponent<ska::PositionComponent>(entityId);
		
		if (!pokec.started) {
			throwBall(entityId);
		}

		ska::GraphicComponent& gc = m_entityManager.getComponent<ska::GraphicComponent>(entityId);

		//Si la Pokeball est en l'air
		if ((pc.x > pokec.finalPos.x &&  pokec.sens == 0) || (pc.x < pokec.finalPos.x &&  pokec.sens == 1)) {
			//ska::Rectangle animPos = m_gestionAnim.getRectOfCurrentFrame(), oRel = { 0 };
			if (pc.x < pokec.finalPos.x) {
				pc.x += pokec.speed;
			}
			else {
				pc.x -= pokec.speed;
			}

			//calcul des coordonn�es � l'aide de l'�quation de parabole pr�alablement calcul�e dans Pokeball::launch
			pc.y = (int)(pokec.a*pc.x*pc.x + pokec.b*pc.x + pokec.c);

			//m_sprite.render(pc.x, m_pokeballPos.y, &animPos);
		} else if (!pokec.isInactive && !pokec.isOpenning)  {
			//sinon lorsqu'elle tombe par terre
			pokec.isOpenning = true;

			gc.sprite.clear();
			gc.sprite.push_back(m_openPokeball);

			/* Lorsque la pokeball s'ouvre, on y ajoute un effet temporaire de "vortex" */
			ska::EntityId vortexEntity = m_entityManager.createEntity();
			ska::DeleterComponent vortexDeleter;
			vortexDeleter.delay = 1500;
			ska::PositionComponent vortexPc = pc;
			vortexPc.x -= m_vortex.getWidth() / 2 - m_openPokeball.getWidth() / 2;
			vortexPc.y -= m_vortex.getHeight() / 2 - m_openPokeball.getHeight() / 2;

			m_entityManager.addComponent<ska::DeleterComponent>(vortexEntity, vortexDeleter);
			m_entityManager.addComponent<ska::PositionComponent>(vortexEntity, vortexPc);
			ska::GraphicComponent vortexGc;
			vortexGc.sprite.push_back(m_vortex);
			m_entityManager.addComponent<ska::GraphicComponent>(vortexEntity, vortexGc);

			/* On fini par ajouter un deleter sur cette Pokeball (avec un delai plus �lev� que celui du vortex) */
			ska::DeleterComponent deleter;
			deleter.delay = vortexDeleter.delay*2;
			m_entityManager.addComponent<ska::DeleterComponent>(entityId, deleter);
		}

	}
}

void PokeballSystem::throwBall(ska::EntityId entityId) {
	PokeballComponent& pokec = m_entityManager.getComponent<PokeballComponent>(entityId);
	ska::PositionComponent& pc = m_entityManager.getComponent<ska::PositionComponent>(entityId);

	ska::Point<int> leftPos;
	ska::Point<int> rightPos;
	if (pc.x < pokec.finalPos.x) {
		leftPos = pc;
		rightPos = pokec.finalPos;
		pokec.sens = 1;
	}
	else {
		rightPos = pc;
		leftPos = pokec.finalPos;
		pokec.sens = 0;
	}

	//pente de la droite passant par la mousePos et la pos centrale du hero
	float p;
	if (rightPos.x != leftPos.x) {
		p = (float)(rightPos.y - leftPos.y) / (rightPos.x - leftPos.x);
	}
	else {
		p = 100000;
	}

	//on adapte la vitesse � la distance � parcourir pour �viter de privil�gier les lancers de pokeball courts.
	pokec.speed = abs(pokec.finalPos.x - pc.x) / 30 + 1; 


	//calcul (au pr�alable fait sur papier) des coefficients de la parabole en fonction de la position du heros et de l'endroit voulu du lancer.
	//th�orique :
	//m_a = - (float) ((p*(xD - xF) + 2*m_power) + 2*sqrt(m_power*(m_power + p*(xD - xF)))) / ((xD - xF)*(xD - xF));


	//experimental (th�or�me de l'arrangement) :
	pokec.a = -(float)5 * ((leftPos.x - rightPos.x + 2 * POWER) + 2 * sqrt((float)(abs(POWER*(POWER + rightPos.x - leftPos.x))))) / ((rightPos.x - leftPos.x)*(rightPos.x - leftPos.x));
	pokec.b = p - pokec.a*(rightPos.x + leftPos.x);
	pokec.c = leftPos.y - pokec.a*leftPos.x*leftPos.x - pokec.b*leftPos.x;

	
	/* Initialisation logique (�tats) */
	pokec.started = true;
	pokec.isInactive = false;
	pokec.isOpenning = false;

	/* Initialisation Graphique */
	ska::GraphicComponent gc;
	gc.sprite.push_back(m_sprite);
	m_entityManager.addComponent<ska::GraphicComponent>(entityId, gc);
}
