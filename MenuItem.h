#ifndef MENUITEM_HEADER
#define MENUITEM_HEADER

class MenuItem
{
public:
	char* name;
	int index;
	bool actived;
	bool option;
	bool group;
	int groupItems;

	MenuItem()
	{
	}

	MenuItem(char* name, int index, bool actived = false, bool option = false, bool group = false, int groupItems = 0)
	{
		this->name = name;
		this->index = index;
		this->actived = actived;
		this->option = option;
		this->group = group;
		this->groupItems = groupItems;
	}
};

#endif