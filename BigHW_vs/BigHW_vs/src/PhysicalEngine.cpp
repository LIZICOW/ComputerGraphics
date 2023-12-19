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

//��������δʵ��
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

// Cube��������
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
	// ���������ʼ��
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
	// ��ӵ���
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
	// ���С����
	btTransform t;
	t.setIdentity();							// ���任����Ϊ��λ�任
	t.setOrigin(btVector3(100.f, 0.f, 0.f));	// ���ñ任λ��

	// ����һ���������Ghost�������ڱ�ʾһ�������顱�����������������������ƶ�������ʵ�ʵ�������Ӱ��
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(t);  // ����������������任Ϊ���涨��ı任

	// �������������е�Ghost�������Ի���ص���������ͨ�����ڰ����Ż���ײ���
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	// �����ɫ�ĸ߶ȺͿ��
	btScalar characterHeight = 2.0f;
	btScalar characterWidth = 2.0f;

	// ����һ��������״�����ڱ�ʾ��ɫ����ײ��״
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	m_ghostObject->setCollisionShape(capsule);	// ��������״����Ϊ����������ײ��״
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);	// ����ghost�������ײ��־Ϊ��ɫ����

	// �����ɫ�Ĳ��и߶�
	btScalar stepHeight = btScalar(0.35);

	// ����һ����̬��ɫ����������ʹ������������������ͽ�����״��������һ�����и߶�
	m_character = new btKinematicCharacterController(m_ghostObject, capsule, stepHeight);

	m_character->setGravity(btVector3(0, -10000 / 80, 0));  // ���ý�ɫ����������

	// �����������ӵ������������ײ�����б���
	world->addCollisionObject(
		m_ghostObject,
		btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
	);

	// ����̬��ɫ��������ӵ���������Ķ����б��У�ʹ��������ģ������Ч
	world->addAction(m_character);
}

btRigidBody* myBulletEngine::addSphere(float radius, float x, float y, float z, float mass)
{
	//��ʼ��λ����Ϣ
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	//������״
	btSphereShape* sphere = new btSphereShape(radius);

	btVector3 inertia(0, 0, 0);
	//����Ϊ�㼴��̬����
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	//Ϊ���������˶���̬��Ϣ
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere);
	btRigidBody* body = new btRigidBody(info);
	//��ӵ�������
	body->setRestitution(btScalar(0.5));
	world->addRigidBody(body);

	body->getCollisionShape()->setUserIndex(bt_cacuts);



	bodies.push_back(body);

	return body;
}
