#pragma once
#include <vector>
#include <time.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

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
	std::vector<btRigidBody*> bodies;
	std::vector<btRigidBody*> barriers;

	// ���ƽ�ɫ��
	btKinematicCharacterController* m_character;
	btPairCachingGhostObject* m_ghostObject;

	myBulletEngine();
	~myBulletEngine();

	void btInit();
	void btExit();

	void addGround();
	void addDinosaur();
	btRigidBody* addSphere(float radius, float x, float y, float z, float mass);


};