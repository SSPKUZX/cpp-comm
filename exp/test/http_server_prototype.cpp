#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

namespace jsn
{
	template< class ObjType>
	int32_t json2obj( std::string const& json_str, ObjType& obj)
	{
		obj = json_str;	
		return 0;
	}

	template<>
	int32_t json2obj( std::string const& json_str, int& obj)
	{
		obj = 9;	
		return 0;
	}

	template< class ObjType>
	int32_t obj2json( ObjType const& obj,  std::string& json_str)
	{
		json_str = obj;	
		return 0;
	}

	template<>
	int32_t obj2json( std::string const& obj,  std::string& json_str)
	{
		json_str = obj + "_json_converted";	
		return 0;
	}
}

namespace htp
{
	template<class BizCore>
	class HttpServer final
	{
		public:
			template<class... Args>
			HttpServer( Args&&... args) : m_biz_core( std::forward<Args>(args)...) {}

			template<class ReqType, class RspType>
			void RegisterBiz( std::string const& biz_url, int32_t(BizCore::*mfn)( ReqType const&, RspType&) )
			{
				m_url2biz.emplace( biz_url, 
						[mfn,this]( std::string const& biz_req, std::string& biz_rsp)	
						{
							ReqType req;
							RspType rsp;
							return ( jsn::json2obj( biz_req, req) == 0 
								&& std::mem_fn(mfn)(&m_biz_core, req, rsp) == 0 
								&& jsn::obj2json( rsp, biz_rsp) == 0 )	? 0 : -1;
						}						
					);	
			}

			int32_t	operator()( std::string const& biz_url, std::string const& biz_req, std::string& biz_rsp)
			{
				return m_url2biz[biz_url]( biz_req, biz_rsp);	
			}

		private:	
			BizCore		m_biz_core;
			std::unordered_map<std::string, std::function<int32_t(std::string const&, std::string&)>>	m_url2biz;
	};
}

namespace tst
{
	class AppMasterHttpCore
	{
		public:	
			int32_t RegisterAM( int const& reg_req, std::string& reg_rsp)		
			{
				reg_rsp	= "966";
				std::cout << "the converted req is=" << reg_req << "\t the response is=" << reg_rsp << '\n';	
				return 0;
			}
	};
}

using namespace tst;
using namespace htp;

int main()
{
	HttpServer<AppMasterHttpCore>	httpServer;		
	httpServer.RegisterBiz("/amp/regam", &AppMasterHttpCore::RegisterAM);	

	std::string rsp_str;
	auto ret = httpServer("/amp/regam", "regam_req", rsp_str);
	std::cout << "ret=" << ret << "\t rsp=" << rsp_str << '\n';
	
	return 0;
}
