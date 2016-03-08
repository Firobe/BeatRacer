#include "model.h"

class LifeBar : public Model {
	public:
		LifeBar();
		~LifeBar();
		void draw(Video&);
		float getValue();
		void setValue(float);
	private:
		float _value;
};
