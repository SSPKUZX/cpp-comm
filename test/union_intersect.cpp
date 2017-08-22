#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

vector<string> read_from(std::string const& file_path)
{
	fstream input(file_path);
	if(input) cout << "succ in reading " << file_path << '\n';
	else cout << "fail in reading " << file_path << '\n'; 
	vector<string> ips;
	string line;
	while( input >> line )
	{
		ips.push_back(line);
	}
	input.close();
	sort(ips.begin(), ips.end());

	return std::move(ips);
}


int main( int argc, char* argv[])
{
	if(argc!=3){
		cout<< "please use it like this: ui path1 path2\n";
		return 0;
	}
	auto tf = read_from(argv[1]);	
	auto mr = read_from(argv[2]);	

	vector<string> intersect, diff; 
	cout << "size of " << argv[1] << " is " << tf.size() << '\n';
	cout << "size of " << argv[2] << " is " << mr.size() << '\n';
	set_intersection(tf.begin(), tf.end(), mr.begin(), mr.end(), back_inserter(intersect));
	cout << "size of insect =" << intersect.size() << '\n';
	set_difference(tf.begin(), tf.end(), intersect.begin(), intersect.end(), back_inserter(diff));	
	cout << "size of diff =" << diff.size() << '\n';

	copy(diff.begin(), diff.end(), ostream_iterator<string>(cout, "\n"));

	return 0;
}
