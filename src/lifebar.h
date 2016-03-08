#include "model.h"

class LifeBar : public Model {
	public:
		LifeBar();
		~LifeBar();
		void draw(Video&);
	private:
		float _value;
};
