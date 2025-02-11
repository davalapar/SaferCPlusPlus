
// Copyright (c) 2015 Noah Lopez
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef MSEPOINTERBASICS_H
#define MSEPOINTERBASICS_H

#include <cassert>
#include <climits>       // ULONG_MAX
#include <limits>       // std::numeric_limits
#include <stdexcept>      // primitives_range_error
#include <memory>
#include <unordered_set>
#include <functional>

#ifndef MSEPRIMITIVES_H

#if __cplusplus >= 201703L
#define MSE_HAS_CXX17
#endif // __cplusplus >= 201703L
/*compiler specific defines*/
#ifdef _MSC_VER
#if _MSVC_LANG >= 201703L || (defined(_HAS_CXX17) && (_HAS_CXX17 >= 1))
#define MSE_HAS_CXX17
#endif // _MSVC_LANG >= 201703L || (defined(_HAS_CXX17) && (_HAS_CXX17 >= 1))
#if (1700 > _MSC_VER)
#define MSVC2010_COMPATIBLE 1
#endif /*(1700 > _MSC_VER)*/
#if (1900 > _MSC_VER)
#define MSVC2013_COMPATIBLE 1
#endif /*(1900 > _MSC_VER)*/
#if (1910 > _MSC_VER)
#define MSVC2015_COMPATIBLE 1
#endif /*(1910 > _MSC_VER)*/
#else /*_MSC_VER*/
#if (defined(__GNUC__) || defined(__GNUG__))
#define GPP_COMPATIBLE 1
#if ((5 > __GNUC__) && (!defined(__clang__)))
#define GPP4P8_COMPATIBLE 1
#endif /*((5 > __GNUC__) && (!defined(__clang__)))*/
#endif /*(defined(__GNUC__) || defined(__GNUG__))*/
#endif /*_MSC_VER*/

#if __cpp_exceptions >= 199711
#define MSE_TRY try
#define MSE_CATCH(x) catch(x)
#define MSE_CATCH_ANY catch(...)
#define MSE_FUNCTION_TRY try
#define MSE_FUNCTION_CATCH(x) catch(x)
#define MSE_FUNCTION_CATCH_ANY catch(...)
#else // __cpp_exceptions >= 199711
#define MSE_TRY if (true)
#define MSE_CATCH(x) if (false)
#define MSE_CATCH_ANY if (false)
#define MSE_FUNCTION_TRY
#define MSE_FUNCTION_CATCH(x) void mse_placeholder_function_catch(x)
#define MSE_FUNCTION_CATCH_ANY void mse_placeholder_function_catch_any()
#define MSE_CUSTOM_THROW_DEFINITION(x) exit(-11)
#endif // __cpp_exceptions >= 199711

#endif /*ndef MSEPRIMITIVES_H*/

#ifdef MSE_SAFER_SUBSTITUTES_DISABLED
#define MSE_POINTERBASICS_DISABLED
#define MSE_SAFERPTR_DISABLED
#endif /*MSE_SAFER_SUBSTITUTES_DISABLED*/


/* start of scope pointer defines */

/* This is done here rather than in msescope.h because some elements in this file have to know whether or not
MSE_SCOPEPOINTER_DISABLED will ultimately be defined. */

#if defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED)
#define MSE_SCOPEPOINTER_DISABLED
#endif /*defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED)*/

/* MSE_SCOPEPOINTER_USE_RELAXED_REGISTERED is deprecated */
#ifdef MSE_SCOPEPOINTER_USE_RELAXED_REGISTERED
#define MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED
#endif // MSE_SCOPEPOINTER_USE_RELAXED_REGISTERED

/* By default, norad pointers are used to catch unsafe misuse of scope pointers in debug mode. Defining
MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED will cause them to be used in non-debug modes as well. */
#if (!defined(NDEBUG)) && (!defined(MSE_SCOPEPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED) && (!defined(MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED)))
#define MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED
#endif // (!defined(NDEBUG)) && (!defined(MSE_SCOPEPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED) && (!defined(MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED)))

#ifdef MSE_SCOPEPOINTER_DISABLED
#undef MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED
#endif // MSE_SCOPEPOINTER_DISABLED

#ifdef MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED
#undef MSE_SAFERPTR_DISABLED
#else // MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED
#ifdef NDEBUG
/* By default we make scope pointers simply an alias for native pointers in non-debug builds. */
#define MSE_SCOPEPOINTER_DISABLED
#endif // NDEBUG
#endif // MSE_SCOPEPOINTER_RUNTIME_CHECKS_ENABLED

/* end of scope pointer defines */

/* start of thread_local pointer defines */

/* This is done here rather than in msethreadlocal.h because some elements in this file have to know whether or not
MSE_THREADLOCALPOINTER_DISABLED will ultimately be defined. */

#if defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED)
#define MSE_THREADLOCALPOINTER_DISABLED
#endif /*defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED)*/

/* By default, norad pointers are used to catch unsafe misuse of thread_local pointers in debug mode. Defining
MSE_THREADLOCALPOINTER_RUNTIME_CHECKS_ENABLED will cause them to be used in non-debug modes as well. */
#if (!defined(NDEBUG)) && (!defined(MSE_THREADLOCALPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED))
#define MSE_THREADLOCALPOINTER_RUNTIME_CHECKS_ENABLED
#endif // (!defined(NDEBUG)) && (!defined(MSE_THREADLOCALPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED))

#ifdef MSE_THREADLOCALPOINTER_DISABLED
#undef MSE_THREADLOCALPOINTER_RUNTIME_CHECKS_ENABLED
#endif // MSE_THREADLOCALPOINTER_DISABLED

#ifdef MSE_THREADLOCALPOINTER_RUNTIME_CHECKS_ENABLED
#undef MSE_SAFERPTR_DISABLED
#else // MSE_THREADLOCALPOINTER_RUNTIME_CHECKS_ENABLED
#ifdef NDEBUG
/* By default we make thread_local pointers simply an alias for native pointers in non-debug builds. */
#define MSE_THREADLOCALPOINTER_DISABLED
#endif // NDEBUG
#endif // MSE_THREADLOCALPOINTER_RUNTIME_CHECKS_ENABLED

/* end of thread_local pointer defines */

/* start of static pointer defines */

/* This is done here rather than in msethreadlocal.h because some elements in this file have to know whether or not
MSE_STATICPOINTER_DISABLED will ultimately be defined. */

#if defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED)
#define MSE_STATICPOINTER_DISABLED
#endif /*defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED)*/

#if (!defined(NDEBUG)) && (!defined(MSE_STATICPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED))
#define MSE_STATICIMMUTABLEPOINTER_RUNTIME_CHECKS_ENABLED
#endif // (!defined(NDEBUG)) && (!defined(MSE_STATICPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED))

#ifdef MSE_STATICPOINTER_DISABLED
#undef MSE_STATICIMMUTABLEPOINTER_RUNTIME_CHECKS_ENABLED
#endif // MSE_STATICPOINTER_DISABLED

#ifdef MSE_STATICIMMUTABLEPOINTER_RUNTIME_CHECKS_ENABLED
#undef MSE_SAFERPTR_DISABLED
#else // MSE_STATICIMMUTABLEPOINTER_RUNTIME_CHECKS_ENABLED
#ifdef NDEBUG
/* By default we make static pointers simply an alias for native pointers in non-debug builds. */
#define MSE_STATICPOINTER_DISABLED
#endif // NDEBUG
#endif // MSE_STATICIMMUTABLEPOINTER_RUNTIME_CHECKS_ENABLED

/* end of static pointer defines */


#if defined(MSVC2013_COMPATIBLE) || defined(MSVC2010_COMPATIBLE)
#define MSE_CONSTEXPR
#else // defined(MSVC2013_COMPATIBLE) || defined(MSVC2010_COMPATIBLE)
#define MSE_CONSTEXPR constexpr
#endif // defined(MSVC2013_COMPATIBLE) || defined(MSVC2010_COMPATIBLE)

#ifdef MSE_HAS_CXX17
#define MSE_INLINE_VAR inline
#else // defined(MSVC2013_COMPATIBLE) || defined(MSVC2010_COMPATIBLE)
#define MSE_INLINE_VAR
#endif // defined(MSVC2013_COMPATIBLE) || defined(MSVC2010_COMPATIBLE)

#ifndef MSE_DEPRECATED
#define MSE_DEPRECATED [[deprecated]]
#endif // !MSE_DEPRECATED


#ifndef MSE_PUSH_MACRO_NOT_SUPPORTED
#pragma push_macro("MSE_THROW")
#endif // !MSE_PUSH_MACRO_NOT_SUPPORTED

#ifdef MSE_CUSTOM_THROW_DEFINITION
#define MSE_THROW(x) MSE_CUSTOM_THROW_DEFINITION(x)
#else // MSE_CUSTOM_THROW_DEFINITION
#define MSE_THROW(x) throw(x)
#endif // MSE_CUSTOM_THROW_DEFINITION

#ifdef _MSC_VER
#pragma warning( push )  
#pragma warning( disable : 4505 )
#endif /*_MSC_VER*/

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
#else /*__clang__*/
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#endif /*__GNUC__*/
#endif /*__clang__*/

namespace mse {

	class primitives_null_dereference_error : public std::logic_error {
	public:
		using std::logic_error::logic_error;
	};

#ifndef NDEBUG
#ifndef MSE_SUPPRESS_CHECK_USE_BEFORE_SET
#define MSE_CHECK_USE_BEFORE_SET
#endif // !MSE_SUPPRESS_CHECK_USE_BEFORE_SET
#endif // !NDEBUG


	/* This macro roughly simulates constructor inheritance. */
#define MSE_USING_V1(Derived, Base) \
    template<typename ...Args, typename = typename std::enable_if<std::is_constructible<Base, Args...>::value>::type> \
    Derived(Args &&...args) : Base(std::forward<Args>(args)...) {}

	namespace impl {
		template<bool _Val>
		struct Cat_base_msepointerbasics : std::integral_constant<bool, _Val> {	// base class for type predicates
		};

		template<class _Ty, class... _Args>
		struct is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics : impl::Cat_base_msepointerbasics<false> {};
		template<class _Ty, class _Tz>
		struct is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics<_Ty, _Tz> : impl::Cat_base_msepointerbasics<std::is_base_of<typename std::remove_reference<_Ty>::type, typename std::remove_reference<_Tz>::type>::value> {};
		template<class _Ty>
		struct is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics<_Ty> : impl::Cat_base_msepointerbasics<false> {};

		template<typename A, typename B>
		using disable_if_is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics =
			typename std::enable_if<!std::is_base_of<A, typename std::remove_reference<B>::type>::value>::type;
	}

	/* This macro roughly simulates constructor inheritance. */
#define MSE_USING(Derived, Base) \
    template<typename ...Args, typename = typename std::enable_if< \
	std::is_constructible<Base, Args...>::value \
	&& !mse::impl::is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics<Derived, Args...>::value \
	>::type> \
    Derived(Args &&...args) : Base(std::forward<Args>(args)...) {}

	/* This macro roughly simulates constructor inheritance, but adds an additional initialization statement
	to each constructor. */
#define MSE_USING_WITH_ADDED_INIT(Derived, Base, InitializationStatement) \
    template<typename ...Args, typename = typename std::enable_if< \
	std::is_constructible<Base, Args...>::value \
	&& !mse::impl::is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics<Derived, Args...>::value \
	>::type> \
    Derived(Args &&...args) : Base(std::forward<Args>(args)...) { InitializationStatement; }

	namespace impl {
		template<class T, class EqualTo>
		struct HasOrInheritsAssignmentOperator_msepointerbasics_impl
		{
			template<class U, class V>
			static auto test(U* u) -> decltype(*u = *u, std::declval<V>(), bool(true));
			template<typename, typename>
			static auto test(...)->std::false_type;

			static const bool value = std::is_same<bool, decltype(test<T, EqualTo>(0))>::value;
			using type = typename std::is_same<bool, decltype(test<T, EqualTo>(0))>::type;
		};
		template<class T, class EqualTo = T>
		struct HasOrInheritsAssignmentOperator_msepointerbasics : HasOrInheritsAssignmentOperator_msepointerbasics_impl<
			typename std::remove_reference<T>::type, typename std::remove_reference<EqualTo>::type>::type {};
	}

#define MSE_USING_ASSIGNMENT_OPERATOR(Base) \
	template<class _Ty2mse_uao, class _TBase2 = Base, typename = typename std::enable_if<mse::impl::HasOrInheritsAssignmentOperator_msepointerbasics<_TBase2>::value \
		&& ((!mse::impl::is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics<_TBase2, _Ty2mse_uao>::value) || std::is_same<_TBase2, typename std::remove_reference<_Ty2mse_uao>::type>::value)>::type> \
	auto& operator=(_Ty2mse_uao&& _X) { Base::operator=(std::forward<decltype(_X)>(_X)); return (*this); } \
	template<class _Ty2mse_uao, class _TBase2 = Base, typename = typename std::enable_if<mse::impl::HasOrInheritsAssignmentOperator_msepointerbasics<_TBase2>::value \
		&& ((!mse::impl::is_a_pair_with_the_first_a_base_of_the_second_msepointerbasics<_TBase2, _Ty2mse_uao>::value) || std::is_same<_TBase2, _Ty2mse_uao>::value)>::type> \
	auto& operator=(const _Ty2mse_uao& _X) { Base::operator=(_X); return (*this); }

#define MSE_DEFAULT_COPY_AND_MOVE_CONSTRUCTOR_DECLARATIONS(ClassName) \
	ClassName(const ClassName&) = default; \
	ClassName(ClassName&&) = default;

#define MSE_USING_AND_DEFAULT_COPY_AND_MOVE_CONSTRUCTOR_DECLARATIONS(Derived, Base) MSE_USING(Derived, Base) MSE_DEFAULT_COPY_AND_MOVE_CONSTRUCTOR_DECLARATIONS(Derived)
#define MSE_USING_AND_DEFAULT_COPY_AND_MOVE_CONSTRUCTOR_DECLARATIONS_AND_USING_ASSIGNMENT_OPERATOR(Derived, Base) \
	MSE_USING_AND_DEFAULT_COPY_AND_MOVE_CONSTRUCTOR_DECLARATIONS(Derived, Base) MSE_USING_ASSIGNMENT_OPERATOR(Base)

#if defined(MSE_REGISTEREDPOINTER_DISABLED) || defined(MSE_NORADPOINTER_DISABLED) || defined(MSE_SCOPEPOINTER_DISABLED) || defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED) || defined(MSE_THREADLOCALPOINTER_DISABLED) || defined(MSE_STATICPOINTER_DISABLED)
#define MSE_SOME_POINTER_TYPE_IS_DISABLED
#endif /*defined(MSE_REGISTEREDPOINTER_DISABLED) || defined(MSE_NORADPOINTER_DISABLED) || defined(MSE_SCOPEPOINTER_DISABLED) || defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED) || defined(MSE_THREADLOCALPOINTER_DISABLED) || defined(MSE_STATICPOINTER_DISABLED)*/

#if defined(MSE_REGISTEREDPOINTER_DISABLED) || defined(MSE_NORADPOINTER_DISABLED) || defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED) || defined(MSE_THREADLOCALPOINTER_DISABLED) || defined(MSE_STATICPOINTER_DISABLED)
#define MSE_SOME_NON_XSCOPE_POINTER_TYPE_IS_DISABLED
#endif /*defined(MSE_REGISTEREDPOINTER_DISABLED) || defined(MSE_NORADPOINTER_DISABLED) || defined(MSE_SAFER_SUBSTITUTES_DISABLED) || defined(MSE_SAFERPTR_DISABLED) || defined(MSE_THREADLOCALPOINTER_DISABLED) || defined(MSE_STATICPOINTER_DISABLED)*/

#if defined(MSE_SOME_POINTER_TYPE_IS_DISABLED)
#define MSE_DEFAULT_OPERATOR_AMPERSAND_DECLARATION
#else /*defined(MSE_SOME_POINTER_TYPE_IS_DISABLED)*/
#define MSE_DEFAULT_OPERATOR_AMPERSAND_DECLARATION	auto operator&() { return this; } auto operator&() const { return this; }
#endif /*defined(MSE_SOME_POINTER_TYPE_IS_DISABLED)*/

#define MSE_DEFAULT_OPERATOR_NEW_DECLARATION	void* operator new(size_t size) { return ::operator new(size); }
#define MSE_DEFAULT_OPERATOR_NEW_AND_AMPERSAND_DECLARATION	MSE_DEFAULT_OPERATOR_NEW_DECLARATION MSE_DEFAULT_OPERATOR_AMPERSAND_DECLARATION

#define MSE_USING_ASSIGNMENT_OPERATOR_AND_DEFAULT_OPERATOR_NEW_AND_AMPERSAND_DECLARATION(Base)	\
	MSE_USING_ASSIGNMENT_OPERATOR(Base) MSE_DEFAULT_OPERATOR_NEW_AND_AMPERSAND_DECLARATION


#define MSE_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

	namespace impl {
		namespace ns_is_instantiation_of {
			template <template <typename...> class Template, typename Type>
			struct is_instance_of : std::false_type {};
			template <template <typename...> class Template, typename... Args>
			struct is_instance_of<Template, Template<Args...>> : std::true_type {};
			template <template <typename...> class Template, typename Type>
			constexpr bool is_instance_of_v = is_instance_of<Template, Type>::value;
		}

		/* determines if a given type is an instantiation of a given template */
		template<typename Type, template<typename...> class Template>
		struct is_instantiation_of : ns_is_instantiation_of::is_instance_of<Template, Type> { };

		template<class...> struct conjunction : std::true_type { };
		template<class B1> struct conjunction<B1> : B1 { };
		template<class B1, class... Bn> struct conjunction<B1, Bn...> : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};

		template<class...> struct disjunction : std::false_type { };
		template<class B1> struct disjunction<B1> : B1 { };
		template<class B1, class... Bn> struct disjunction<B1, Bn...> : std::conditional_t<bool(B1::value), B1, disjunction<Bn...>> { };

		template<class B>
		struct negation : std::integral_constant<bool, !bool(B::value)> { };
	}

	namespace us {
		namespace impl {
			namespace ns_as_ref {
				template<typename _Ty, typename _Ty2>
				_Ty& as_ref_helper1(std::false_type, _Ty2& x) {
					_Ty* ptr = std::addressof(x);
					return *ptr;
				}
				template<typename _Ty, typename _Ty2>
				_Ty as_ref_helper1(std::true_type, _Ty2&& x) {
					return std::forward<decltype(x)>(x);
				}
			}
			template<typename _Ty, typename _Ty2>
			auto as_ref(_Ty2&& x) -> decltype(ns_as_ref::as_ref_helper1(typename std::is_rvalue_reference<decltype(x)>::type(), std::forward<decltype(x)>(x))) {
				return ns_as_ref::as_ref_helper1<_Ty>(typename std::is_rvalue_reference<decltype(x)>::type(), std::forward<decltype(x)>(x));
			}
			template<typename _Ty, typename _Ty2>
			_Ty& as_ref(_Ty2& x) {
				_Ty* ptr = std::addressof(x);
				return *ptr;
			}
			template<typename _Ty, typename _Ty2>
			const _Ty& as_ref(const _Ty2& x) {
				_Ty const * ptr = std::addressof(x);
				return *ptr;
			}
		}
	}


	namespace us {
		namespace impl {
			class XScopeTagBase { public: void xscope_tag() const {} };

			/* Note that objects not derived from ReferenceableByScopePointerTagBase might still be targeted by a scope pointer via
			make_pointer_to_member_v2(). */
			class ReferenceableByScopePointerTagBase {};

			class ContainsNonOwningScopeReferenceTagBase {};
			class XScopeContainsNonOwningScopeReferenceTagBase : public ContainsNonOwningScopeReferenceTagBase, public XScopeTagBase {};

			template<typename _Ty, typename _TID/* = TPointerID<_Ty>*/>
			class TPointer;
			template<typename _Ty, typename _TID/* = TPointerID<_Ty>*/>
			class TPointerForLegacy;
		}
	}
	namespace rsv {
		class XScopeTagBase : public mse::us::impl::XScopeTagBase { public: void xscope_tag() const {} };
		class ContainsNonOwningScopeReferenceTagBase : public mse::us::impl::ContainsNonOwningScopeReferenceTagBase {};
		class ReferenceableByScopePointerTagBase : public mse::us::impl::ReferenceableByScopePointerTagBase {};
	}

	namespace rsv {
		template<typename _Ty>
		class TFParam;
		template<typename _Ty>
		class TReturnableFParam;

		namespace impl {
			template<typename _Ty>
			struct remove_fparam {
				typedef _Ty type;
			};
			template<typename _Ty>
			struct remove_fparam<mse::rsv::TReturnableFParam<_Ty> > {
				typedef typename remove_fparam<_Ty>::type type;
			};
			template<typename _Ty>
			struct remove_fparam<mse::rsv::TFParam<_Ty> > {
				typedef typename remove_fparam<_Ty>::type type;
			};
		}
	}

	namespace impl {

		template<typename _Ty>
		struct is_potentially_xscope : std::integral_constant<bool, mse::impl::disjunction<
			std::is_base_of<mse::us::impl::XScopeTagBase
			, typename std::remove_reference<_Ty>::type>
			, std::is_pointer<typename std::remove_reference<_Ty>::type>
#ifdef MSE_SCOPEPOINTER_DISABLED
			, mse::impl::is_instantiation_of<typename std::remove_reference<_Ty>::type, mse::us::impl::TPointerForLegacy>
			, mse::impl::is_instantiation_of<typename std::remove_reference<_Ty>::type, mse::us::impl::TPointer>
#endif // MSE_SCOPEPOINTER_DISABLED
		>::value> {};

		template<typename _Ty>
		struct is_potentially_not_xscope : std::integral_constant<bool, mse::impl::conjunction<
			mse::impl::negation<std::is_base_of<mse::us::impl::XScopeTagBase
				, typename std::remove_reference<_Ty>::type> >
#if (!defined(MSE_SOME_NON_XSCOPE_POINTER_TYPE_IS_DISABLED)) && (!defined(MSE_SAFER_SUBSTITUTES_DISABLED)) && (!defined(MSE_DISABLE_RAW_POINTER_SCOPE_RESTRICTIONS))
			, mse::impl::negation<std::is_pointer<typename std::remove_reference<_Ty>::type> >
#endif // (!defined(MSE_SOME_NON_XSCOPE_POINTER_TYPE_IS_DISABLED)) && (!defined(MSE_SAFER_SUBSTITUTES_DISABLED)) && (!defined(MSE_DISABLE_RAW_POINTER_SCOPE_RESTRICTIONS))
		>::value> {};

		template<typename _Ty>
		struct is_xscope : mse::impl::negation<is_potentially_not_xscope<_Ty> > {};

		/* The purpose of these template functions are just to produce a compile error on attempts to instantiate
		when certain conditions are not met. */
		template<class _Ty, class = typename std::enable_if<(mse::impl::is_potentially_not_xscope<_Ty>::value), void>::type>
		void T_valid_if_not_an_xscope_type() {}

		template<class _Ty>
		void T_valid_if_not_an_xscope_type(const _Ty&) {
			T_valid_if_not_an_xscope_type<_Ty>();
		}
	}

	namespace impl {

		template<typename _Ty>
		struct potentially_contains_non_owning_scope_reference : std::integral_constant<bool, mse::impl::disjunction<
			std::is_base_of<mse::us::impl::ContainsNonOwningScopeReferenceTagBase
			, typename std::remove_reference<_Ty>::type>
			, std::is_pointer<typename std::remove_reference<_Ty>::type>
#ifdef MSE_SCOPEPOINTER_DISABLED
			, mse::impl::is_instantiation_of<typename std::remove_reference<_Ty>::type, mse::us::impl::TPointerForLegacy>
			, mse::impl::is_instantiation_of<typename std::remove_reference<_Ty>::type, mse::us::impl::TPointer>
#endif // MSE_SCOPEPOINTER_DISABLED
		>::value> {};

		template<typename _Ty>
		struct potentially_does_not_contain_non_owning_scope_reference : std::integral_constant<bool, mse::impl::conjunction<
			mse::impl::negation<std::is_base_of<mse::us::impl::ContainsNonOwningScopeReferenceTagBase
				, typename std::remove_reference<_Ty>::type> >
#if (!defined(MSE_SOME_NON_XSCOPE_POINTER_TYPE_IS_DISABLED)) && (!defined(MSE_SAFER_SUBSTITUTES_DISABLED)) && (!defined(MSE_DISABLE_RAW_POINTER_SCOPE_RESTRICTIONS))
			, mse::impl::negation<std::is_pointer<typename std::remove_reference<_Ty>::type> >
#endif // (!defined(MSE_SOME_NON_XSCOPE_POINTER_TYPE_IS_DISABLED)) && (!defined(MSE_SAFER_SUBSTITUTES_DISABLED)) && (!defined(MSE_DISABLE_RAW_POINTER_SCOPE_RESTRICTIONS))
		>::value> {};

		template<typename _Ty>
		struct contains_non_owning_scope_reference : mse::impl::negation<potentially_does_not_contain_non_owning_scope_reference<_Ty> > {};
	}

	namespace impl {

		template<typename _Ty>
		struct is_potentially_referenceable_by_scope_pointer : std::integral_constant<bool, mse::impl::disjunction<
			std::is_base_of<mse::us::impl::ReferenceableByScopePointerTagBase, typename std::remove_reference<_Ty>::type>
#ifdef MSE_SCOPEPOINTER_DISABLED
			, std::true_type
#endif // MSE_SCOPEPOINTER_DISABLED
		>::value> {};

		template<typename _Ty>
		struct is_potentially_not_referenceable_by_scope_pointer : std::integral_constant<bool, mse::impl::conjunction<
			mse::impl::negation<std::is_base_of<mse::us::impl::ReferenceableByScopePointerTagBase, typename std::remove_reference<_Ty>::type> >
#ifdef MSE_SCOPEPOINTER_DISABLED
			, std::true_type
#endif // MSE_SCOPEPOINTER_DISABLED
		>::value> {};

		template<typename _Ty>
		struct is_referenceable_by_scope_pointer : mse::impl::negation<is_potentially_not_referenceable_by_scope_pointer<_Ty> > {};
	}

	namespace impl {
		namespace lambda {

			template<class T, class EqualTo>
			struct HasOrInheritsFunctionCallOperator_msemsearray_impl
			{
				template<class U, class V>
				static auto test(U*) -> decltype(std::declval<U>().operator(), std::declval<V>(), bool(true));
				template<typename, typename>
				static auto test(...)->std::false_type;

				using type = typename std::is_same<bool, decltype(test<T, EqualTo>(0))>::type;
			};
			template<class T, class EqualTo = T>
			struct HasOrInheritsFunctionCallOperator_msemsearray : HasOrInheritsFunctionCallOperator_msemsearray_impl<
				typename std::remove_reference<T>::type, typename std::remove_reference<EqualTo>::type>::type {};

			template<typename T> struct remove_class { };
			template<typename C, typename R, typename... A>
			struct remove_class<R(C::*)(A...)> { using type = R(A...); };
			template<typename C, typename R, typename... A>
			struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
			template<typename C, typename R, typename... A>
			struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
			template<typename C, typename R, typename... A>
			struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

			template<typename T>
			auto get_signature_impl_helper1(std::true_type) {
				return &std::remove_reference<T>::type::operator();
			}
			template<typename T>
			auto get_signature_impl_helper1(std::false_type) {
				/* This type doesn't seem to have a function signature (as would be the case with generic lambdas for
				example), so we'll just return a dummy signature. */
				auto lambda1 = []() {};
				return &std::remove_reference<decltype(lambda1)>::type::operator();
			}
			template<typename T>
			struct get_signature_impl {
				using type = typename remove_class<decltype(get_signature_impl_helper1<T>(typename HasOrInheritsFunctionCallOperator_msemsearray<T>::type::type()))>::type;
			};
			template<typename R, typename... A>
			struct get_signature_impl<R(A...)> { using type = R(A...); };
			template<typename R, typename... A>
			struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
			template<typename R, typename... A>
			struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
			/* Get the signature of a function type. */
			template<typename T> using get_signature = typename get_signature_impl<T>::type;

			/* "non-capture" lambdas are, unlike "capture" lambdas, convertible to function pointers */
			template<class T, class Ret, class...Args>
			struct is_convertible_to_function_pointer1 : std::is_convertible<T, Ret(*)(Args...)> {};
			template <typename T, typename T2>
			struct is_convertible_to_function_pointer2;
			template <typename T, typename Ret, typename... Args>
			struct is_convertible_to_function_pointer2<T, Ret(Args...)> {
				static constexpr auto value = is_convertible_to_function_pointer1<T, Ret, Args...>::value;
			};

			template <typename T>
			struct is_convertible_to_function_pointer : is_convertible_to_function_pointer2<T, get_signature<T> > {};

			template<class T, class Ret, class...Args>
			struct is_function_obj_that_is_not_convertible_to_function_pointer1 {
				/* "capture" lambdas are convertible to corresponding std::function<>s, but not to function pointers */
				static constexpr auto convertible = std::is_convertible<T, std::function<Ret(Args...)>>::value;
				static constexpr auto value = convertible && !is_convertible_to_function_pointer1<T, Ret, Args...>::value;
			};
			template <typename T, typename T2>
			struct is_function_obj_that_is_not_convertible_to_function_pointer2;
			template <typename T, typename Ret, typename... Args>
			struct is_function_obj_that_is_not_convertible_to_function_pointer2<T, Ret(Args...)> {
				static constexpr auto value = is_function_obj_that_is_not_convertible_to_function_pointer1<T, Ret, Args...>::value;
			};

			template <typename T>
			struct is_function_obj_that_is_not_convertible_to_function_pointer : is_function_obj_that_is_not_convertible_to_function_pointer2<T, get_signature<T> > {};
		}
	}

	namespace us {
		namespace impl {
			class AsyncNotShareableTagBase {};
			class AsyncNotPassableTagBase {};
			class AsyncNotShareableAndNotPassableTagBase : public AsyncNotShareableTagBase, public AsyncNotPassableTagBase {};
		}
	}


	namespace impl {

		template<typename T>
		struct HasAsyncNotShareableAndNotPassableTagMethod_msemsearray
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::async_not_shareable_and_not_passable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template<typename T>
		struct HasAsyncShareableAndPassableTagMethod_msemsearray
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::async_shareable_and_passable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template<typename T>
		struct HasAsyncShareableTagMethod_msemsearray
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::async_shareable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template<typename T>
		struct HasAsyncPassableTagMethod_msemsearray
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::async_passable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template<typename T>
		struct HasXScopeAsyncShareableAndPassableTagMethod_msemsearray
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::xscope_async_shareable_and_passable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template<typename T>
		struct HasXScopeAsyncShareableTagMethod_msemsearray
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::xscope_async_shareable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template<typename T>
		struct HasXScopeAsyncPassableTagMethod_msescope
		{
			template<typename U, void(U::*)() const> struct SFINAE {};
			template<typename U> static char Test(SFINAE<U, &U::xscope_async_passable_tag>*);
			template<typename U> static int Test(...);
			static const bool Has = (sizeof(Test<T>(0)) == sizeof(char));
		};

		template <typename _Ty> struct is_marked_as_shareable_msemsearray : std::integral_constant<bool, (mse::impl::HasAsyncShareableTagMethod_msemsearray<_Ty>::Has)
			|| (mse::impl::HasAsyncShareableAndPassableTagMethod_msemsearray<_Ty>::Has) || (std::is_arithmetic<_Ty>::value) /*|| (std::is_function<typename std::remove_pointer<typename std::remove_reference<_Ty>::type>::type>::value)*/
			|| (mse::impl::lambda::is_convertible_to_function_pointer<typename std::remove_pointer<typename std::remove_reference<_Ty>::type>::type>::value)
#ifdef MSE_SCOPEPOINTER_DISABLED
			|| (std::is_pointer<_Ty>::value)
#endif // MSE_SCOPEPOINTER_DISABLED
			|| (std::is_same<_Ty, void>::value)> {};

		template<class _Ty, class = typename std::enable_if<(is_marked_as_shareable_msemsearray<_Ty>::value), void>::type>
		void T_valid_if_is_marked_as_shareable_msemsearray() {}

		template<typename _Ty>
		const _Ty& async_shareable(const _Ty& _X) {
			T_valid_if_is_marked_as_shareable_msemsearray<_Ty>();
			return _X;
		}
		template<typename _Ty>
		_Ty&& async_shareable(_Ty&& _X) {
			T_valid_if_is_marked_as_shareable_msemsearray<typename std::remove_reference<_Ty>::type>();
			return std::forward<decltype(_X)>(_X);
		}

		template <typename _Ty> struct is_marked_as_passable_msemsearray : std::integral_constant<bool, (mse::impl::HasAsyncPassableTagMethod_msemsearray<_Ty>::Has)
			|| (mse::impl::HasAsyncShareableAndPassableTagMethod_msemsearray<_Ty>::Has) || (std::is_arithmetic<_Ty>::value) /*|| (std::is_function<typename std::remove_pointer<typename std::remove_reference<_Ty>::type>::type>::value)*/
			|| (mse::impl::lambda::is_convertible_to_function_pointer<typename std::remove_pointer<typename std::remove_reference<_Ty>::type>::type>::value)
#ifdef MSE_SCOPEPOINTER_DISABLED
			|| (std::is_pointer<_Ty>::value)
#endif // MSE_SCOPEPOINTER_DISABLED
			|| (std::is_same<_Ty, void>::value)> {};

		template<class _Ty, class = typename std::enable_if<(is_marked_as_passable_msemsearray<_Ty>::value), void>::type>
		void T_valid_if_is_marked_as_passable_or_shareable_msemsearray() {}
		template<typename _Ty>
		const _Ty& async_passable(const _Ty& _X) {
			T_valid_if_is_marked_as_passable_or_shareable_msemsearray<_Ty>();
			return _X;
		}
		template<typename _Ty>
		_Ty&& async_passable(_Ty&& _X) {
			T_valid_if_is_marked_as_passable_or_shareable_msemsearray<typename std::remove_reference<_Ty>::type>();
			return std::forward<decltype(_X)>(_X);
		}

		template <typename _Ty> struct is_marked_as_shareable_and_passable_msemsearray : std::integral_constant<bool, (is_marked_as_shareable_msemsearray<_Ty>::value)
			&& (is_marked_as_passable_msemsearray<_Ty>::value)> {};
		template<class _Ty, class = typename std::enable_if<(is_marked_as_shareable_and_passable_msemsearray<_Ty>::value), void>::type>
		void T_valid_if_is_marked_as_shareable_and_passable_msemsearray() {}


		template <typename _Ty> struct is_marked_as_xscope_shareable_msemsearray : std::integral_constant<bool, (mse::impl::HasXScopeAsyncShareableTagMethod_msemsearray<_Ty>::Has)
			|| (mse::impl::HasXScopeAsyncShareableAndPassableTagMethod_msemsearray<_Ty>::Has) || (is_marked_as_shareable_msemsearray<_Ty>::value)> {};

		template<class _Ty, class = typename std::enable_if<(is_marked_as_xscope_shareable_msemsearray<_Ty>::value), void>::type>
		void T_valid_if_is_marked_as_xscope_shareable_msemsearray() {}
		template<typename _Ty>
		const _Ty& xscope_async_shareable(const _Ty& _X) {
			T_valid_if_is_marked_as_xscope_shareable_msemsearray<_Ty>();
			return _X;
		}
		template<typename _Ty>
		_Ty&& xscope_async_shareable(_Ty&& _X) {
			T_valid_if_is_marked_as_xscope_shareable_msemsearray<typename std::remove_reference<_Ty>::type>();
			return std::forward<decltype(_X)>(_X);
		}

		template <typename _Ty> struct is_marked_as_xscope_passable_msemsearray : std::integral_constant<bool, (mse::impl::HasXScopeAsyncPassableTagMethod_msescope<_Ty>::Has)
			|| (mse::impl::HasXScopeAsyncShareableAndPassableTagMethod_msemsearray<_Ty>::Has) || (is_marked_as_passable_msemsearray<_Ty>::value)> {};

		template<class _Ty, class = typename std::enable_if<(is_marked_as_xscope_passable_msemsearray<_Ty>::value), void>::type>
		void T_valid_if_is_marked_as_xscope_passable_msemsearray() {}
		template<typename _Ty>
		const _Ty& xscope_async_passable(const _Ty& _X) {
			T_valid_if_is_marked_as_xscope_passable_msemsearray<_Ty>();
			return _X;
		}
		template<typename _Ty>
		_Ty&& xscope_async_passable(_Ty&& _X) {
			T_valid_if_is_marked_as_xscope_passable_msemsearray<typename std::remove_reference<_Ty>::type>();
			return std::forward<decltype(_X)>(_X);
		}

		template <typename _Ty> struct is_marked_as_xscope_shareable_and_passable_msemsearray : std::integral_constant<bool, (is_marked_as_xscope_shareable_msemsearray<_Ty>::value)
			&& (is_marked_as_xscope_passable_msemsearray<_Ty>::value)> {};
		template<class _Ty, class = typename std::enable_if<(is_marked_as_xscope_shareable_and_passable_msemsearray<_Ty>::value), void>::type>
		void T_valid_if_is_marked_as_xscope_shareable_and_passable_msemsearray() {}
	}
#define MSE_INHERIT_ASYNC_SHAREABILITY_AND_PASSABILITY_OF(Tmse_isap) \
		template<class Tmse_isap2 = Tmse_isap, class = typename std::enable_if<(std::is_same<Tmse_isap2, Tmse_isap>::value) \
			&& (mse::impl::is_marked_as_shareable_msemsearray<Tmse_isap2>::value), void>::type> \
		void async_shareable_tag() const {} \
		template<class Tmse_isap2 = Tmse_isap, class = typename std::enable_if<(std::is_same<Tmse_isap2, Tmse_isap>::value) \
			&& (mse::impl::is_marked_as_passable_msemsearray<Tmse_isap2>::value), void>::type> \
		void async_passable_tag() const {} \
		template<class Tmse_isap2 = Tmse_isap, class = typename std::enable_if<(std::is_same<Tmse_isap2, Tmse_isap>::value) \
			&& (mse::impl::is_marked_as_xscope_shareable_msemsearray<Tmse_isap2>::value), void>::type> \
		void xscope_async_shareable_tag() const {} \
		template<class Tmse_isap2 = Tmse_isap, class = typename std::enable_if<(std::is_same<Tmse_isap2, Tmse_isap>::value) \
			&& (mse::impl::is_marked_as_xscope_passable_msemsearray<Tmse_isap2>::value), void>::type> \
		void xscope_async_passable_tag() const {}

#define MSE_INHERIT_XSCOPE_ASYNC_SHAREABILITY_AND_PASSABILITY_OF(Tmse_isap) \
		template<class Tmse_isap2 = Tmse_isap, class = typename std::enable_if<(std::is_same<Tmse_isap2, Tmse_isap>::value) \
			&& (mse::impl::is_marked_as_xscope_shareable_msemsearray<Tmse_isap2>::value), void>::type> \
		void xscope_async_shareable_tag() const {} \
		template<class Tmse_isap2 = Tmse_isap, class = typename std::enable_if<(std::is_same<Tmse_isap2, Tmse_isap>::value) \
			&& (mse::impl::is_marked_as_xscope_passable_msemsearray<Tmse_isap2>::value), void>::type> \
		void xscope_async_passable_tag() const {}

	namespace impl {
		template<typename... _Types>
		class TPlaceHolder {};
	}

//define MSE_FIRST_OR_PLACEHOLDER_IF_A_BASE_OF_SECOND(tag_base, class2, class3) std::conditional<!std::is_convertible<class2 const * const, tag_base const * const>::value, tag_base, mse::impl::TPlaceHolder<tag_base, class3> >::type
//define MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(tag_base, class2, class3) std::conditional<std::is_convertible<class2 const * const, tag_base const * const>::value, tag_base, mse::impl::TPlaceHolder<tag_base, class3> >::type
#define MSE_FIRST_OR_PLACEHOLDER_IF_A_BASE_OF_SECOND(tag_base, class2, class3) std::conditional<!std::is_base_of<tag_base, class2>::value, tag_base, mse::impl::TPlaceHolder<tag_base, class3> >::type
#define MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(tag_base, class2, class3) std::conditional<std::is_base_of<tag_base, class2>::value, tag_base, mse::impl::TPlaceHolder<tag_base, class3> >::type

#define MSE_INHERIT_ASYNC_TAG_BASE_SET_FROM(class2, class3) \
	public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::AsyncNotShareableTagBase, class2, class3) \
	, public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::AsyncNotPassableTagBase, class2, class3)

#define MSE_INHERIT_COMMON_ITERATOR_TAG_BASE_SET_FROM(class2, class3) \
	MSE_INHERIT_ASYNC_TAG_BASE_SET_FROM(class2, class3) \
	, public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::ExclusivePointerTagBase, class2, class3)

#define MSE_INHERIT_COMMON_POINTER_TAG_BASE_SET_FROM(class2, class3) \
	MSE_INHERIT_ASYNC_TAG_BASE_SET_FROM(class2, class3) \
	, public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::StrongPointerTagBase, class2, class3) \
	, public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::NeverNullTagBase, class2, class3) \
	, public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::ExclusivePointerTagBase, class2, class3)

#define MSE_INHERIT_XSCOPE_TAG_BASE_SET_FROM(class2, class3) \
	public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::ReferenceableByScopePointerTagBase, class2, class3) \
	, public MSE_FIRST_OR_PLACEHOLDER_IF_NOT_A_BASE_OF_SECOND(mse::us::impl::ContainsNonOwningScopeReferenceTagBase, class2, class3)

#define MSE_INHERIT_COMMON_XSCOPE_ITERATOR_TAG_BASE_SET_FROM(class2, class3) \
	MSE_INHERIT_XSCOPE_TAG_BASE_SET_FROM(class2, class3) \
	, MSE_INHERIT_COMMON_ITERATOR_TAG_BASE_SET_FROM(class2, class3)

#define MSE_INHERIT_COMMON_XSCOPE_POINTER_TAG_BASE_SET_FROM(class2, class3) \
	MSE_INHERIT_XSCOPE_TAG_BASE_SET_FROM(class2, class3) \
	, MSE_INHERIT_COMMON_POINTER_TAG_BASE_SET_FROM(class2, class3)

#define MSE_INHERIT_COMMON_XSCOPE_OBJ_TAG_BASE_SET_FROM(class2, class3) MSE_INHERIT_COMMON_XSCOPE_POINTER_TAG_BASE_SET_FROM(class2, class3)

	namespace rsv {
		inline void suppress_check_directive() {}
#define MSE_SUPPRESS_CHECK_IN_XSCOPE \
		mse::rsv::suppress_check_directive();

#define MSE_IMPL_SUPPRESS_CHECK_IN_DECLSCOPE_MERGE_(a,b)  a##b
#define MSE_IMPL_SUPPRESS_CHECK_IN_DECLSCOPE_LABEL_(a) MSE_IMPL_SUPPRESS_CHECK_IN_DECLSCOPE_MERGE_(mse_suppress_check_directive_, a)
#define MSE_IMPL_SUPPRESS_CHECK_IN_DECLSCOPE_UNIQUE_NAME MSE_IMPL_SUPPRESS_CHECK_IN_DECLSCOPE_LABEL_(__LINE__)

#define MSE_SUPPRESS_CHECK_IN_DECLSCOPE \
		static void MSE_IMPL_SUPPRESS_CHECK_IN_DECLSCOPE_UNIQUE_NAME() {}
	}


	namespace us {
		namespace impl {
			template<typename _Ty>
			class TPointerID {};

			/* TPointer is just a wrapper for native pointers that can act as a base class. */
			template<typename _Ty, typename _TID = TPointerID<_Ty>>
			class TPointer : public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {
			public:
				TPointer() : m_ptr(nullptr) {}
				TPointer(_Ty* ptr) : m_ptr(ptr) { note_value_assignment(); }
				TPointer(const TPointer<_Ty, _TID>& src) : m_ptr(src.m_ptr) { note_value_assignment(); }
				template<class _Ty2, class = typename std::enable_if<std::is_convertible<_Ty2 *, _Ty *>::value || std::is_same<const _Ty2, _Ty>::value, void>::type>
				TPointer(const TPointer<_Ty2, TPointerID<_Ty2> >& src_cref) : m_ptr(src_cref.m_ptr) { note_value_assignment(); }
				virtual ~TPointer() {}

				void raw_pointer(_Ty* ptr) { note_value_assignment(); m_ptr = ptr; }
				_Ty* raw_pointer() const { return m_ptr; }
				_Ty* get() const { return m_ptr; }
				_Ty& operator*() const {
					assert_initialized();
#ifndef NDEBUG
					if (nullptr == m_ptr) {
						MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TPointer"));
					}
#endif // !NDEBUG
					return (*m_ptr);
				}
				_Ty* operator->() const {
					assert_initialized();
#ifndef NDEBUG
					if (nullptr == m_ptr) {
						MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TPointer"));
					}
#endif // !NDEBUG
					return m_ptr;
				}
				TPointer<_Ty, _TID>& operator=(_Ty* ptr) {
					note_value_assignment();
					m_ptr = ptr;
					return (*this);
				}
				TPointer<_Ty, _TID>& operator=(const TPointer<_Ty, _TID>& _Right_cref) {
					note_value_assignment();
					m_ptr = _Right_cref.m_ptr;
					return (*this);
				}
				template<class _Ty2, class = typename std::enable_if<std::is_convertible<_Ty2 *, _Ty *>::value || std::is_same<const _Ty2, _Ty>::value, void>::type>
				TPointer<_Ty, _TID>& operator=(const TPointer<_Ty2, TPointerID<_Ty2> >& _Right_cref) {
					note_value_assignment();
					m_ptr = _Right_cref.m_ptr;
					return (*this);
				}
				bool operator==(const _Ty* _Right_cref) const { assert_initialized(); return (_Right_cref == m_ptr); }
				bool operator!=(const _Ty* _Right_cref) const { /*assert_initialized();*/ return (!((*this) == _Right_cref)); }
				bool operator==(const TPointer<_Ty, _TID> &_Right_cref) const { /*assert_initialized();*/ return (_Right_cref == m_ptr); }
				bool operator!=(const TPointer<_Ty, _TID> &_Right_cref) const { /*assert_initialized();*/ return (!((*this) == _Right_cref)); }

				bool operator!() const { assert_initialized(); return (!m_ptr); }
				operator bool() const {
					assert_initialized();
					return (m_ptr != nullptr);
				}

				explicit operator _Ty*() const {
					assert_initialized();
#ifdef NATIVE_PTR_DEBUG_HELPER1
					if (nullptr == m_ptr) {
						int q = 3; /* just a line of code for putting a debugger break point */
					}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
					return m_ptr;
				}

				_Ty* m_ptr;

#ifdef MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
				void note_value_assignment() { m_initialized = true; }
				void assert_initialized() const { assert(m_initialized); }
				bool m_initialized = false;
#else // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
				void note_value_assignment() {}
				void assert_initialized() const {}
#endif // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
			};

			template<typename _Ty, typename _TID = TPointerID<_Ty>>
			class TPointerForLegacy : public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {
			public:
				TPointerForLegacy() : m_ptr(nullptr) {}
				TPointerForLegacy(_Ty* ptr) : m_ptr(ptr) { note_value_assignment(); }
				template<class _Ty2, class = typename std::enable_if<std::is_convertible<_Ty2 *, _Ty *>::value || std::is_same<const _Ty2, _Ty>::value, void>::type>
				TPointerForLegacy(const TPointerForLegacy<_Ty2, _TID>& src_cref) : m_ptr(src_cref.m_ptr) { note_value_assignment(); }
				virtual ~TPointerForLegacy() {}

				void raw_pointer(_Ty* ptr) { note_value_assignment(); m_ptr = ptr; }
				_Ty* raw_pointer() const { return m_ptr; }
				_Ty* get() const { return m_ptr; }
				_Ty& operator*() const {
					assert_initialized();
#ifndef NDEBUG
					if (nullptr == m_ptr) {
						MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TPointerForLegacy"));
					}
#endif // !NDEBUG
					return (*m_ptr);
				}
				_Ty* operator->() const {
					assert_initialized();
#ifndef NDEBUG
					if (nullptr == m_ptr) {
						MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TPointerForLegacy"));
					}
#endif // !NDEBUG
					return m_ptr;
				}
				TPointerForLegacy<_Ty, _TID>& operator=(_Ty* ptr) {
					note_value_assignment();
					m_ptr = ptr;
					return (*this);
				}
				bool operator==(const _Ty* _Right_cref) const { assert_initialized(); return (_Right_cref == m_ptr); }
				bool operator!=(const _Ty* _Right_cref) const { /*assert_initialized();*/ return (!((*this) == _Right_cref)); }
				bool operator==(const TPointerForLegacy<_Ty, _TID> &_Right_cref) const { /*assert_initialized();*/ return (m_ptr == _Right_cref); }
				bool operator!=(const TPointerForLegacy<_Ty, _TID> &_Right_cref) const { /*assert_initialized();*/ return (!((*this) == _Right_cref)); }

				bool operator!() const { assert_initialized(); return (!m_ptr); }
				operator bool() const {
					assert_initialized();
					return (m_ptr != nullptr);
				}

				operator _Ty*() const {
					assert_initialized();
#ifdef NATIVE_PTR_DEBUG_HELPER1
					if (nullptr == m_ptr) {
						int q = 3; /* just a line of code for putting a debugger break point */
					}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
					return m_ptr;
				}

				_Ty* m_ptr;

#ifdef MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
				void note_value_assignment() { m_initialized = true; }
				void assert_initialized() const { assert(m_initialized); }
				bool m_initialized = false;
#else // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
				void note_value_assignment() {}
				void assert_initialized() const {}
#endif // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
			};
		}
	}


	namespace us {
		namespace impl {
			class CSaferPtrBase : public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {
			public:
				/* spb_set_to_null() needs to be available even when the smart pointer is const, because the object it points to may become
				invalid (deleted). */
				virtual void spb_set_to_null() const = 0;
			};
		}

#ifndef NDEBUG
#ifndef MSE_SUPPRESS_TSAFERPTR_CHECK_USE_BEFORE_SET
#define MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
#endif // !MSE_SUPPRESS_TSAFERPTR_CHECK_USE_BEFORE_SET
#endif // !NDEBUG

		/* TSaferPtr behaves similar to, and is largely compatible with, native pointers. It's a bit safer in that it initializes to
		nullptr by default and checks for attempted dereference of null pointers. */
		template<typename _Ty>
		class TSaferPtr : public impl::CSaferPtrBase {
		public:
			TSaferPtr() : m_ptr(nullptr) {}
			TSaferPtr(_Ty* ptr) : m_ptr(ptr) { note_value_assignment(); }
			TSaferPtr(const TSaferPtr<_Ty>& src) : m_ptr(src.m_ptr) { note_value_assignment(); }
			template<class _Ty2, class = typename std::enable_if<std::is_convertible<_Ty2 *, _Ty *>::value, void>::type>
			TSaferPtr(const TSaferPtr<_Ty2>& src_cref) : m_ptr(src_cref.m_ptr) { note_value_assignment(); }
			virtual ~TSaferPtr() {}

			virtual void spb_set_to_null() const { m_ptr = nullptr; }

			void raw_pointer(_Ty* ptr) { note_value_assignment(); m_ptr = ptr; }
			_Ty* raw_pointer() const { return m_ptr; }
			_Ty* get() const { return m_ptr; }
			_Ty& operator*() const {
				assert_initialized();
#ifndef MSE_DISABLE_TSAFERPTR_CHECKS
				if (nullptr == m_ptr) {
					MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TSaferPtr"));
				}
#endif /*MSE_DISABLE_TSAFERPTR_CHECKS*/
				return (*m_ptr);
			}
			_Ty* operator->() const {
				assert_initialized();
#ifndef MSE_DISABLE_TSAFERPTR_CHECKS
				if (nullptr == m_ptr) {
					MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TSaferPtr"));
				}
#endif /*MSE_DISABLE_TSAFERPTR_CHECKS*/
				return m_ptr;
			}
			TSaferPtr<_Ty>& operator=(_Ty* ptr) {
				note_value_assignment();
				m_ptr = ptr;
				return (*this);
			}
			TSaferPtr<_Ty>& operator=(const TSaferPtr<_Ty>& _Right_cref) {
				note_value_assignment();
				m_ptr = _Right_cref.m_ptr;
				return (*this);
			}
			bool operator==(const _Ty* _Right_cref) const { assert_initialized(); return (_Right_cref == m_ptr); }
			bool operator!=(const _Ty* _Right_cref) const { assert_initialized(); return (!((*this) == _Right_cref)); }
			bool operator==(const TSaferPtr<_Ty> &_Right_cref) const { assert_initialized(); return (_Right_cref == m_ptr); }
			bool operator!=(const TSaferPtr<_Ty> &_Right_cref) const { assert_initialized(); return (!((*this) == _Right_cref)); }

			bool operator!() const { assert_initialized(); return (!m_ptr); }
			operator bool() const {
				assert_initialized();
				return (m_ptr != nullptr);
			}

			explicit operator _Ty*() const {
				assert_initialized();
#ifdef NATIVE_PTR_DEBUG_HELPER1
				if (nullptr == m_ptr) {
					int q = 3; /* just a line of code for putting a debugger break point */
				}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
				return m_ptr;
			}

			/* m_ptr needs to be mutable so that it can be set to nullptr when the object it points to is no longer valid (i.e. has
			been deleted) even in cases when this smart pointer is const. */
			mutable _Ty* m_ptr;

#ifdef MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
			void note_value_assignment() { m_initialized = true; }
			void assert_initialized() const { assert(m_initialized); }
			bool m_initialized = false;
#else // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
			void note_value_assignment() {}
			void assert_initialized() const {}
#endif // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
		};

		/* TSaferPtrForLegacy is similar to TSaferPtr, but more readily converts to a native pointer implicitly. So when replacing
		native pointers with safer pointers in legacy code, fewer code changes (explicit casts) may be required when using this
		template. */
		template<typename _Ty>
		class TSaferPtrForLegacy : public impl::CSaferPtrBase {
		public:
			TSaferPtrForLegacy() : m_ptr(nullptr) {}
			TSaferPtrForLegacy(_Ty* ptr) : m_ptr(ptr) { note_value_assignment(); }
			template<class _Ty2, class = typename std::enable_if<std::is_convertible<_Ty2 *, _Ty *>::value, void>::type>
			TSaferPtrForLegacy(const TSaferPtrForLegacy<_Ty2>& src_cref) : m_ptr(src_cref.m_ptr) { note_value_assignment(); }
			virtual ~TSaferPtrForLegacy() {}

			virtual void spb_set_to_null() const { m_ptr = nullptr; }

			void raw_pointer(_Ty* ptr) { note_value_assignment(); m_ptr = ptr; }
			_Ty* raw_pointer() const { return m_ptr; }
			_Ty* get() const { return m_ptr; }
			_Ty& operator*() const {
				assert_initialized();
#ifndef MSE_DISABLE_TSAFERPTRFORLEGACY_CHECKS
				if (nullptr == m_ptr) {
					MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TSaferPtrForLegacy"));
				}
#endif /*MSE_DISABLE_TSAFERPTRFORLEGACY_CHECKS*/
				return (*m_ptr);
			}
			_Ty* operator->() const {
				assert_initialized();
#ifndef MSE_DISABLE_TSAFERPTRFORLEGACY_CHECKS
				if (nullptr == m_ptr) {
					MSE_THROW(primitives_null_dereference_error("attempt to dereference null pointer - mse::TSaferPtrForLegacy"));
				}
#endif /*MSE_DISABLE_TSAFERPTRFORLEGACY_CHECKS*/
				return m_ptr;
			}
			TSaferPtrForLegacy<_Ty>& operator=(_Ty* ptr) {
				note_value_assignment();
				m_ptr = ptr;
				return (*this);
			}
			//operator bool() const { return m_ptr; }

			operator _Ty*() const {
				assert_initialized();
#ifdef NATIVE_PTR_DEBUG_HELPER1
				if (nullptr == m_ptr) {
					int q = 3; /* just a line of code for putting a debugger break point */
				}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
				return m_ptr;
			}

			/* m_ptr needs to be mutable so that it can be set to nullptr when the object it points to is no longer valid (i.e. has
			been deleted) even in cases when this smart pointer is const. */
			mutable _Ty* m_ptr;

#ifdef MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
			void note_value_assignment() { m_initialized = true; }
			void assert_initialized() const { assert(m_initialized); }
			bool m_initialized = false;
#else // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
			void note_value_assignment() {}
			void assert_initialized() const {}
#endif // MSE_TSAFERPTR_CHECK_USE_BEFORE_SET
		};
	}
	/* Deprecated in this namespace. Use mse::us::TSaferPtr<> instead. */
	template<typename _Ty> using TSaferPtr = mse::us::TSaferPtr<_Ty>;
	/* Deprecated in this namespace. Use mse::us::TSaferPtrForLegacy<> instead. */
	template<typename _Ty> using TSaferPtrForLegacy = mse::us::TSaferPtrForLegacy<_Ty>;

#if defined(MSE_SOME_POINTER_TYPE_IS_DISABLED)
	template<typename _Ty> auto pointer_to(_Ty& _X) { return &_X; }

	template<typename _Ty>
	auto pointer_to(_Ty&& _X) {
		/* Some compilers (prudently) don't allow you to obtain a pointer to an r-value. But since it's safe and supported
		for the library's safe elements, for compatibility reasons, here we enable you to do it when the those elements are
		"disabled" (i.e. replaced with their native counterparts). */
		const _Ty& X2 = _X;
		return &X2;
	}
#else /*defined(MSE_SOME_POINTER_TYPE_IS_DISABLED)*/

	namespace impl {
		template<typename _Ty, class = typename std::enable_if<(!std::is_pointer<_Ty>::value), void>::type>
		void T_valid_if_not_raw_pointer_msepointerbasics() {}
	}

	template<typename _Ty>
	auto pointer_to(const _Ty& _X) {
		impl::T_valid_if_not_raw_pointer_msepointerbasics<decltype(&_X)>();
		return &_X;
	}

	template<typename _Ty>
	auto pointer_to(_Ty&& _X) -> decltype(&std::forward<_Ty>(_X)) {
		const _Ty& X2 = _X;
		impl::T_valid_if_not_raw_pointer_msepointerbasics<decltype(&X2)>();
		return &std::forward<_Ty>(_X);
	}
#endif /*defined(MSE_SOME_POINTER_TYPE_IS_DISABLED)*/

	template<typename _Ty>
	_Ty* not_null_from_nullable(const _Ty* src) {
		return src;
	}


	template <class _TTargetType, class _TLeasePointerType> class TSyncWeakFixedConstPointer;

	/* If, for example, you want a safe pointer to a member of a registered pointer target, you can use a
	TSyncWeakFixedPointer to store a copy of the registered pointer along with the pointer targeting the
	member. */
	template <class _TTargetType, class _TLeasePointerType>
	class TSyncWeakFixedPointer : public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {
	public:
		TSyncWeakFixedPointer(const TSyncWeakFixedPointer&) = default;
		template<class _TLeasePointerType2, class = typename std::enable_if<std::is_convertible<_TLeasePointerType2, _TLeasePointerType>::value, void>::type>
		TSyncWeakFixedPointer(const TSyncWeakFixedPointer<_TTargetType, _TLeasePointerType2>&src) : m_target_pointer(std::addressof(*src)), m_lease_pointer(src.lease_pointer()) {}
		_TTargetType& operator*() const {
			/*const auto &test_cref =*/ *m_lease_pointer; // this should throw if m_lease_pointer is no longer valid
			return (*m_target_pointer);
		}
		_TTargetType* operator->() const {
			/*const auto &test_cref =*/ *m_lease_pointer; // this should throw if m_lease_pointer is no longer valid
			return m_target_pointer;
		}

		bool operator==(const _TTargetType* _Right_cref) const { return (_Right_cref == m_target_pointer); }
		bool operator!=(const _TTargetType* _Right_cref) const { return (!((*this) == _Right_cref)); }
		bool operator==(const TSyncWeakFixedPointer &_Right_cref) const { return (_Right_cref == m_target_pointer); }
		bool operator!=(const TSyncWeakFixedPointer &_Right_cref) const { return (!((*this) == _Right_cref)); }
		bool operator==(const TSyncWeakFixedConstPointer<_TTargetType, _TLeasePointerType> &_Right_cref) const;
		bool operator!=(const TSyncWeakFixedConstPointer<_TTargetType, _TLeasePointerType> &_Right_cref) const;

		bool operator!() const { return (!m_target_pointer); }
		operator bool() const {
			return (m_target_pointer != nullptr);
		}

		explicit operator _TTargetType*() const {
#ifdef NATIVE_PTR_DEBUG_HELPER1
			if (nullptr == m_target_pointer) {
				int q = 3; /* just a line of code for putting a debugger break point */
			}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
			return m_target_pointer;
		}
		_TLeasePointerType lease_pointer() const { return (*this).m_lease_pointer; }

		template <class _TTargetType2, class _TLeasePointerType2>
		static TSyncWeakFixedPointer make(_TTargetType2& target, const _TLeasePointerType2& lease_pointer) {
			return TSyncWeakFixedPointer(target, lease_pointer);
		}

	private:
		TSyncWeakFixedPointer(_TTargetType& target/* often a struct member */, _TLeasePointerType lease_pointer/* usually a registered pointer */)
			: m_target_pointer(&target), m_lease_pointer(lease_pointer) {}
		TSyncWeakFixedPointer& operator=(const TSyncWeakFixedPointer& _Right_cref) = delete;

		_TTargetType* m_target_pointer;
		_TLeasePointerType m_lease_pointer;
		friend class TSyncWeakFixedConstPointer<_TTargetType, _TLeasePointerType>;
	};

	template <class _TTargetType, class _TLeasePointerType>
	TSyncWeakFixedPointer<_TTargetType, _TLeasePointerType> make_syncweak(_TTargetType& target, const _TLeasePointerType& lease_pointer) {
		return TSyncWeakFixedPointer<_TTargetType, _TLeasePointerType>::make(target, lease_pointer);
	}

	template <class _TTargetType, class _TLeasePointerType>
	class TSyncWeakFixedConstPointer : public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {
	public:
		TSyncWeakFixedConstPointer(const TSyncWeakFixedConstPointer&) = default;
		template<class _TLeasePointerType2, class = typename std::enable_if<std::is_convertible<_TLeasePointerType2, _TLeasePointerType>::value, void>::type>
		TSyncWeakFixedConstPointer(const TSyncWeakFixedConstPointer<_TTargetType, _TLeasePointerType2>&src) : m_target_pointer(std::addressof(*src)), m_lease_pointer(src.lease_pointer()) {}
		TSyncWeakFixedConstPointer(const TSyncWeakFixedPointer<_TTargetType, _TLeasePointerType>&src) : m_target_pointer(src.m_target_pointer), m_lease_pointer(src.m_lease_pointer) {}
		const _TTargetType& operator*() const {
			/*const auto &test_cref =*/ *m_lease_pointer; // this should throw if m_lease_pointer is no longer valid
			return (*m_target_pointer);
		}
		const _TTargetType* operator->() const {
			/*const auto &test_cref =*/ *m_lease_pointer; // this should throw if m_lease_pointer is no longer valid
			return m_target_pointer;
		}

		bool operator==(const _TTargetType* _Right_cref) const { return (_Right_cref == m_target_pointer); }
		bool operator!=(const _TTargetType* _Right_cref) const { return (!((*this) == _Right_cref)); }
		bool operator==(const TSyncWeakFixedConstPointer &_Right_cref) const { return (_Right_cref == m_target_pointer); }
		bool operator!=(const TSyncWeakFixedConstPointer &_Right_cref) const { return (!((*this) == _Right_cref)); }

		bool operator!() const { return (!m_target_pointer); }
		operator bool() const {
			return (m_target_pointer != nullptr);
		}

		explicit operator const _TTargetType*() const {
#ifdef NATIVE_PTR_DEBUG_HELPER1
			if (nullptr == m_target_pointer) {
				int q = 3; /* just a line of code for putting a debugger break point */
			}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
			return m_target_pointer;
		}
		_TLeasePointerType lease_pointer() const { return (*this).m_lease_pointer; }

		template <class _TTargetType2, class _TLeasePointerType2>
		static TSyncWeakFixedConstPointer make(const _TTargetType2& target, const _TLeasePointerType2& lease_pointer) {
			return TSyncWeakFixedConstPointer(target, lease_pointer);
		}

	private:
		TSyncWeakFixedConstPointer(const _TTargetType& target/* often a struct member */, _TLeasePointerType lease_pointer/* usually a registered pointer */)
			: m_target_pointer(&target), m_lease_pointer(lease_pointer) {}
		TSyncWeakFixedConstPointer& operator=(const TSyncWeakFixedConstPointer& _Right_cref) = delete;

		const _TTargetType* m_target_pointer;
		_TLeasePointerType m_lease_pointer;
	};

	template <class _TTargetType, class _TLeasePointerType>
	bool TSyncWeakFixedPointer<_TTargetType, _TLeasePointerType>::operator==(const TSyncWeakFixedConstPointer<_TTargetType, _TLeasePointerType> &_Right_cref) const { return (_Right_cref == m_target_pointer); }
	template <class _TTargetType, class _TLeasePointerType>
	bool TSyncWeakFixedPointer<_TTargetType, _TLeasePointerType>::operator!=(const TSyncWeakFixedConstPointer<_TTargetType, _TLeasePointerType> &_Right_cref) const { return (!((*this) == _Right_cref)); }

	template<class _TTargetType, class _Ty>
	TSyncWeakFixedPointer<_TTargetType, _Ty> make_pointer_to_member(_TTargetType& target, const _Ty &lease_pointer) {
		return TSyncWeakFixedPointer<_TTargetType, _Ty>::make(target, lease_pointer);
	}
	template<class _TTargetType, class _Ty>
	TSyncWeakFixedConstPointer<_TTargetType, _Ty> make_pointer_to_member(const _TTargetType& target, const _Ty &lease_pointer) {
		return TSyncWeakFixedConstPointer<_TTargetType, _Ty>::make(target, lease_pointer);
	}
	template<class _TTargetType, class _Ty>
	TSyncWeakFixedConstPointer<_TTargetType, _Ty> make_const_pointer_to_member(const _TTargetType& target, const _Ty &lease_pointer) {
		return TSyncWeakFixedConstPointer<_TTargetType, _Ty>::make(target, lease_pointer);
	}

	namespace impl {
		template<class _Ty, class _Ty2, class = typename std::enable_if<std::is_same<_Ty, _Ty2>::value>::type>
		static void T_valid_if_same_msepointerbasics() {}
		template<class _TLeasePointer, class _TMemberObjectPointer>
		static void make_pointer_to_member_v2_checks_msepointerbasics(const _TLeasePointer &/*lease_pointer*/, const _TMemberObjectPointer& member_object_ptr) {
			/* Check for possible problematic parameters. */
			if (!member_object_ptr) { MSE_THROW(std::logic_error("null member_object_ptr - make_pointer_to_member_v2_checks_msepointerbasics()")); }
			/*
			typedef typename std::remove_reference<decltype(*lease_pointer)>::type _TLeaseTarget;
			typedef typename std::remove_reference<decltype((*lease_pointer).*member_object_ptr)>::type _TTarget;
			_TTarget _TLeaseTarget::* l_member_object_ptr = member_object_ptr;
			typedef typename std::remove_reference<decltype(l_member_object_ptr)>::type _TMemberObjectPointer2;
			T_valid_if_same_msepointerbasics<const _TMemberObjectPointer2, const _TMemberObjectPointer>();
			*/
		}
	}
	template<class _TLeasePointer, class _TMemberObjectPointer>
	static auto make_pointer_to_member_v2(const _TLeasePointer &lease_pointer, const _TMemberObjectPointer& member_object_ptr) {
		impl::T_valid_if_not_an_xscope_type(lease_pointer);
		typedef typename std::remove_reference<decltype((*lease_pointer).*member_object_ptr)>::type _TTarget;
		impl::make_pointer_to_member_v2_checks_msepointerbasics(lease_pointer, member_object_ptr);
		return mse::TSyncWeakFixedPointer<_TTarget, _TLeasePointer>::make((*lease_pointer).*member_object_ptr, lease_pointer);
	}
	template<class _TLeasePointer, class _TMemberObjectPointer>
	static auto make_const_pointer_to_member_v2(const _TLeasePointer &lease_pointer, const _TMemberObjectPointer& member_object_ptr) {
		impl::T_valid_if_not_an_xscope_type(lease_pointer);
		typedef typename std::remove_reference<decltype((*lease_pointer).*member_object_ptr)>::type _TTarget;
		impl::make_pointer_to_member_v2_checks_msepointerbasics(lease_pointer, member_object_ptr);
		return mse::TSyncWeakFixedConstPointer<_TTarget, _TLeasePointer>::make((*lease_pointer).*member_object_ptr, lease_pointer);
	}
}

namespace std {
	template <class _TTargetType, class _TLeaseType>
	struct hash<mse::TSyncWeakFixedPointer<_TTargetType, _TLeaseType> > {	// hash functor
		typedef mse::TSyncWeakFixedPointer<_TTargetType, _TLeaseType> argument_type;
		typedef size_t result_type;
		size_t operator()(const mse::TSyncWeakFixedPointer<_TTargetType, _TLeaseType>& _Keyval) const {
			const _TTargetType* ptr1 = nullptr;
			if (_Keyval) {
				ptr1 = std::addressof(*_Keyval);
			}
			return (hash<const _TTargetType *>()(ptr1));
		}
	};
	template <class _TTargetType, class _TLeaseType>
	struct hash<mse::TSyncWeakFixedConstPointer<_TTargetType, _TLeaseType> > {	// hash functor
		typedef mse::TSyncWeakFixedConstPointer<_TTargetType, _TLeaseType> argument_type;
		typedef size_t result_type;
		size_t operator()(const mse::TSyncWeakFixedConstPointer<_TTargetType, _TLeaseType>& _Keyval) const {
			const _TTargetType* ptr1 = nullptr;
			if (_Keyval) {
				ptr1 = std::addressof(*_Keyval);
			}
			return (hash<const _TTargetType *>()(ptr1));
		}
	};
}

namespace mse {

	namespace us {
		namespace impl {
			class StrongPointerTagBase {
			public:
				void strong_pointer_tag() const {}
			};
			class StrongPointerAsyncNotShareableAndNotPassableTagBase : public StrongPointerTagBase, public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {};

			class NeverNullTagBase {
			public:
				void never_null_tag() const {}
			};
			class StrongPointerNeverNullTagBase : public StrongPointerTagBase, public NeverNullTagBase {};
			class StrongPointerNeverNullAsyncNotShareableAndNotPassableTagBase : public StrongPointerTagBase, public NeverNullTagBase, public mse::us::impl::AsyncNotShareableAndNotPassableTagBase {};

			class ExclusivePointerTagBase {
			public:
				void exclusive_pointer_tag() const {}
			};
			class StrongExclusivePointerTagBase : public StrongPointerTagBase, public ExclusivePointerTagBase {};

			class XScopeStructureLockGuardTagBase : public XScopeContainsNonOwningScopeReferenceTagBase, public StrongPointerAsyncNotShareableAndNotPassableTagBase {};
		}
	}

	namespace impl {
		template <typename T> struct is_shared_ptr : std::false_type {};
		template <typename T> struct is_shared_ptr<std::shared_ptr<T> > : std::true_type {};
		template <typename T> struct is_unique_ptr : std::false_type {};
		template <typename T> struct is_unique_ptr<std::unique_ptr<T> > : std::true_type {};

		template <typename _TStrongPointer> struct is_strong_ptr : std::conditional<
			(std::is_base_of<mse::us::impl::StrongPointerTagBase, _TStrongPointer>::value)
			|| (std::is_pointer<_TStrongPointer>::value)/* for when scope pointers are "disabled" */
			|| (is_shared_ptr<_TStrongPointer>::value)/* for when refcounting pointers are "disabled" */
			|| (is_unique_ptr<_TStrongPointer>::value)/* for when TXScopeOwningPointer<>s are "disabled" */
			, std::true_type, std::false_type>::type {};

		template<typename _TStrongPointer, class = typename std::enable_if<is_strong_ptr<_TStrongPointer>::value, void>::type>
		class is_valid_if_strong_pointer {
			public:
				static void no_op() {}
		};

		template<typename _TNeverNullPointer, class = typename std::enable_if<
			(std::is_base_of<mse::us::impl::NeverNullTagBase, _TNeverNullPointer>::value)
			|| (std::is_pointer<_TNeverNullPointer>::value)/* for when scope pointers are "disabled" */
			|| (is_shared_ptr<_TNeverNullPointer>::value)/* for when refcounting pointers are "disabled" */
			, void>::type>
			class is_valid_if_never_null_pointer {
			public:
				static void no_op() {}
		};

		template<typename _TStrongAndNeverNullPointer>
		class is_valid_if_strong_and_never_null_pointer : public is_valid_if_strong_pointer<_TStrongAndNeverNullPointer>, public is_valid_if_never_null_pointer<_TStrongAndNeverNullPointer> {
		public:
			static void no_op() {}
		};

		template<typename _Ty, class = typename std::enable_if<(std::is_base_of<mse::us::impl::ExclusivePointerTagBase, _Ty>::value), void>::type>
		class is_valid_if_exclusive_pointer {
		public:
			static void no_op() {}
		};

		template<typename _Ty, class = typename std::enable_if<(std::is_base_of<mse::us::impl::ExclusivePointerTagBase, _Ty>::value), void>::type>
		void T_valid_if_exclusive_pointer_msepointerbasics() {}
	}

	namespace us {
		template <class _TTargetType, class _TLeaseType> class TStrongFixedConstPointer;

		/* If, for example, you want an "owning" pointer to a member of a refcounting pointer target, you can use a
		TStrongFixedPointer to store a copy of the owning (refcounting) pointer along with the pointer targeting the
		member. */
		template <class _TTargetType, class _TLeaseType>
		class TStrongFixedPointer : public mse::us::impl::StrongPointerNeverNullAsyncNotShareableAndNotPassableTagBase {
		public:
			TStrongFixedPointer(const TStrongFixedPointer&) = default;
			template<class _TLeaseType2, class = typename std::enable_if<std::is_convertible<_TLeaseType2, _TLeaseType>::value, void>::type>
			TStrongFixedPointer(const TStrongFixedPointer<_TTargetType, _TLeaseType2>&src) : m_target_pointer(std::addressof(*src)), m_lease(src.lease()) {}
			virtual ~TStrongFixedPointer() {
				/* This is just a no-op function that will cause a compile error when _TLeaseType is not an eligible type. */
				mse::impl::is_valid_if_strong_pointer<_TLeaseType>::no_op();
			}
			_TTargetType& operator*() const {
				return (*m_target_pointer);
			}
			_TTargetType* operator->() const {
				return m_target_pointer;
			}

			bool operator==(const _TTargetType* _Right_cref) const { return (_Right_cref == m_target_pointer); }
			bool operator!=(const _TTargetType* _Right_cref) const { return (!((*this) == _Right_cref)); }
			bool operator==(const TStrongFixedPointer &_Right_cref) const { return (_Right_cref == m_target_pointer); }
			bool operator!=(const TStrongFixedPointer &_Right_cref) const { return (!((*this) == _Right_cref)); }
			bool operator==(const TStrongFixedConstPointer<_TTargetType, _TLeaseType> &_Right_cref) const;
			bool operator!=(const TStrongFixedConstPointer<_TTargetType, _TLeaseType> &_Right_cref) const;

			bool operator!() const { return (!m_target_pointer); }
			operator bool() const {
				return (m_target_pointer != nullptr);
			}

			explicit operator _TTargetType*() const {
#ifdef NATIVE_PTR_DEBUG_HELPER1
				if (nullptr == m_target_pointer) {
					int q = 3; /* just a line of code for putting a debugger break point */
				}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
				return m_target_pointer;
			}
			_TLeaseType lease() const { return (*this).m_lease; }

			template <class _TTargetType2, class _TLeaseType2>
			static TStrongFixedPointer make(_TTargetType2& target, const _TLeaseType2& lease) {
				return TStrongFixedPointer(target, lease);
			}
			template <class _TTargetType2, class _TLeaseType2>
			static TStrongFixedPointer make(_TTargetType2& target, _TLeaseType2&& lease) {
				return TStrongFixedPointer(target, std::forward<decltype(lease)>(lease));
			}

		protected:
			TStrongFixedPointer(_TTargetType& target/* often a struct member */, const _TLeaseType& lease/* usually a reference counting pointer */)
				: m_target_pointer(std::addressof(target)), m_lease(lease) {}
			TStrongFixedPointer(_TTargetType& target/* often a struct member */, _TLeaseType&& lease)
				: m_target_pointer(std::addressof(target)), m_lease(std::forward<decltype(lease)>(lease)) {}
		private:
			TStrongFixedPointer& operator=(const TStrongFixedPointer& _Right_cref) = delete;
			MSE_DEFAULT_OPERATOR_AMPERSAND_DECLARATION;

			_TTargetType* m_target_pointer;
			_TLeaseType m_lease;

			friend class TStrongFixedConstPointer<_TTargetType, _TLeaseType>;
		};

		template <class _TTargetType, class _TLeaseType>
		TStrongFixedPointer<_TTargetType, _TLeaseType> make_strong(_TTargetType& target, const _TLeaseType& lease) {
			return TStrongFixedPointer<_TTargetType, _TLeaseType>::make(target, lease);
		}
		template <class _TTargetType, class _TLeaseType>
		auto make_strong(_TTargetType& target, _TLeaseType&& lease) -> TStrongFixedPointer<_TTargetType, typename std::remove_reference<_TLeaseType>::type> {
			return TStrongFixedPointer<_TTargetType, typename std::remove_reference<_TLeaseType>::type>::make(target, std::forward<decltype(lease)>(lease));
		}

		template <class _TTargetType, class _TLeaseType>
		class TStrongFixedConstPointer : public mse::us::impl::StrongPointerNeverNullAsyncNotShareableAndNotPassableTagBase {
		public:
			TStrongFixedConstPointer(const TStrongFixedConstPointer&) = default;
			template<class _TLeaseType2, class = typename std::enable_if<std::is_convertible<_TLeaseType2, _TLeaseType>::value, void>::type>
			TStrongFixedConstPointer(const TStrongFixedConstPointer<_TTargetType, _TLeaseType2>&src) : m_target_pointer(std::addressof(*src)), m_lease(src.lease()) {}
			TStrongFixedConstPointer(const TStrongFixedPointer<_TTargetType, _TLeaseType>&src) : m_target_pointer(src.m_target_pointer), m_lease(src.m_lease) {}
			template<class _TLeaseType2, class = typename std::enable_if<std::is_convertible<_TLeaseType2, _TLeaseType>::value, void>::type>
			TStrongFixedConstPointer(const TStrongFixedPointer<_TTargetType, _TLeaseType2>&src) : m_target_pointer(std::addressof(*src)), m_lease(src.lease()) {}
			virtual ~TStrongFixedConstPointer() {
				/* This is just a no-op function that will cause a compile error when _TLeaseType is not an eligible type. */
				mse::impl::is_valid_if_strong_pointer<_TLeaseType>::no_op();
			}
			const _TTargetType& operator*() const {
				return (*m_target_pointer);
			}
			const _TTargetType* operator->() const {
				return m_target_pointer;
			}

			bool operator==(const _TTargetType* _Right_cref) const { return (_Right_cref == m_target_pointer); }
			bool operator!=(const _TTargetType* _Right_cref) const { return (!((*this) == _Right_cref)); }
			bool operator==(const TStrongFixedConstPointer &_Right_cref) const { return (_Right_cref == m_target_pointer); }
			bool operator!=(const TStrongFixedConstPointer &_Right_cref) const { return (!((*this) == _Right_cref)); }

			bool operator!() const { return (!m_target_pointer); }
			operator bool() const {
				return (m_target_pointer != nullptr);
			}

			explicit operator const _TTargetType*() const {
#ifdef NATIVE_PTR_DEBUG_HELPER1
				if (nullptr == m_target_pointer) {
					int q = 3; /* just a line of code for putting a debugger break point */
				}
#endif /*NATIVE_PTR_DEBUG_HELPER1*/
				return m_target_pointer;
			}
			_TLeaseType lease() const { return (*this).m_lease; }

			template <class _TTargetType2, class _TLeaseType2>
			static TStrongFixedConstPointer make(const _TTargetType2& target, const _TLeaseType2& lease) {
				return TStrongFixedConstPointer(target, lease);
			}
			template <class _TTargetType2, class _TLeaseType2>
			static TStrongFixedConstPointer make(const _TTargetType2& target, _TLeaseType2&& lease) {
				return TStrongFixedConstPointer(target, std::forward<decltype(lease)>(lease));
			}

		protected:
			TStrongFixedConstPointer(const _TTargetType& target/* often a struct member */, const _TLeaseType& lease/* usually a reference counting pointer */)
				: m_target_pointer(std::addressof(target)), m_lease(lease) {}
			TStrongFixedConstPointer(const _TTargetType& target/* often a struct member */, _TLeaseType&& lease)
				: m_target_pointer(std::addressof(target)), m_lease(std::forward<decltype(lease)>(lease)) {}
		private:
			TStrongFixedConstPointer& operator=(const TStrongFixedConstPointer& _Right_cref) = delete;

			const _TTargetType* m_target_pointer;
			_TLeaseType m_lease;
		};

		template <class _TTargetType, class _TLeaseType>
		TStrongFixedConstPointer<_TTargetType, _TLeaseType> make_const_strong(const _TTargetType& target, const _TLeaseType& lease) {
			return TStrongFixedConstPointer<_TTargetType, _TLeaseType>::make(target, lease);
		}
		template <class _TTargetType, class _TLeaseType>
		auto make_const_strong(const _TTargetType& target, _TLeaseType&& lease) -> TStrongFixedConstPointer<_TTargetType, typename std::remove_reference<_TLeaseType>::type> {
			return TStrongFixedConstPointer<_TTargetType, typename std::remove_reference<_TLeaseType>::type>::make(target, std::forward<decltype(lease)>(lease));
		}

		template <class _TTargetType, class _TLeaseType>
		bool TStrongFixedPointer<_TTargetType, _TLeaseType>::operator==(const TStrongFixedConstPointer<_TTargetType, _TLeaseType> &_Right_cref) const { return (_Right_cref == m_target_pointer); }
		template <class _TTargetType, class _TLeaseType>
		bool TStrongFixedPointer<_TTargetType, _TLeaseType>::operator!=(const TStrongFixedConstPointer<_TTargetType, _TLeaseType> &_Right_cref) const { return (!((*this) == _Right_cref)); }
	}
	template <class _TTargetType, class _TLeaseType>
	using TStrongFixedPointer MSE_DEPRECATED = us::TStrongFixedPointer<_TTargetType, _TLeaseType>;
	template <class _TTargetType, class _TLeaseType>
	using TStrongFixedConstPointer MSE_DEPRECATED = us::TStrongFixedConstPointer<_TTargetType, _TLeaseType>;
	template <class _TTargetType, class _TLeaseType>
	MSE_DEPRECATED auto make_strong(_TTargetType& target, const _TLeaseType& lease) { return us::make_strong(target, lease); }
	template <class _TTargetType, class _TLeaseType>
	MSE_DEPRECATED auto make_strong(_TTargetType& target, _TLeaseType&& lease) { return us::make_strong(target, std::forward<decltype(lease)>(lease)); }
	template <class _TTargetType, class _TLeaseType>
	MSE_DEPRECATED auto make_const_strong(_TTargetType& target, const _TLeaseType& lease) { return us::make_const_strong(target, lease); }
	template <class _TTargetType, class _TLeaseType>
	MSE_DEPRECATED auto make_const_strong(_TTargetType& target, _TLeaseType&& lease) { return us::make_const_strong(target, std::forward<decltype(lease)>(lease)); }
}

namespace std {
	template <class _TTargetType, class _TLeaseType>
	struct hash<mse::us::TStrongFixedPointer<_TTargetType, _TLeaseType> > {	// hash functor
		typedef mse::us::TStrongFixedPointer<_TTargetType, _TLeaseType> argument_type;
		typedef size_t result_type;
		size_t operator()(const mse::us::TStrongFixedPointer<_TTargetType, _TLeaseType>& _Keyval) const {
			const _TTargetType* ptr1 = nullptr;
			if (_Keyval) {
				ptr1 = std::addressof(*_Keyval);
			}
			return (hash<const _TTargetType *>()(ptr1));
		}
	};
	template <class _TTargetType, class _TLeaseType>
	struct hash<mse::us::TStrongFixedConstPointer<_TTargetType, _TLeaseType> > {	// hash functor
		typedef mse::us::TStrongFixedConstPointer<_TTargetType, _TLeaseType> argument_type;
		typedef size_t result_type;
		size_t operator()(const mse::us::TStrongFixedConstPointer<_TTargetType, _TLeaseType>& _Keyval) const {
			const _TTargetType* ptr1 = nullptr;
			if (_Keyval) {
				ptr1 = std::addressof(*_Keyval);
			}
			return (hash<const _TTargetType *>()(ptr1));
		}
	};
}

namespace mse {
	namespace us {
		namespace impl {

#ifdef _MSC_VER
#pragma warning( push )  
#pragma warning( disable : 4127 )
#endif /*_MSC_VER*/

			/* CSAllocRegistry essentially just maintains a list of all objects allocated by a registered "new" call and not (yet)
			subsequently deallocated with a corresponding registered delete. */
			class CSAllocRegistry {
			public:
				CSAllocRegistry() {}
				~CSAllocRegistry() {}
				bool registerPointer(void *alloc_ptr) {
					if (nullptr == alloc_ptr) { return true; }
					{
						if (1 <= sc_fs1_max_objects) {
							/* We'll add this object to fast storage. */
							if (sc_fs1_max_objects == m_num_fs1_objects) {
								/* Too many objects. We're gonna move the oldest object to slow storage. */
								moveObjectFromFastStorage1ToSlowStorage(0);
							}
							auto& fs1_object_ref = m_fs1_objects[m_num_fs1_objects];
							fs1_object_ref = alloc_ptr;
							m_num_fs1_objects += 1;
							return true;
						}
						else {
							/* Add the mapping to slow storage. */
							std::unordered_set<CFS1Object>::value_type item(alloc_ptr);
							m_pointer_set.insert(item);
						}
					}
					return true;
				}
				bool unregisterPointer(void *alloc_ptr) {
					if (nullptr == alloc_ptr) { return true; }
					bool retval = false;
					{
						/* check if the object is in "fast storage 1" first */
						for (int i = (m_num_fs1_objects - 1); i >= 0; i -= 1) {
							if (alloc_ptr == m_fs1_objects[i]) {
								removeObjectFromFastStorage1(i);
								return true;
							}
						}

						/* The object was not in "fast storage 1". It's proably in "slow storage". */
						auto num_erased = m_pointer_set.erase(alloc_ptr);
						if (1 <= num_erased) {
							retval = true;
						}
					}
					return retval;
				}
				bool registerPointer(const void *alloc_ptr) { return (*this).registerPointer(const_cast<void *>(alloc_ptr)); }
				bool unregisterPointer(const void *alloc_ptr) { return (*this).unregisterPointer(const_cast<void *>(alloc_ptr)); }
				void reserve_space_for_one_more() {
					/* The purpose of this function is to ensure that the next call to registerPointer() won't
					need to allocate more memory, and thus won't have any chance of throwing an exception due to
					memory allocation failure. */
					m_pointer_set.reserve(m_pointer_set.size() + 1);
				}

				bool isEmpty() const { return ((0 == m_num_fs1_objects) && (0 == m_pointer_set.size())); }

			private:
				/* So this tracker stores the allocation pointers in either "fast storage1" or "slow storage". The code for
				"fast storage1" is ugly. The code for "slow storage" is more readable. */
				void removeObjectFromFastStorage1(int fs1_obj_index) {
					for (int j = fs1_obj_index; j < (m_num_fs1_objects - 1); j += 1) {
						m_fs1_objects[j] = m_fs1_objects[j + 1];
					}
					m_num_fs1_objects -= 1;
				}
				void moveObjectFromFastStorage1ToSlowStorage(int fs1_obj_index) {
					auto& fs1_object_ref = m_fs1_objects[fs1_obj_index];
					/* First we're gonna copy this object to slow storage. */
					//std::unordered_set<CFS1Object>::value_type item(fs1_object_ref);
					m_pointer_set.insert(fs1_object_ref);
					/* Then we're gonna remove the object from fast storage */
					removeObjectFromFastStorage1(fs1_obj_index);
				}

				typedef void* CFS1Object;

#ifndef MSE_SALLOC_REGISTRY_FS1_MAX_OBJECTS
#define MSE_SALLOC_REGISTRY_FS1_MAX_OBJECTS 8/* Arbitrary. The optimal number depends on how slow "slow storage" is. */
#endif // !MSE_SALLOC_REGISTRY_FS1_MAX_OBJECTS
				MSE_CONSTEXPR static const int sc_fs1_max_objects = MSE_SALLOC_REGISTRY_FS1_MAX_OBJECTS;
				CFS1Object m_fs1_objects[sc_fs1_max_objects];
				int m_num_fs1_objects = 0;

				/* "slow storage" */
				std::unordered_set<CFS1Object> m_pointer_set;
			};

#ifdef _MSC_VER
#pragma warning( pop )  
#endif /*_MSC_VER*/

			template<typename _Ty>
			inline CSAllocRegistry& tlSAllocRegistry_ref() {
				thread_local static CSAllocRegistry tlSAllocRegistry;
				return tlSAllocRegistry;
			}
		}
	}
}

#ifdef __clang__
#pragma clang diagnostic pop
#else /*__clang__*/
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif /*__GNUC__*/
#endif /*__clang__*/

#ifdef _MSC_VER
#pragma warning( pop )  
#endif /*_MSC_VER*/

#ifndef MSE_PUSH_MACRO_NOT_SUPPORTED
#pragma pop_macro("MSE_THROW")
#endif // !MSE_PUSH_MACRO_NOT_SUPPORTED

#endif /*ndef MSEPOINTERBASICS_H*/
