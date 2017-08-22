#include <utility>

namespace cmn
{

	template<class Resource, class UnlockPolicy>
	class Guard
	{
		template<class _Resource, class _LockPolicy, class _UnlockPolicy>
		friend Guard<_Resource, _UnlockPolicy> make_guard( _Resource& res, _LockPolicy&& locker, _UnlockPolicy&& unlocker);

		public:
		template<class LockPolicy>
		Guard( Resource& res, LockPolicy&& locker, UnlockPolicy&& unlocker)
		: m_resource( res),m_unlocker( std::move(unlocker))
		{
			locker(res);	
		}

		~Guard()
		{
			m_unlocker( m_resource);	
		}

		// only movable 
		Guard( Guard&&)					= default;

		Guard()							= delete;
		Guard( Guard const&)			= delete;
		Guard& operator=( Guard const&)	= delete;
		Guard& operator=( Guard&&)		= delete;

		private:
		Resource&		m_resource;
		UnlockPolicy	m_unlocker;	
	};

	template<class Resource, class LockPolicy, class UnlockPolicy>
	auto make_guard( Resource&& res, LockPolicy&& locker, UnlockPolicy&& unlocker)
	->Guard< Resource,UnlockPolicy>
	{
		//	static_assert(!std::is_const<Resource>::value && std::is_lvalue_reference<Resource>::value, "Resource should be lvalue_reference but not const");
		//	static_assert(std::is_convertible<LockPolicy,	std::function<void(Resource)> >::value, "LockPolicy should be a functional class with only one Resource parameter");
		//	static_assert(std::is_convertible<UnlockPolicy,	std::function<void(Resource)> >::value, "UnlockPolicy should be a functional class with only one Resource parameter");

		return Guard<Resource,UnlockPolicy>( std::forward<Resource>(res), std::forward<LockPolicy>(locker), std::forward<UnlockPolicy>(unlocker) );
	}

}
