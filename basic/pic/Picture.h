#ifndef PICTURE_H_
#define PICTURE_H_

#include <iostream>

class Picture
{
	friend std::ostream& operator<< (std::ostream&, const Picture&);
	friend Picture frame(const Picture&);
	friend Picture operator| (const Picture&, const Picture&);
	friend Picture operator& (const Picture&, const Picture&);

	public:
	Picture();
	Picture(const char* const*, unsigned int);
	Picture(const Picture&);
	~Picture();

	Picture& operator= (const Picture&);

	private:
	unsigned int width;
	unsigned int height;
	char* data;

	char& pixelAt(unsigned int row, unsigned int col);
	char pixelAt(unsigned int row, unsigned int col) const;
	static unsigned int max(unsigned int left, unsigned int right);
	void init(unsigned int w, unsigned int h);
	void copyBlock(unsigned int x, unsigned int y, const Picture& pic);
 	void fillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const char ch);	
};

std::ostream& operator<< (std::ostream&, const Picture&);

Picture operator| (const Picture&, const Picture&);

Picture operator& (const Picture&, const Picture&);

Picture frame(const Picture&);

#endif

