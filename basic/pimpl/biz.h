#include "pimpl.h"

class BizImpl;

class Biz : public cmn::Pimpl<BizImpl>
{
	public:
		// must forward declare here, b' BizImpl is incomplete currently 
		// while the default constructor requires a complete type
		Biz();

		void DoWork();
};
