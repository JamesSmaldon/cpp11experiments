#pragma once

#include <tuple>

/*
 * Following simple C++11 metaprogramming tutorial here:
 * http://www.pdimov.com/cpp2/simple_cxx11_metaprogramming.html
 */

/*
 * Previously everything had to be done using templated classes or structs
 *
 * This c++03 style template adds a pointer to the type you specialize it with.
 */
template<class T> struct add_pointer_cpp03 { typedef T* type; };
/*
 * Which can be used as follows. The typename keyword here is telling the compiler 
 * that we are referring to add_pointer_cpp03<int>::type is a type, rather than a
 * member of the struct. 
 */

/*
 * Dependent names are names where the compiler can't figure out exactly what the name 
 * referes too (a type, or something else) because the type is specified by a template 
 * parameter. For example:
 *
 * t::x *f;
 *
 * The above could be parsed differently depending on whether x is a static data member or 
 * a type. You don't know whether that's the case until the template is instantiated.
 * Therefore "x" is a dependent name, because it depends on what type is passed as the 
 * template parameter.
 *
 * In order to force the compiler to treat t::x as a type, you can use the typename keyword.
 * e.g.
 *
 * typename t::x *f;
 *
 * In the example this is used as follows:
 *
 * typedef typename add_pointer_cpp03<int>::type intp;
 *
 * However, the way this is used in the example is incorrect, in c++03 you couldn't use the 
 * typename keyword outside of a template (it's fine in c++11 tho)
 *
 * Since the specialisation is not a template, you can just do:
 */
typedef add_pointer_cpp03<int>::type intp_cpp03;

/*
 * in C++11, metafunctions can be template aliases. Aliases are very similar to typedefs, but 
 * can be templated. E.g.
 *
 * template <class T>
 * struct Alloc { };
 * template <class T>
 * using Vec = vector<T, Alloc<T>>;
 * Vec <int> v;
 *
 * So we could rewrite the above as:
 */
template <typename T>
using add_pointer_cpp11 = T*;

using intp_cpp11 = add_pointer_cpp11<int>;
template <typename T>
using add_pointer = add_pointer_cpp11<T>;

/*
 * Note that template aliases can also be passed as template template parameters.
 */

/* 
 * A basic data type representing a list of types.
 */
template <typename... T>
struct mp_list {};
//e.g. mp_list<int, void, char *, float> test_mp_list;

/*
 * A metafunction that renames list like template types to mp_lists, so that we can write our library against 
 * mp_list types.
 *
 * We want a template that convertes e.g. std::tuple<int, char, float> to mp_list<int, char, float>
 *
 * This declares our template.
 */
template <class A, template <class...> class B> 
struct mp_rename_impl;

/*
 * This is a partial specialisation of the above template (even tho it actually requires more parameters)
 *
 * I understand this as a specialisation that will match a fully specified variadic template as A, and 
 * then an unspecialised variadic template as B, which we then specialise with the types we've extracted 
 * from A as T in the alias "type".  
 */
template <template<class... > class A, class... T, template<class...> class B> 
struct mp_rename_impl<A<T...>, B>
{
    using type=B<T...>;
};

template <class A, template <class...> class B> 
using mp_rename = typename mp_rename_impl<A, B>::type;
using tuple_to_list = mp_rename<std::tuple<int, int, char>, mp_list>;

/*
 * A metafunction to return the size of and mp_list (e.g. the number of types passed as template parameters).
 *
 * All this does is apply the sizeof... operator to the variadic template parameters, and then create a type 
 * with the resulting value.
 */
template <class L> 
struct mp_size_impl;

template <template <class...> class L, class... T>
struct mp_size_impl<L<T...>>
{
    using type=std::integral_constant<std::size_t, sizeof...(T)>;
};

template <class L>
using mp_size = typename mp_size_impl<L>::type;

/*
 * A metafunction to take an variadic parameter pack and return its length (essentially aliasing std::integral_constant)
 */

template <class... L>
using mp_length = std::integral_constant<std::size_t, sizeof...(L)>;

/*
 * You can actually reimplement mp_size in terms of mp_length using mp_rename. this is because mp_size takes an mp_list and 
 * returns an mp_length. e.g.
 * mp_size<mp_list<int,char>> = mp_length<int,char>
 *
 * So we can just implement mp_size by renaming mp_list (or whatever type it happens to be), to mp_length.
 */
template <class L>
using mp_size_alt = mp_rename<L, mp_length>;

/*
 * Essentially, renaming is the same as applying a metafunction to the elements of the list. For convenience, we can define an apply 
 * metafunction in terms of rename.
 *
 * The F parameter needs to be a template template because we want to pass the metafunction without any parameters applied to it.
 */
template <template <class...> class F, class L>
using mp_apply = mp_rename<L, F>;

/*
 * A metafunction to transform a list of types by applying a function to each one. For example, you might apply add_pointer to each 
 * type in the list <int, char, float> to get <int *, char *, float*>
 *
 * To implement this, we need to be able to push types into a list, so let's add mp_push_front for this.
 *
 * Note that mp_push_front uses variadic templates to allow you to push any number of types onto the front of the list.
 */
template <class L, class T>
struct mp_push_front_impl;

template <template <class...> class L, class... T, class... U>
struct mp_push_front_impl<L<U...>, T...>
{
    using type=L<T..., U...>;
};

template <class T, class L>
using mp_push_front = typename mp_push_front_impl<T, L>::type;

/*
 * Now we can implement mp_transform. This uses the parameter pack expansion features of the language to expand the list, applying F to each element.
 * Note that this could also have been implemented via recursion, by creating a template specialisation for the base case (empty list) and the 
 * general case.
 */
template <template <class...> class F, class L>
struct mp_transform_impl;

template <template <class...> class F, class... T, template <class...> class L>
struct mp_transform_impl<F, L<T...>>
{
    using type=L<F<T>...>;
};

template <template <class...> class F, class L>
using mp_transform = typename mp_transform_impl<F, L>::type;

void templateMetaprogramming();
