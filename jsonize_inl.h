#pragma once

#include "call.h"

namespace json
{

	namespace impl
	{
		class SerInvoker
		{
			public:
				SerInvoker(std::type_index const& type) : m_type(type), m_iNameIndex(0){}
	
				template< class _Property>
				bool operator() ( _Property const& property)
				{
					// just for display here, 
					// real serialization needs json lib support like following
					// return obj2json( property, json[ m_vecPropNames[m_iNameIndex++] ] );
					m_strJson += "key=" + g_type2info[m_type][m_iNameIndex++] + "\t";
					return obj2json( property, m_strJson ); 
				}
				
				json_type const& get() { return m_strJson;}
	
			private:
				std::type_index		m_type; 
				uint8_t				m_iNameIndex;
				json_type			m_strJson;
		};
	
		class DesInvoker
		{
			public:
				DesInvoker( json_type const& json, std::type_index const& type) : m_strJson( json), m_type( type), m_iNameIndex(0) {}
	
				template< class _Property>
				bool operator() ( _Property& property)
				{
					// just for display		
					// real deserialization needs json lib support like following
					// return json2obj( m_strJson[g_type2info[m_type][m_iNameIndex++]], property);
					return json2obj( m_strJson, property);
				}
	
			private:
				json_type			m_strJson;
				std::type_index		m_type;
				uint8_t				m_iNameIndex;
		};
	
	} // end of impl

	template<class... _Args>
	bool ser( json_type& json, std::type_index const& type, _Args const&... args )
	{
		impl::SerInvoker invoker(type);	
		auto isSucc = comm::call_each( invoker, args... );
		// error msg should be logged if failed
		return isSucc ? json = invoker.get(), isSucc : isSucc ;
	}

	template<class... _Args>
	bool des( json_type const& json, std::type_index const& type, _Args&... args)
	{
		impl::DesInvoker invoker(json, type);	
		// error msg should be logged if failed
		return comm::call_each( invoker, args... );
	}

} // end of json
