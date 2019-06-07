#ifndef LINE_H
#define LINE_H

namespace game_framework {
	class Line
	{
	public:
		Line(unsigned int);
		unsigned int getWatch();
		unsigned int setWatch();
		~Line();
	private:
		unsigned int _operation;
		unsigned int _watch;
		unsigned int _count;
	};
}

#endif 