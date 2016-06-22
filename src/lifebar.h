#include "model2d.h"

#define BAR_HEIGHT 50

class Video;

class LifeBar : public Model2D {
	public:
		LifeBar(glm::vec2);
		~LifeBar();
		float getValue();
		void setValue(float);
		void draw(Video&);
	private:
		float _value;
		Model2D _back;
};
