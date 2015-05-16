///////////////////////////////////////////
//An engine resource smart pointer, that
//always returns the default
//resource instead of NULL
///////////////////////////////////////////

#pragma once

namespace Comet
{
	
	template<typename T>
	T* DefaultPtr()
	{
		static T obj(true);
		return &obj;
	}

	template<typename T>
	class res_ptr
	{
	public:
		res_ptr(){ _reset(); }
		res_ptr(T* p){ ptr = p; if (!p) _reset(); }
		~res_ptr(){}

		void operator=(T* p){ ptr = p; if (!p) _reset(); }
		T* operator->() const
		{	return ptr;}

		T* get() const { return ptr; }
		bool isnull(){ return (DefaultPtr<T>() == ptr); }
		void free(){ delete ptr; }
	private:
		//Set ptr to default value
		void _reset(){ ptr = DefaultPtr<T>(); }
		T* ptr;
	};

}