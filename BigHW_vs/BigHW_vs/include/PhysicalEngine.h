#pragma once
#include <vector>
#include <time.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class myBulletEngine {
public:
	// bullet 初始化
	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfiguration;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;

	btRigidBody* ground = NULL;
	btRigidBody* dinosaur = NULL;
	std::vector<btRigidBody*> bodies;
	std::vector<btRigidBody*> barriers;

	// 控制角色类
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