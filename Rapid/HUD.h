
class HUD 
{
public:
	HUD();
	~HUD();
	void renderBitmapString (float x, float y, float z, char *string);
	void drawText(int fps, int curTime);
};
