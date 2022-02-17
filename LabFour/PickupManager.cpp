#include "pch.h"
#include "PickupManager.h"

template<> PickupManager* Ogre::Singleton<PickupManager>::msSingleton = 0;

// need to declare the static variables, otherwise we get a link error
SceneManager* PickupManager::scene_manager_;
SceneNode* PickupManager::player_node_;
std::list<IPickupObject*> PickupManager::pickup_objects;

PickupManager::PickupManager(){ }

PickupManager::~PickupManager(){ }

bool PickupManager::initialize(SceneManager* sceneManager, SceneNode* playerNode)
{
    if (msSingleton == NULL)
    {
        msSingleton = OGRE_NEW PickupManager;
        if (false == msSingleton->_initialize(sceneManager, playerNode))
        {
            OGRE_DELETE msSingleton;
            msSingleton = NULL;
            return false;
        }
    }

    return true;
}

void PickupManager::destroy()
{
    if (msSingleton != NULL)
    {
        msSingleton->_destroy();

        OGRE_DELETE msSingleton;
        msSingleton = NULL;
    }
}

PickupManager& PickupManager::getSingleton()
{
	assert(msSingleton);
	return (*msSingleton);
}

PickupManager* PickupManager::getSingletonPtr()
{
	return msSingleton;
}

Ogre::Vector3 PickupManager::getRandomPosition()
{
    random_device rd;                           // obtain a random number from hardware
    mt19937 gen(rd());                          // seed the generator
    uniform_real_distribution<> distr(-50, 50); // define the range

    // Create a new pickup Object in a random position near the player
    Ogre::Vector3 playerPosition = player_node_->getPosition();
    Real randomXOffset = distr(gen);
    Real randomZOffset = distr(gen);
    return Ogre::Vector3(playerPosition.x + randomXOffset, -3.0f, playerPosition.z + randomZOffset);
}


/** This function spawns a new pickup object at a random position near the player */
void PickupManager::addPickupObject(const char* mesh_file_name)
{
    Ogre::Vector3 newSpawnPosition = getRandomPosition();
    PickupObject* pickupObject = new PickupObject(scene_manager_, mesh_file_name, newSpawnPosition, Vector3(1.0, 1.0, 1.0));
    pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();


    // Set the color of the object
    String MaterialName = pickupObject->getEntity()->getMesh()->getSubMesh(0)->getMaterialName();
    MaterialPtr pMaterial = MaterialManager::getSingleton().getByName(MaterialName);
    
    float r = 255.0f / 255;
    float g = 215.0f / 255;
    float b = 0.0f / 255;

    ColourValue* color = new ColourValue(r, g, b, 1.0F);
    
    pMaterial->setDiffuse(*color);
    pMaterial->setAmbient(*color);
    pMaterial->setSpecular(*color);


    // Insert the new Pickup Object in the list of managed objects
    pickup_objects.push_back(pickupObject);
}

void PickupManager::Update(Ogre::Real delta_time, const Uint8* state)
{
    // Update all the managed pickup objects, and delete them if they finished the effect
    for (auto i = pickup_objects.begin(); i != pickup_objects.end();) {
        bool erased = false;
        IPickupObject* pickupObject = *i;

        pickupObject->update(delta_time);

        // Check for collision with a game object that has not yet been picked up
        if (pickupObject->collidesWith(player_node_, player_node_->getPosition().distance(pickupObject->getSceneNode()->getPosition())))
        {
            if (!pickupObject->isPickedUp())
            {
                pickupObject->getSceneNode()->getParentSceneNode()->removeChild(pickupObject->getSceneNode());
                player_node_->addChild(pickupObject->getSceneNode()); // Make child of player
                pickupObject->getSceneNode()->setPosition(Ogre::Vector3(0,0,0)); // Reference the same position
                pickupObject->runPickupEffect();
            }
        }

        if (pickupObject->isPickedUp())
        {
            // If the effect is finished we can dispose of the object
            if (pickupObject->getPickupEffect()->isFinished())
            {
                player_node_->removeChild(pickupObject->getSceneNode());
                scene_manager_->getRootSceneNode()->addChild(pickupObject->getSceneNode());
                pickupObject->reset(getRandomPosition());
                //pickupObject->getEntity()->getParentSceneNode()->detachObject(pickupObject->getEntity());
                //scene_manager_->destroyEntity(pickupObject->getEntity());
                //i = pickup_objects.erase(i);
                //erased = true;

            }
        }

        // Don't increase the counter if we have deleted an item, otherwise it throws an error
        //if (!erased) ++i;
        i++;
    }
}

bool PickupManager::_initialize(SceneManager* sceneManager, SceneNode* playerNode)
{
    OGRE_LOCK_AUTO_MUTEX;
    // Do here initialization stuff if needed
    scene_manager_ = sceneManager;
    player_node_ = playerNode;

    return true;
}

void PickupManager::_destroy()
{
    OGRE_LOCK_AUTO_MUTEX;
    // Do here destruction stuff if needed
    OGRE_DELETE scene_manager_;
    scene_manager_ = NULL;
    OGRE_DELETE player_node_;
    player_node_ = NULL;
    OGRE_DELETE &pickup_objects;
}
