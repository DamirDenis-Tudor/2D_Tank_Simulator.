#include "Bullet.h"

Bullet::Bullet(string type, string impactAnim, int damage, Vector2T<int> position, float angle, int tankId) 
	: _position(position), _tankId(tankId)  , _angle(angle), _damage(damage) , _impactAnim(impactAnim)
{
	_sprite = new SpriteComponent(AssetsStorage::_movebles[{type, "bullet"}]);
	_sprite->setPosition(position - _sprite->_dest->w / 2 - CameraManager::offset);
	_sprite->_angle = angle;
}

Bullet::~Bullet()
{
	_sprite->setSrcTextNullPtr();
	delete _sprite;
	_sprite = nullptr;
}

void Bullet::update()
{
	Vector2T<float> potentialPos;
	potentialPos._x = _position._x;
	potentialPos._y = _position._y;

	potentialPos._x += _velocity._x * SDL_cos((_sprite->_angle - 90) * M_PI / 180) * TimeManager::getDeltaTime();
	potentialPos._y += _velocity._y * SDL_sin((_sprite->_angle - 90) * M_PI / 180) * TimeManager::getDeltaTime();

	bool hasCollision = false;

	Vector2T<int> mapColliderObject;
	if (CollisionManager::pointCollisionMap(potentialPos , mapColliderObject))
	{
		Mediator::registerHit(Mediator::getId(mapColliderObject) , _damage);
		hasCollision = true;
	}

	/*
		coliziuni bullet - tank
	*/
	int rectDim = 2 * AssetsStorage::_tileDim;
	for (auto& i : Mediator::recieveTanksPositions(_tankId))
	{
		Vector2T<int> rectPos = i ;
		if (CollisionManager::pointCollisionRectagle(potentialPos, rectPos, rectDim))
		{
			hasCollision = true;
			if (!Mediator::checkTeammates(_tankId, Mediator::getId(i)))
			{
				Mediator::registerHit(Mediator::getId(i), _damage);
			}
		}
	}

	if (hasCollision)
	{
		disable();
	}
	else
	{
		_position._x = static_cast<int>(potentialPos._x);
		_position._y = static_cast<int>(potentialPos._y);

	}
	_sprite->setPosition(_position - CameraManager::offset - _sprite->_dest->w / 2);
}

void Bullet::draw()
{
	_sprite->draw();
}
