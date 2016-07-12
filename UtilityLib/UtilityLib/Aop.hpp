#pragma once
namespace ztl
{
	namespace aop
	{
		template<typename... AopObjectType>
		struct AopInvoker
		{
		private:
			template<typename T, typename = decltype(T::before()), typename = decltype(T::after())>
			static std::true_type HasAopFunction();
			template<typename T>
			static std::false_type HasAopFunction(...);
			template<typename X, typename... T>
			struct resultType
			{
				static const int value = std::is_same<X, std::true_type>::value + resultType<T...>::value;
			};
			template<typename X>
			struct resultType<X>
			{
				static const int value = std::is_same<X, std::true_type>::value;
			};
			struct AopInvokerGuard
			{
				AopInvokerGuard()
				{
					std::initializer_list<int>{(AopObjectType::before(), 0)...};
				}
				~AopInvokerGuard()
				{
					std::initializer_list<int>{(AopObjectType::after(), 0)...};
				}
			};
		public:
			static_assert(resultType<decltype(HasAopFunction<AopObjectType>())...>::value == sizeof... (AopObjectType), "AopObjectType need have before and after function");

			template<typename... ArgType, typename CallableType>
			static decltype(auto) Invoker(CallableType&& callable, ArgType&&... args)
			{
				AopInvokerGuard guard;
				return std::invoke(std::forward<CallableType>(callable), std::forward<ArgType>(args)...);
			}
		};
		template<>
		struct AopInvoker<>
		{
			template<typename... ArgType, typename CallableType>
			static auto Invoker(CallableType&& callable, ArgType&&... args)
			{
				return std::invoke(std::forward<CallableType>(callable), std::forward<ArgType>(args)...);;
			}
		};
	}
}