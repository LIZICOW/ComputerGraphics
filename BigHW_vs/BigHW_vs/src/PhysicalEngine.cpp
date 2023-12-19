#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>
//#include <learnopengl/filesystem.h>
#include <learnopengl/model.h>
#include <learnopengl/shader_m.h>
#include <stb_image.h>

#include <iostream>

//声音，暂未实现
//#include <irrKlang.h>
//extern irrklang::ISoundEngine* engine;

#include <stdio.h>
#include <btBulletDynamicsCommon.h>

#include "../include/PhysicalEngine.h"


enum ObjIndex {
	bt_dinosaur,
	bt_birds,
	bt_cacuts,
	bt_Ground
};

// Cube定点数据
float cube_vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void cubeInit(unsigned int& cubeVAO, unsigned int& cubeVBO)
{
	int stride = (3 + 3 + 2);

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

myBulletEngine::myBulletEngine() {};

myBulletEngine::~myBulletEngine() {};

void myBulletEngine::btInit()
{
	// 物理引擎初始化
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
	world->setGravity(btVector3(0, -10, 0));
}

void myBulletEngine::btExit()
{
	delete dispatcher;
	delete collisionConfiguration;
	delete solver;
	delete world;
	delete broadphase;
}

void myBulletEngine::addGround() {
	// 添加地面
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);

	body->getCollisionShape()->setUserIndex(bt_Ground);

	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);
}

void myBulletEngine::addDinosaur() {
	// 添加小恐龙
	btTransform t;
	t.setIdentity();							// 将变换重置为单位变换
	t.setOrigin(btVector3(100.f, 0.f, 0.f));	// 设置变换位置

	// 创建一个带缓存的Ghost对象，用于表示一个“幽灵”对象，它可以在物理世界中移动但不受实际的物理力影响
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(t);  // 设置幽灵对象的世界变换为上面定义的变换

	// 设置物理世界中的Ghost对象的配对缓存回调函数，这通常用于帮助优化碰撞检测
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	// 定义角色的高度和宽度
	btScalar characterHeight = 2.0f;
	btScalar characterWidth = 2.0f;

	// 创建一个胶囊形状，用于表示角色的碰撞形状
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	m_ghostObject->setCollisionShape(capsule);	// 将胶囊形状设置为幽灵对象的碰撞形状
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);	// 设置ghost对象的碰撞标志为角色对象

	// 定义角色的步行高度
	btScalar stepHeight = btScalar(0.35);

	// 创建一个动态角色控制器，它使用上述定义的幽灵对象和胶囊形状，并设置一个步行高度
	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight);

	m_character->setGravity(btVector3(0, -10000 / 80, 0));  // 设置角色的重力向量

	// 将幽灵对象添加到物理世界的碰撞对象列表中
	world->addCollisionObject(
		m_ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
	);

	// 将动态角色控制器添加到物理世界的动作列表中，使其在物理模拟中生效
	world->addAction(m_character);
}

btRigidBody* myBulletEngine::addSphere(float radius, float x, float y, float z, float mass)
{
	//初始化位姿信息
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	//生成形状
	btSphereShape* sphere = new btSphereShape(radius);

	btVector3 inertia(0, 0, 0);
	//质量为零即静态物体
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//为物体增添运动姿态信息
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);
	//添加到世界中
	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);

	body->getCollisionShape()->setUserIndex(bt_cacuts);



	bodies.push_back(body);

	return body;
}
