#include "pch.h"
#include "MyEngine.h"

MyEngine::MyEngine() : ApplicationContext("T-637-GEDE Lab 2")
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
	_camera_node = _scene_manager->getRootSceneNode()->createChildSceneNode();
	_camera = _scene_manager->createCamera("MainCamera");

	_camera->setNearClipDistance(5);
	_camera->setAutoAspectRatio(true);

	_camera_node->setPosition(0, 10, 50);
	_camera_node->lookAt(Ogre::Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
	_camera_node->attachObject(_camera);

	Viewport* vp = getRenderWindow()->addViewport(_camera);
	_camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// To match the BG color with the fog
	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);
	vp->setBackgroundColour(fadeColour);

	// Add fog that starts 50 units away from the camera, ends at 300 units, and increases in a linear fashion
	_scene_manager->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 50, 300);
}

void MyEngine::populateScene()
{
	_player = new PlayerAvatar(_scene_manager, _camera_node, "Sinbad.mesh");
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
	ApplicationContext::frameStarted(evt);
	_player->Update(evt.timeSinceLastFrame);
	return true;
}