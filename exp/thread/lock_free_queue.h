#pragma once 

/* this lock free queue is only suitable for one producer and one consumer 
 * for the Peek and IsEmpty function, they can't guarantee semantically correct result, 
 * thought this won't crash the application
*/
 
#include <list>

namespace utl 
{
	template <typename T>
	class LockFreeQueue
	{
	    public:
			LockFreeQueue()
			{
				list.push_back(T());//分割节点
				iHead = list.begin();
				iTail = list.end();
			};

			LockFreeQueue( LockFreeQueue const&) = delete;
			LockFreeQueue& operator=( LockFreeQueue const&) = delete;
		
		 	void Produce(const T& t) //存消息
			{
				list.push_back(t);
				iTail = list.end();
				list.erase(list.begin(), iHead);
			};
		
			bool Consume(T& t) //取消息
			{
				TIterator iNext = iHead;
				++iNext;
				if (iNext != iTail)
				{
					iHead = iNext;
					t = *iHead;
					return true;
				}
				return false;
			};
			 
			bool Peek(T& t) const //查看消息不删除
			{
				TIterator iNext = iHead;
				++iNext;
				if (iNext != iTail)
				{
					t = *iNext;
					return true;
				}
				return false;
			};
			 
			bool IsEmpty() const
			{
				TIterator iNext = iHead;
				++iNext;
				if (iNext != iTail)
				{
					return false;
				}
				else
				{
					return true;
				}
			};
				    
			int GetCapacity() const
			{
				return list.max_size();
			};
		
		private:
			using TList = std::list<T>;
			TList	list;
			using TIterator	= typename TList::iterator;
			TIterator	iHead, iTail;
	};

} // end of comm

/*
 *analysis about why this implementation could support synchronization without lock
 *
 *firstly, there is a fake node in the list, node is produced at the tailer while consumed at the header;
 *secondly, for the producing, iHead may get changed by the consuming through backward moving, the real-time 
 *consumed node will be removed correctly.
 *thirdly,  for the consuming, iTail may get changed by the producing through backward moving, the real-time
 *produced node will push list.end() one step backward which will result in consuming of the new produced node.
 * */

