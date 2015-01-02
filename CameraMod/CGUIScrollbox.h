#ifndef C_GUISCROLLBOX
#define C_GUISCROLLBOX
class CGUIScrollbox
{
private:
	int x, y, width, height;
	int shift;
	float size;
	int sizeOverall;
	char test[500];
public:
	CGUIScrollbox();
	void	SetUp(int x, int y, int width, int height);
	void	Update(int overallSize);
	void	Render();

	int		GetShift();

	void	OnKey(int VK);
	void	OnMouseMove(int x, int y);
};

#endif