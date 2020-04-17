#pragma once

/* 
	GetInstance() 함수가 있는 싱글턴 클래스를 만드는 매크로
*/
#define THIS_CLASS_IS_SINGLETON(className)				\
	public:												\
		static className* GetInstance()					\
		{												\
			static className instance;					\
			return &instance;							\
		}												\
														\
	private:											\
		className() = default;							\
		~className() = default;							\
		className(const className& rhs) = delete;		\
		className(const className&& rhs) = delete;		\
		void operator=(const className& rhs) = delete;	\
		void operator=(const className&& rhs) = delete;

/*
	문자열 매크로
*/
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)
#define CONCAT_STRING(a,b,c) a##b##c
