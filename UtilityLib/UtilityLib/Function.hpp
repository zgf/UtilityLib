#pragma once
namespace ztl
{
	template<typename Callable>
	class Function
	{
	public:
		template<typename Func>
		Function(Func&& func) :callable(std::forward<Func>(func))
		{

		}
	public:
		template<typename... Arg>
		auto operator()(Arg&&... args)
		{
			return std::invoke(std::forward<Callable>(callable), std::forward<Arg>(args)...);
		}
	private:
		std::conditional_t<std::is_member_function_pointer<Callable>::value, Callable, std::add_lvalue_reference_t<Callable>>  callable;
	};
}