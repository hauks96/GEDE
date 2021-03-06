#include "pch.h"
#include "PickupObject.h"

PickupObject::PickupObject(short allocation_mode, SceneManager* scene_manager, const char* mesh_file_name,
	Vector3 position, Vector3 scale)
{
	_initialize(scene_manager, mesh_file_name, position, scale);
	array_size = 100000;
	this->allocation_mode = allocation_mode;

	switch (this->allocation_mode)
	{
	case 0:
	{
		// C++ NEW
		for (auto i = 0; i < array_size; i++)
		{
			arr[i] = new int;
		}
		break;
	}
	case 1:
	{
		// OGRE NEW
		for (auto i = 0; i < array_size; i++)
		{
			arr[i] = OGRE_NEW(int);
		}
		break;
	}
	case 2:
	{
		// STACK ALLOCATOR
		stack_allocator = new StackAllocator();
		stack_allocator->init(array_size * sizeof(int));

		for (auto i = 0; i < array_size; i++)
		{
			arr[i] = (int*)stack_allocator->allocate(sizeof(int));
		}
		break;
	}

	default:
		break;
	}
}

PickupObject::~PickupObject()
{
	if (pickup_effect_ != nullptr) delete(pickup_effect_);

	switch (this->allocation_mode)
	{
	case 0:
	{
		// C++ DELETE
		for (auto i = 0; i < array_size; i++)
		{
			if (arr[i] != nullptr)
			{
				delete arr[i];
				arr[i] = nullptr;
			}
		}
		break;
	}
	case 1:
	{
		// OGRE DELETE
		for (auto i = 0; i < array_size; i++)
		{
			if (arr[i] != nullptr)
			{
				OGRE_DELETE(arr[i]);
				arr[i] = nullptr;
			}
		}
		break;
	}
	case 2:
	{
		// STACK ALLOCATOR DELETE
		stack_allocator->reset();
		delete stack_allocator;
		break;
	}

	default:
		break;
	}
	delete arr;
}

SceneNode* PickupObject::getSceneNode() const
{
	return entity_node_;
}

IPickupEffect* PickupObject::getPickupEffect() const
{
	return pickup_effect_;
}

Entity* PickupObject::getEntity() const
{
	return entity_;
}

bool PickupObject::isPickedUp() {
	return picked_up_;
}

bool PickupObject::collidesWith(SceneNode* other_node, float distance) {
	bool collision = false;
	Ogre::AxisAlignedBox object_bb = entity_node_->_getWorldAABB();
	Ogre::AxisAlignedBox other_bb = other_node->_getWorldAABB();
	return object_bb.intersects(other_bb);
}

void PickupObject::runPickupEffect()
{
	delete pickup_effect_;
	pickup_effect_ = new SwirlEffect(entity_node_, Ogre::Vector3(5.0, -5, 50.0));
	pickup_effect_->run();
	picked_up_ = true;
}

void PickupObject::update(float delta_time) const
{
	if (pickup_effect_ != nullptr) {
		if (pickup_effect_->isRunning()) pickup_effect_->update(delta_time);
	}
}

void PickupObject::reset(Ogre::Vector3 position)
{
	picked_up_ = false;
	entity_node_->setPosition(position);
}

void PickupObject::_initialize(SceneManager* scene_manager,
	const char* mesh_file_name, Vector3	position, Vector3 scale)
{
	scene_manager_ = scene_manager;
	entity_ = scene_manager_->createEntity(mesh_file_name);
	entity_node_ = scene_manager_->getRootSceneNode()->createChildSceneNode();
	entity_node_->attachObject(entity_);
	entity_node_->setPosition(position);
	entity_node_->setScale(scale);
	entity_->setCastShadows(true);
	pickup_effect_ = nullptr;
	picked_up_ = false;
	array_size = 100000;
	arr = new int*[array_size];
}