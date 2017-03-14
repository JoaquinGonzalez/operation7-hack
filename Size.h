#ifndef SIZE_HEADER
#define SIZE_HEADER

class Size
{
public:
	int width;
	int height;

	Size()
	{
	}

	Size(int width, int height)
	{
		this->width = width;
		this->height = height;
	}
};

#endif