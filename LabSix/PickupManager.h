#pragma once
#include "pch.h"
#include "OgreSingleton.h"
#include "PickupObject.h"
#include <iostream>
#include <random>

using namespace std;
using namespace Ogre;
using namespace OgreBites;

class PickupManager : public Singleton<PickupManager>
{
    public:
        PickupManager(PickupManager const&) = delete;
        void operator=(PickupManager const&) = delete;
        /**
        Initialize the Pickup Manager.
        Return true upon success.
        */
        static bool initialize(SceneManager* sceneManager, SceneNode* playerNode);

        /**
        Destroy the Pickup Manager.
        */
        static void destroy();


        /** Override standard Singleton retrieval. */
        static PickupManager& getSingleton();


        /// @copydoc Singleton::getSingleton()
        static PickupManager* getSingletonPtr();

        static void addPickupObject(const char* mesh_file_name, Vector3 position);

        static void createPickupTower();

        static void _initialize_tower_parameters();


        static void Update(Ogre::Real delta_time, const Uint8* state);

        static void _create_pickup_tower_layer();

        static void _destroy_pickup_tower();

        static Ogre::Vector3 getRandomPosition();

    protected:
        static SceneManager* scene_manager_;
        static SceneNode* player_node_;
        static std::list<IPickupObject*> pickup_objects;
        static bool create_pickup_tower;
        static float tower_spawn_period;
        static float tower_spawn_timer;
        static float tower_width;
        static float tower_length;
        static float tower_current_height;
        static float tower_max_layers;
        static float tower_current_layers;
        static float tower_instantiation_milliseconds;

        /** Class default constructor */
        PickupManager();

        /** Class destructor */
        ~PickupManager();

        /** Initialize the pickup manager instance. */
        bool _initialize(SceneManager* sceneManager, SceneNode* playerNode);

        /** Destory the pickup manager instance. */
        void _destroy();
};

