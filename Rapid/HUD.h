
class HUD 
{
public:
	HUD();
	~HUD();
	void renderBitmapString (float x, float y, float z, char *string);
	void renderGlutAimer(float px, float py, float dx, float dy);
	void drawText(int fps, int curTime, int health);
};
