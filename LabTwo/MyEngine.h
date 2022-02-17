#pragma once
#include "pch.h"
#include "PlayerAvatar.h"

using namespace Ogre;
using namespace OgreBites;

class MyEngine : public ApplicationContext, public InputListener
{
public:
	MyEngine();
	virtual ~MyEngine() {};
	void setup();
	bool keyPressed(const OgreBites::KeyboardEvent& evt);
	void setupSceneManager();
	void setupCamera();
	void populateScene();
	bool frameStarted(const Ogre::FrameEvent& evt);
private:
	Ogre::Root* _root;
	Ogre::SceneManager* _scene_manager;
	Ogre::Camera* _camera;
	Ogre::SceneNode* _camera_node;
	PlayerAvatar* _player;
};
