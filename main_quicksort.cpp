#include <boost/mpl/comparison.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/size.hpp>
#include <type_traits>
#include <iostream>
#include <array>
#include "vector_print.h"

typedef boost::mpl::vector_c<int, 2, 1, 4, 5, 7, 5, 6> input_vector_type;

template <typename T> struct QuickSort {
  typedef typename boost::mpl::front<T>::type first_ele_type;
  typedef typename boost::mpl::pop_front<T>::type tail_type;

  typedef typename boost::mpl::copy_if<
    tail_type, 
    boost::mpl::less<
      boost::mpl::_1, 
      first_ele_type
    >
  >::type right_unsort_type;

  typedef typename boost::mpl::copy_if<
    tail_type,
    boost::mpl::greater_equal<
      boost::mpl::_1, 
      first_ele_type
    >
  >::type left_unsort_type;

  typedef typename boost::mpl::if_<
    typename boost::mpl::greater<
      typename boost::mpl::size<
        right_unsort_type
      >::type,
      boost::mpl::int_<1>
    >::type,
    QuickSort<right_unsort_type>, 
    right_unsort_type
  >::type::type right_sort_type;

  typedef typename boost::mpl::if_<
    typename boost::mpl::greater<
      typename boost::mpl::size<
        left_unsort_type
      >::type,
      boost::mpl::int_<1>
    >::type,
    QuickSort<left_unsort_type>, 
    left_unsort_type
  >::type::type left_sort_type;

  typedef typename boost::mpl::insert_range<
    right_sort_type, 
    typename boost::mpl::end<
      right_sort_type
    >::type,
    typename boost::mpl::push_front<
      left_sort_type,
      first_ele_type
    >::type
  >::type type;
};

typedef typename VectorPrint<
  typename QuickSort<input_vector_type>::type
>::type FinallyResult;

int main() {
  FinallyResult data;
  data.print();
  return 0;
}
