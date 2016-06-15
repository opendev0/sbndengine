#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    iPhysicsObject *object;
    cCamera3rdPerson *camera;
	float velocity;

public:
	Player(iPhysicsObject &object, cCamera3rdPerson& camera, float velocity) : velocity(velocity)
	{
		this->object = &object;
		this->camera = &camera;
	}
	~Player();

    startMove(eDirection dir)
	{
		switch (dir) {
			case FORWARDS:
				object
				break;
		}
	}
};

#endif // PLAYER_H
