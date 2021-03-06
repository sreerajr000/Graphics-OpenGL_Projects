#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletWorldImporter/btBulletWorldImporter.h>
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfig;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* world;
btBulletWorldImporter *importer;


std::vector<btRigidBody *> bodies;
std::vector<glm::mat4> bodyPos;

btRigidBody* addSphere(float rad, float x, float y, float z, float mass) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btSphereShape *sphere = new btSphereShape(rad);
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	btMotionState *motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere,
			inertia);
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);
	return body;
}

void addCameraBody(float rad, float x, float y, float z, float mass) {
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btSphereShape *sphere = new btSphereShape(rad);
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0) {
		sphere->calculateLocalInertia(mass, inertia);
	}
	btMotionState *motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere,
			inertia);
	cameraBody = new btRigidBody(info);
	world->addRigidBody(cameraBody);
}

void physicsSetUp() {
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
			collisionConfig);
	//importer = new btBulletWorldImporter(world);
	//importer->loadFile("/home/sree/test.bullet");
	world->setGravity(btVector3(0, -10, 0));

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState *motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody *body = new btRigidBody(info);
	body->setRestitution(0.5);
	body->setFriction(0.5);
	//world->addRigidBody(body);
	bodies.push_back(body);
	bodyPos.push_back(glm::mat4());


	addCameraBody(0.1, 0, 10, 10, 1.0);
	body = landscape->createRigidBodyFromMesh(0, 0, 0, 0);
	world->addRigidBody(body);
	bodies.push_back(body);
}

void renderSphere(btRigidBody *sphere, int i) {
	if(sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
		return;

	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	model = glm::make_mat4(mat);

	//render sphere after model * mat
}

void renderPlane(btRigidBody *plane, int i) {
	if(plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
		return;

	btTransform t;
	plane->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);

	//render sphere after model * mat
	model = glm::make_mat4(mat);
}

void cleanPhysics() {

	for(int i= 0; i < bodies.size(); i++){
		world->removeCollisionObject(bodies[i]);
		btMotionState *motionState = bodies[i]->getMotionState();
		btCollisionShape *shape= bodies[i]->getCollisionShape();
		delete bodies[i];
		delete shape;
		delete motionState;
	}
	delete cameraBody;
	delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadphase;
	delete world;

}

void throwSphere() {
	float angle = glm::atan(camera.Front.z / camera.Front.x);
	float x = camera.Position.x + glm::cos(angle) * 5;
	float z = camera.Position.z + glm::sin(angle) * 5;
	btRigidBody *sphere = addSphere(1.0, x, camera.Position.y, z, 1.0);
	glm::vec3 look = camera.Front * glm::vec3(20);
	sphere->setLinearVelocity(btVector3(look.x, look.y, look.z));
	sphere->setRestitution(1.0);
	sphere->setFriction(0.5);

}

void characterMoveForward() {
	glm::vec3 look = camera.Front * glm::vec3(0.025);
	moveForward = false;
	cameraBody->translate(btVector3(look.x, 0, look.z));
}

void characterMoveBackward() {
	moveBackward = false;
	glm::vec3 look = camera.Front * glm::vec3(-0.025);
	cameraBody->translate(btVector3(look.x, 0, look.z));
}

void move() {
	cameraBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform t;
	cameraBody->getMotionState()->getWorldTransform(t);
	camera.Position = glm::vec3(t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ());
}


