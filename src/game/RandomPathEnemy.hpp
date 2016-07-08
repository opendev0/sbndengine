#ifndef RANDOMPATHENEMY_HPP
#define RANDOMPATHENEMY_HPP

class RandomPathEnemy : public Enemy
{
public:
	RandomPathEnemy(const CVector<3, float> &min, const CVector<3, float> &max, iRef<iPhysicsObject> physicsObject, const iEngine &engine) : Enemy(physicsObject, engine), min(min), max(max)
	{
		srand(time(NULL));
		curGoal = randomGoal();
	}

	void move()
	{
		float sqDist = (curGoal - getPosition()).getLength2();
		if (sqDist < 0.001) {
			curGoal = randomGoal();
		}

		CVector<3, float> pathVector = (curGoal - getPosition()).getNormalized();
		physicsObject->object->position += (pathVector * velocity) * engine.time.frame_elapsed_seconds;
		physicsObject->object->updateModelMatrix();
	}

private:
	CVector<3, float> min;
	CVector<3, float> max;
	CVector<3, float> curGoal;

	CVector<3, float> randomGoal()
	{
		return CVector<3, float>();
	}
};

#endif // RANDOMPATHENEMY_HPP
