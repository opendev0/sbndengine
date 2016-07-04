#ifndef PATHENEMY_HPP
#define PATHENEMY_HPP

#include <vector>
#include "Enemy.hpp"

class PathEnemy : public Enemy
{
public:
	PathEnemy(iRef<iPhysicsObject> &physicsObject, const iEngine &engine) : Enemy(physicsObject, engine)
	{}

	void move()
	{
		if (path.empty()) return;

		float sqDist = (path.at(curNodeIndex) - getPosition()).getLength2();
		if (sqDist < 0.001) {
			curNodeIndex = (curNodeIndex + 1 == path.size()) ? 0 : (curNodeIndex + 1);
		}

		CVector<3, float> pathVector = (path.at(curNodeIndex) - getPosition()).getNormalized();
		physicsObject->object->position += (pathVector * velocity) * engine.time.frame_elapsed_seconds;
		physicsObject->object->updateModelMatrix();
	}

	void addCheckpoint(CVector<3, float> pos, int index = -1)
	{
		if (index < 0 || path.size() == 0) {
			path.push_back(pos);
			return;
		}

		int i = 0;
		for (std::vector<CVector<3, float>>::iterator it = path.begin(); it != path.end(); ++it) {
			if (i == index) {
				path.insert(it, pos);
				return;
			}

			++i;
		}
	}

protected:
	std::vector<CVector<3, float>> path;
	size_t curNodeIndex = 0;
};

#endif // PATHENEMY_HPP
