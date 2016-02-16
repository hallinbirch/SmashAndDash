#ifndef DEF_PHYSICOBJECT
#define DEF_PHYSICOBJECT

#include <stdlib.h>
#include <vector>
#include <SDL2/SDL.h>
#include "Force.h"

class World;

bool FindID(std::vector<SDL_Rect> &ids, SDL_Rect id);

class PhysicObject
{
	public:
		PhysicObject(int id, unsigned int entityNumber, float weight, float frictionCoeff, unsigned int speedLimit, SDL_Rect r);
		PhysicObject(int id, unsigned int entityNumber, float weight, float frictionCoeff, unsigned int speedLimit, SDL_Rect r, int offset[4]);
		PhysicObject(int id, unsigned int entityNumber, float weight, float frictionCoeff, unsigned int speedLimit, SDL_Rect r, int offset0, int offset1, int offset2, int offset3);
		PhysicObject(int id, unsigned int entityNumber, float weight, float frictionCoeff, unsigned int speedLimit, int x, int y, unsigned int w, unsigned int h, int offset0, int offset1, int offset2, int offset3);
		PhysicObject(int id, unsigned int entityNumber);

        SDL_Rect getPos();
        int getDirection() const;
        SDL_Rect getCenterPos();
		SDL_Rect getHitboxCenterPos() const;
        int getID();
        float getAngle();
        float getSpeed();
        unsigned int getEntityNumber();
        int getOffset(int direction);
        unsigned int getSpeedLimit();
        SDL_Rect getHitbox();
		bool getGhost();
		int getJumpHeight();

		void setSpeedLimit(int speedLimit);
		void setMoving(bool b);
        void setEntityNumber(unsigned int n);
        void setGhost(bool x);
        void setDirection(int x);
        void setOffset(int direction, int nbr);
		void setJumpHeight(int jumpHeight);
		void applyForce(Force f);										//mouvement automatique déterminé par l'application d'une force au barycentre de l'objet
		void applyForce(int direction, float power);         			//déplacer un objet dans une des 8 directions
		void applyForce(SDL_Rect dest, float power);    				//déplacer un objet vers une destination
        void teleport(int x, int y);
        void reset();
        bool movable();
		bool collisionWorld();          								//retourne true s'il y a collision avec le décor ou d'autres éléments solides du monde, false sinon
        std::vector<SDL_Rect> collisionNPC();
		virtual void refresh();													//calcule automatiquement v(t) à l'aide de v(t-1) et des forces + accélération
		void jump(unsigned int power);
		bool isOnGround();
		bool isDodging();
		bool collision();
		bool collision(SDL_Rect targetPos);

		~PhysicObject();


	private:                                                            //tout ce qui touche à la mécanique de mouvement interne à la classe
		float m_frictionCoeff, m_weight, m_slopeMove, m_ax, m_ay, m_az, m_angle;


	protected :															//tout ce qui doit être accessible dans les classes filles, sans se soucier de l'accélération ou des forces
		void move(SDL_Rect dest, float speed); //bouge dans la direction déterminée par "dest" et à la vitesse "speed"
		void move(int direction, float speed);
		void move(SDL_Rect dest);				//bouge dans une direction vers "dest", à la vitesse courante de l'objet
		virtual std::vector<SDL_Rect> move();	//bouge dans la direction courante à la vitesse courante

		float m_speedx, m_speedy, m_speedz;								        //appliquées à l'objet, on accède juste aux vitesses et positions
		bool m_ghost, m_movementsEnabled, m_canFly, m_jumpStarted;                  //m_ghost : mouvements sans condition de collision
		SDL_Rect m_rect, m_lastGroundPos, m_jumpTargetPos;
		int m_id, m_offset[4], m_jumpHeight;
		unsigned int m_direction, m_entityNumber, m_speedLimit, m_jumpPower;

};

#endif