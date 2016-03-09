#include "model2d.h"
#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"

#define BAR_HEIGHT 50

class Video;

class LifeBar : public Model2D {
	public:
		LifeBar(glm::vec2);
		~LifeBar();
		float getValue();
		void setValue(float);
	private:
		float _value;
};
