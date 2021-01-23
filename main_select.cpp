#include "vector_print.h"
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/deref.hpp>
typedef boost::mpl::vector_c<int, 2, 1, 4, 5, 7, 5, 6> input_vector_type;

template<typename T>
struct SelectSort {
    typedef typename boost::mpl::min_element<T>::type min_iter_type;
    typedef typename boost::mpl::erase<T, min_iter_type, typename boost::mpl::next<min_iter_type>::type>::type leaf_vector;
    typedef typename boost::mpl::if_<
      typename boost::mpl::less_equal<
        typename boost::mpl::size<leaf_vector>::type,
        boost::mpl::int_<1>
      >::type,
      leaf_vector,
      SelectSort<leaf_vector>
    >::type::type next_type;
    typedef typename boost::mpl::push_front<
      next_type,
      typename boost::mpl::deref<min_iter_type>::type
    >::type type;
};


typedef typename VectorPrint<
  typename SelectSort<input_vector_type>::type
>::type FinallyResult;

int main() {
  FinallyResult t;
  t.print();
  return 0;
}