#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>
//#include <learnopengl/filesystem.h>
//#include <learnopengl/model.h>
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
	world->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;	// 穿透系数
	world->setGravity(btVector3(0, -15.8, 0));
}

void myBulletEngine::btExit()
{
	delete dispatcher;
	delete collisionConfiguration;
	delete solver;
	delete world;
	delete broadphase;
}

void myBulletEngine::addGround(btVector3 shape, btVector3 position)
{
	btCollisionShape* Shape = new btBoxShape(shape);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);

	btScalar mass = 0.f;
	btVector3 localInertia(0., 0., 0.);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, Shape, localInertia);
	ground = new btRigidBody(cInfo);
	ground->setRestitution(0);	// 反弹系数
	world->addRigidBody(ground);
}

// 长方体表示dinosaur
void myBulletEngine::addDinosaur(btVector3 cubeshape, btVector3 position)
{
	// 创建一个立方体，并加入到场景中
	btCollisionShape* shape = new btBoxShape(cubeshape);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);  // 设置初始位置

	btScalar mass = 2.f;
	btVector3 localInertia(0, 0, 0);

	// 计算局部惯性
	shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans); // 创建默认运动状态对象
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia); // 创建刚体构造信息对象
	btRigidBody* g_body = new btRigidBody(cInfo); // 创建刚体

	g_body->setRestitution(0.5);	// 设置反弹系数，不想Q弹设置为0

	dinosaur = g_body;
	world->addRigidBody(g_body);
}

//// 加障碍物，均为长方体
//void myBulletEngine::addBarrier(btVector3 cubeshape, btVector3 position, float mass, btVector3 speed) 
//{
//	// 创建一个立方体，并加入到场景中
//	btCollisionShape* shape = new btBoxShape(cubeshape);
//	btTransform trans;
//	trans.setIdentity();
//	trans.setOrigin(position);  // 设置球体的初始位置
//
//	btVector3 localInertia(0, 0, 0);
//	bool isDynamic = (mass != 0.f);
//
//	// 如果是动态长方体，计算局部惯性
//	if (isDynamic)
//		shape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans); // 创建默认运动状态对象
//	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia); // 创建刚体构造信息对象
//	btRigidBody* g_body = new btRigidBody(cInfo); // 创建刚体
//
//	// 设置速度
//	g_body->setLinearVelocity(speed);
//
//	Barrier* barrier = new Barrier(g_body);
//	barriers.push_back(barrier);
//	world->addRigidBody(g_body);
//}
//
//// 加会飞障碍物，均为长方体
//void myBulletEngine::addCloud(btVector3 cubeshape, btVector3 position, float mass, btVector3 speed)
//{
//	// 创建一个立方体，并加入到场景中
//	btCollisionShape* shape = new btBoxShape(cubeshape);
//	btTransform trans;
//	trans.setIdentity();
//	trans.setOrigin(position);  // 设置球体的初始位置
//
//	btVector3 localInertia(0, 0, 0);
//	bool isDynamic = (mass != 0.f);
//
//	// 如果是动态长方体，计算局部惯性
//	if (isDynamic)
//		shape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans); // 创建默认运动状态对象
//	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia); // 创建刚体构造信息对象
//	btRigidBody* g_body = new btRigidBody(cInfo); // 创建刚体
//
//	// 设置速度
//	g_body->setLinearVelocity(speed);
//	// 设置重力为0
//	g_body->setGravity(btVector3(0, 0, 0));
//
//	Barrier* cloud = new Barrier(g_body, true);
//
//	clouds.push_back(cloud);
//	world->addRigidBody(g_body);
//}
void myBulletEngine::addBarrier(btVector3 cubeshape, btVector3 position, float mass, btVector3 speed, bool fly)
{
	// 创建一个立方体，并加入到场景中
	btCollisionShape* shape = new btBoxShape(cubeshape);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);  // 设置球体的初始位置

	btVector3 localInertia(0, 0, 0);
	bool isDynamic = (mass != 0.f);

	// 如果是动态长方体，计算局部惯性
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(trans); // 创建默认运动状态对象
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia); // 创建刚体构造信息对象
	btRigidBody* g_body = new btRigidBody(cInfo); // 创建刚体

	// 设置速度
	g_body->setLinearVelocity(speed);

	// 设置重力为0
	g_body->setGravity(btVector3(0, 0, 0));
	g_body->setRestitution(1);

	Barrier* barrier = new Barrier(g_body, fly);

	barriers.push_back(barrier);
	world->addRigidBody(g_body);
}

void myBulletEngine::DinosaurJump(btVector3 force)
{
	btTransform trans;
	dinosaur->getMotionState()->getWorldTransform(trans);
	// 假设 trans 是一个 btTransform 类型的对象
	btVector3 pos = trans.getOrigin();

	std::cout << "dinosaur Position: x=" << pos.getX()
		<< ", y=" << pos.getY()
		<< ", z=" << pos.getZ() << std::endl;
	for (auto& x : useBarriers) {
		btTransform trans;
		x->barrier->getMotionState()->getWorldTransform(trans);
		// 假设 trans 是一个 btTransform 类型的对象
		btVector3 pos = trans.getOrigin();

		std::cout << "Position: x=" << pos.getX()
			<< ", y=" << pos.getY()
			<< ", z=" << pos.getZ() << std::endl;

	}
	std::cout << std::endl;

	dinosaur->activate(true);
	dinosaur->applyCentralImpulse(force);	// 施加冲量
}

void myBulletEngine::CloudsFly(float deltaTime, float amplitude, float frequency)
{
	static float time = 0.0f;
	time += deltaTime;
	for (auto& x : useBarriers) {
		if (x->fly) {
			btTransform trans;
			x->barrier->getMotionState()->getWorldTransform(trans);
			btVector3 pos = trans.getOrigin();
			pos.setY(pos.y() + amplitude * sin(frequency * time));
			trans.setOrigin(pos);
			x->barrier->getMotionState()->setWorldTransform(trans);
		}
	}
}

// 渲染地面
void myBulletEngine::renderGround(Shader& shader)
{
	btTransform trans = ground->getWorldTransform();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	shader.setMat4("model", model);
}

// 渲染恐龙
void myBulletEngine::renderDinosaur(Shader& shader)
{
	btTransform trans = dinosaur->getWorldTransform();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	shader.setMat4("model", model);
}

glm::mat4 myBulletEngine::getDinosaurMode() {
	btTransform trans = dinosaur->getWorldTransform();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	return model;
}

glm::mat4 myBulletEngine::getBarriersMode(int index)
{
	btTransform trans = useBarriers[index]->barrier->getWorldTransform();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
	return model;
}


// 渲染障碍物（同时添加速度）
void myBulletEngine::renderBarriers(Shader& shader)
{
	/*for (btRigidBody* body : barriers) {
		btTransform trans = body->getWorldTransform();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		shader.setMat4("model", model);
	}*/
}

