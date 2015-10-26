#pragma once
namespace yt
{
/*
	//[]��autoptr
*/
	template <class T>
		class autoptr_array
		{
			public:
				autoptr_array(const T *ptr_array):m_ptr_array(ptr_array){}
				~autoptr_array()
				{
					if(m_ptr_array)
						delete []m_ptr_array;
				}
			private:
				const T *m_ptr_array;
		};
}
