#ifndef ECO_TYPE_TRAITS_
#define ECO_TYPE_TRAITS_

namespace eco::user::inline cpp20 {

template <typename T>
struct value_type_traits
{
  using type = typename T::value_type;
};

template <typename T>
struct ssize_type_traits
{
  using type = typename T::ssize_type;
};

template <typename T>
struct weight_type_traits
{
  using type = typename T::weight_type;
};

template <typename T>
struct index_type_traits
{
  using type = typename T::index_type;
};

template <typename T>
struct iterator_type_traits
{
  using type = typename T::iterator_type;
};

template <typename T>
struct const_iterator_type_traits
{
  using type = typename T::const_iterator_type;
};

}

namespace eco::inline cpp20 {

template <typename T>
using value_t = typename user::value_type_traits<T>::type;

template <typename T>
using ssize_t = typename user::ssize_type_traits<T>::type;

template <typename T>
using weight_type_t = typename user::weight_type_traits<T>::type;

template <typename T>
using index_type_t = typename user::index_type_traits<T>::type;

template <typename T>
using iterator_t = typename user::iterator_type_traits<T>::type;

template <typename T>
using const_iterator_t = typename user::const_iterator_type_traits<T>::type;

}

#endif
