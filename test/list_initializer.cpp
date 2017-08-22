#include <iostream>
#include <vector>

class Tree{
	public:
			Tree( float x_, double y_, int z_, std::string name_) : x(x_), y(y_), z(z_), name(name_){}

			void Echo()  const
			{
				std::cout << "x=" << x << ", y=" << y << ", z=" << z << ",name=" << name << '\n';	
			}

	private:
			float				x;
			double			y;
			int					z;
			std::string	name;
};

int main()
{
	Tree t{3.0f, 3.14, 8, "first tree"};	
	t.Echo();
	std::vector<Tree> trees{{4.0f, 4.14, 9, "second tree"}};
	trees.push_back({5.0f, 5.14, 10, "third tree"});
	for( auto const& t : trees){
			t.Echo();
	}

	return 0;
}
