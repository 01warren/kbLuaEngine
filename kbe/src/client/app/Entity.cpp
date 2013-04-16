#include "Entity.h"
#include "space_world.h"
#include "DotSceneLoader.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "../kbengine_dll/kbengine_dll.h"

//-------------------------------------------------------------------------------------
void KBEntity::addTime(Real deltaTime)
{
	if(mBodyNode)
	{
		updateBody(deltaTime);
		updateAnimations(deltaTime);
	}

	if(mCamera)
		updateCamera(deltaTime);

	if(!isJump())
	{
		Ogre::Vector3 camPos = getPosition();
		Ogre::Ray ray;
		ray.setOrigin(Ogre::Vector3(camPos.x, 10000, camPos.z));
		ray.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

		Ogre::TerrainGroup::RayResult rayResult = mSpacePtr->getDotSceneLoader()->getTerrainGroup()->rayIntersects(ray);
		Ogre::Real distanceAboveTerrain = CHAR_HEIGHT;
		Ogre::Real fallSpeed = 200;
		Ogre::Real newy = camPos.y;
		if (rayResult.hit)
		{
			if (camPos.y > rayResult.position.y + distanceAboveTerrain)
			{
				mFallVelocity += deltaTime * 10;
				mFallVelocity = std::min(mFallVelocity, fallSpeed);
				newy = camPos.y - mFallVelocity * deltaTime;
			}
			newy = std::max(rayResult.position.y + distanceAboveTerrain, newy);
			setPosition(camPos.x, newy, camPos.z);
		}
	}

	if(kbe_playerID() != mID)
	{
		Ogre::Vector3 currpos = getPosition();
		Ogre::Vector3 movement = destPos_ - currpos;
		
		movement.y = 0.f;

		if(movement.length() < mMoveSpeed)
		{
			currpos = destPos_;
		}
		else
		{
			movement.normalise();

			// �ƶ�λ��
			movement *= mMoveSpeed;
			currpos += movement;
		}
		
		setPosition(currpos.x, currpos.y, currpos.z);
	}
}

//-------------------------------------------------------------------------------------
bool KBEntity::_checkJumpEnd()
{
	Ogre::Vector3 camPos = getPosition();
	Ogre::Ray ray;
	ray.setOrigin(Ogre::Vector3(camPos.x, 10000, camPos.z));
	ray.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

	Ogre::TerrainGroup::RayResult rayResult = mSpacePtr->getDotSceneLoader()->getTerrainGroup()->rayIntersects(ray);

	if (rayResult.hit)
	{
		if (camPos.y <= rayResult.position.y + CHAR_HEIGHT)
		{
			return true;
		}
	}

	return false;
}
