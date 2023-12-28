#pragma once
#include <vector>
#include <time.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class Barrier {
public:
	int id;
	bool fly;
	btRigidBody* barrier;
	Barrier(btRigidBody* body = NULL, bool f = false) {
		id = -1;
		fly = f;
		barrier = body;
	}
	void setID(int index) {
		id = index;
	}
	void setPosition(btVector3 position) {
		if (barrier) {
			btTransform trans;
			barrier->getMotionState()->getWorldTransform(trans); // Get the current transform
			trans.setOrigin(position); // Set the new position
			barrier->getMotionState()->setWorldTransform(trans); // Apply the new transform
		}
	}
	void setyz0() {
		if (barrier) {
			btTransform trans;
			barrier->getMotionState()->getWorldTransform(trans); // Get the current transform
			btVector3 pos = trans.getOrigin();
			trans.setOrigin(btVector3(pos.getX(), 0.f, 0.f));
			barrier->getMotionState()->setWorldTransform(trans); // Apply the new transform
		}
	}
};

class myBulletEngine {
public:
	// bullet ��ʼ��
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;

	btRigidBody* ground = NULL;
	btRigidBody* dinosaur = NULL;
	std::vector<Barrier*> barriers;	  // ���е��ϰ���
	//std::vector<Barrier*> clouds;	  // ��ɵ��ϰ���
	std::vector<Barrier*> useBarriers;	// ���ֵ��ϰ���



	/********************************************************/
	myBulletEngine();
	~myBulletEngine();

	void btInit();
	void btExit();

	void addGround(btVector3 shape, btVector3 position);
	void addDinosaur(btVector3 cubeshape, btVector3 position);
	//void addBarrier(btVector3 cubeshape, btVector3 position, float mass, btVector3 speed);
	//void addCloud(btVector3 cubeshape, btVector3 position, float mass, btVector3 speed);
	void addBarrier(btVector3 cubeshape, btVector3 position, float mass, btVector3 speed, bool fly);

	void DinosaurJump(btVector3 force);
	void CloudsFly(float deltaTime, float amplitude, float frequency);

	void renderGround(Shader& shader);
	void renderDinosaur(Shader& shader);
	void renderBarriers(Shader& shader);

	glm::mat4 getDinosaurMode();
	glm::mat4 getBarriersMode(int index);

};