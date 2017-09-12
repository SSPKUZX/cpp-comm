#include <string>
#include <iostream>

class Chainer{
	public:
			Chainer& Attr(const std::string& attr_){
				std::cout << m_name << " got registered attr:		" << attr_ << '\n';	
				return *this;
			}

			Chainer& Input(const std::string& input_){
				std::cout << m_name << " got registered input:	" << input_ << '\n';	
				return *this;
			}

			Chainer(std::string const& name_) : m_name(name_){}

	private:
			std::string m_name;
};

#define REG_CHAIN(name) \
static Chainer chainer_##__COUNTER__ = Chainer(name)

REG_CHAIN("chain_test")
	.Attr("attr_1").Input("input_1");

int main(){

	return 0;				
}
