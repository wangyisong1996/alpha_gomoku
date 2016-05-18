#ifndef __BASIC_AI_H__
#define __BASIC_AI_H__

class BasicAI {
	public:
	BasicAI() {}
	virtual ~BasicAI() {}
	
	// *_PLAYER
	virtual void init(int player) = 0;
	
	virtual void opponent_step(int x, int y, int player) = 0;
	
	// return false if you wanna say GG
	virtual bool get_step(int &x, int &y) = 0;
};

#endif
