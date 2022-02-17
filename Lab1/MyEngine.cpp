#include "pch.h"
#include "MyEngine.h"

MyEngine::MyEngine() : ApplicationContext("T-637-GEDE Lab 1")
{
}

void MyEngine::setup()
{
	ApplicationContext::setup();
	Root* root = getRoot();
	/*
	 * The SceneManager keeps track of the locations and other attributes of the objects in your scene. 
	 * The SceneManager also manages any cameras that you add to your scene
	 */
	SceneManager* sceneManager = root->createSceneManager();
	/*
	 * ShaderGenerator is a singleton class that enables automatic generation of shader code 
	 * based on existing material techniques and we need to register our scene with i
	 */
	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(sceneManager);

	/*
	 * A SceneNode can have children nodes, whose transforms are relative to the parent node. 
	 * SceneNodes can have objects attached to them such as lights and 3d models. 
	 * Think of the tree of SceneNodes as a Christmastree that can be decorated with 
	 * the various elements you wish to have in the scene. In this case, we create a child node 
	 * of the root node (top of the tree) and add our light to that node. 
	 * By default, the light is a point light source but that can be modified.
	 */
	Light* light = sceneManager->createLight("MainLight");
	SceneNode* lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	lightNode->setPosition(0, 10, 15);
	lightNode->attachObject(light);

	/*
	 * Add a camera at a node right below the root node
	 */
	SceneNode* cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	cameraNode->setPosition(0, 0, 15);
	cameraNode->lookAt(Vector3(0, 0, -1), Node::TS_PARENT);
	Camera* cam = sceneManager->createCamera("MainCamera");
	cam->setNearClipDistance(5);
	cam->setAutoAspectRatio(true);
	cameraNode->attachObject(cam);
	getRenderWindow()->addViewport(cam);

	/*
	 *  Add a 3D model mesh, also attached to a node right below the root node
	 */
	Entity* ent = sceneManager->createEntity("Sinbad.mesh");
	SceneNode* sinbadNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	sinbadNode->attachObject(ent);
}