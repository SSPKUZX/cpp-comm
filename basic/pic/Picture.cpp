#include <iostream>
#include "Picture.h"

Picture::Picture() : width(0), height(0), data(0){}

Picture::Picture(const char* const* raw, unsigned int rows)
{
	int w = 0;
	int r;
	for(r = 0; r < rows; ++r)
		w = Picture::max(w, strlen(raw[r]));
	
	init(w, rows);

	for(r = 0; r < rows; ++r)
	{
		const char* rowPixels = raw[r];
		int len = strlen(rowPixels);
		int col = 0;
		while( col < len)	
		{
			pixelAt(r, col) = rowPixels[col];
			++col;
		}
		while(col < width)
		{
			pixelAt(r, col) = ' ';
			++col;
		}
	}
}

Picture::Picture(const Picture& rhs)
{
	init(rhs.width, rhs.height);
	copyBlock(0, 0, rhs);
}

Picture::~Picture()
{
	delete []data;
}

Picture& Picture::operator= (const Picture& rhs)
{	
	if(this != &rhs)
	{
		delete[] data;
		init(rhs.width, rhs.height);
		copyBlock(0, 0, rhs);
	}
	return *this;
}

char& Picture::pixelAt(unsigned int row, unsigned int col)
{
	return data[row * width + col];
}

char Picture::pixelAt(unsigned int row, unsigned int col) const
{
	return data[row * width + col];
}

unsigned int Picture::max(unsigned int left, unsigned int right)
{
	return left > right ? left : right;
}

void Picture::init(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
	data = new char[w * h];
}

void Picture::copyBlock(unsigned int x, unsigned int y, const Picture& pic)
{
	for(int r = 0; r < pic.height; ++r)
		for(int c = 0; c< pic.width; ++c)
		{
			pixelAt(y + r, x + c) = pic.pixelAt(r, c);
		}
}

std::ostream& operator<< (std::ostream& out, const Picture& pic)
{
	for(int r = 0; r < pic.height; ++r)
	{
		for(int c = 0; c < pic.width; ++c)
			out << pic.pixelAt(r, c);
		out << std::endl;
	}

	return out;
}

Picture frame(const Picture& pic)
{
	Picture p;

	p.init(pic.width + 2, pic.height + 2 );
	p.copyBlock(1, 1, pic);

	const char hor = '-', ver = '|', star = '*';
	for(int c = 1; c < p.width - 1; ++c)
	{
		p.pixelAt(0, c) = hor;
		p.pixelAt(p.height-1, c) = hor;
	}
	for(int r = 1; r< p.height - 1; ++r)
	{
		p.pixelAt(r, 0) = ver;
		p.pixelAt(r, p.width - 1) = ver;
	}

	p.pixelAt(0, 0) = star;
	p.pixelAt(0, p.width - 1) = star;
	p.pixelAt(p.height - 1, 0) = star;
	p.pixelAt(p.height - 1, p.width -1) = star;

	return p;
}
	
void Picture::fillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h,const char ch)
{
	for(int r = 0; r < h; ++r)
		for(int c = 0; c < w; ++c)
			pixelAt(y + r, x + c) = ch;
}

Picture operator| (const Picture& left, const Picture& right)
{
	Picture p;
	
	p.init(left.width + right.width, Picture::max(left.height, right.height));
	p.copyBlock(0, 0 ,left);
	p.copyBlock(left.width, 0,  right);
	
	const char space = ' ';
	if(left.height < right.height)
	{
		p.fillRect(0, left.height, left.width, p.height - left.height, space);
	}
	else
	{
		p.fillRect(left.width, right.height, right.width, p.height - right.height, space);
	}

	return p;
}

Picture operator& (const Picture& upper, const  Picture& lower)
{
	Picture p;

	p.init(Picture::max(upper.width, lower.width), upper.height + lower.height);
	p.copyBlock(0, 0, upper);
	p.copyBlock(0, upper.height, lower);

	const char space  = ' ';
	if(upper.width < lower.width)
	{
		p.fillRect(upper.width, 0, p.width - upper.width, upper.height, space);
	}
	else
	{
		p.fillRect(lower.width, upper.height, p.width - lower.width, lower.height, space);	
	}

	return p;
}	

