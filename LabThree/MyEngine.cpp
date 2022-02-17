#include "pch.h"
#include "MyEngine.h"
#include "PickupManager.h"

MyEngine::MyEngine() : ApplicationContext("T-637-GEDE Lab 3")
{
}

void MyEngine::setup()
{
	ApplicationContext::setup();
	addInputListener(this);
	setupSceneManager();
	setupCamera();
	populateScene();
}

void MyEngine::setupSceneManager()
{
	_root = getRoot();
	/*
	 * The SceneManager keeps track of the locations and other attributes of the objects in your scene.
	 * The SceneManager also manages any cameras that you add to your scene
	 */
	_scene_manager = _root->createSceneManager();
	/*
	 * ShaderGenerator is a singleton class that enables automatic generation of shader code
	 * based on existing material techniques and we need to register our scene with i
	 */
	RTShader::ShaderGenerator* shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();
	shaderGenerator->addSceneManager(_scene_manager);
}


void MyEngine::setupCamera()
{

	/*
	 * Add a camera at a node right below the root node
	 */
	 // Add camera
	// _camera_node = _scene_manager->getRootSceneNode()->createChildSceneNode();
	_roaming_camera = new RoamingCamera(_scene_manager, getRenderWindow(), Vector3(0, 0, 50));
}

void MyEngine::populateScene()
{
	_player = new PlayerAvatar(_scene_manager, "Sinbad.mesh");
	// Add Ground
	Plane plane(Ogre::Vector3::UNIT_Y, -5);
	MeshPtr groundMesh = MeshManager::getSingleton()
		.createPlane("MainGround", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane, 1000, 1000, 100, 100, true, 1, 50, 50, Ogre::Vector3::UNIT_Z);

	// build tangent vectors for our mesh, to show the normal texture
	// This will make the floor tiles look like they have depth, even thought they are flat
	unsigned short src, dest;
	if (!groundMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
	{
		groundMesh->buildTangentVectors(VES_TANGENT, src, dest);
	}

	Entity* groundEntity = _scene_manager->createEntity("LightPlaneEntity", "MainGround");
	_scene_manager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);

	// Specify the material that is going to be used to render the floor tiles
	groundEntity->setMaterialName("Custom/BrickTiles");


	// Set Shadow Technique
	_scene_manager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

	// Add Directional Light
	_scene_manager->setAmbientLight(Ogre::ColourValue(0.2f, 0.5f, 0.8f));
	Light* directionalLight = _scene_manager->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(1, 1, 1);
	directionalLight->setSpecularColour(.4, .4, .4);
	SceneNode* directionalLightNode = _scene_manager->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(Ogre::Vector3(0, -1, -1));

	PickupManager::initialize(_scene_manager, _player->getEntityNode());
	PickupManager::addPickupObject("cube.mesh");
}

bool MyEngine::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
	{
		getRoot()->queueEndRendering();
	}
	return true;
}

bool MyEngine::frameStarted(const Ogre::FrameEvent& evt)
{
	// Main "game loop" of the application
		// Let parent handle this callback as well
	ApplicationContext::frameStarted(evt);
	// Store the time that has passed since last time we got the callback
	const Ogre::Real delta_time = evt.timeSinceLastFrame;
	// Check what keys of the keyboard are being pressed
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	// Update any subsystems
	if (_player != nullptr) _player->Update(delta_time, state);
	if (_roaming_camera != nullptr) _roaming_camera->update(delta_time, state);

	// Update all the managed pickup objects
	PickupManager::Update(delta_time, state);

	return true;
}