#include "vector_print.h"
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/bind.hpp>
#include <boost/mpl/reverse_iter_fold.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/unpack_args.hpp>
#include <boost/mpl/assert.hpp>


typedef boost::mpl::vector_c<int, 2, 1, 4, 5, 7, 5, 6, 1000, 213, 123, 1231, 21412> input_vector_type;

template<typename S, typename IndexA, typename IndexB>
struct swap_in_vector {
  typedef typename boost::mpl::at<S, IndexA>::type ItemAType;
  typedef typename boost::mpl::at<S, IndexB>::type ItemBType;

  // 删除第一个
  typedef typename boost::mpl::erase<
    S, 
    typename boost::mpl::advance<
      typename boost::mpl::begin<S>::type, 
      IndexA
    >::type
  >::type RemoveAType;

  // 增加第一个
  typedef typename boost::mpl::insert<
    RemoveAType, 
    typename boost::mpl::advance<
      typename boost::mpl::begin<RemoveAType>::type, 
      IndexA
    >::type,
    ItemBType
  >::type AddAType;

  // 删除第一个
  typedef typename boost::mpl::erase<
    AddAType, 
    typename boost::mpl::advance<
      typename boost::mpl::begin<AddAType>::type, 
      IndexB
    >::type
  >::type RemoveBType;

  typedef typename boost::mpl::insert<
    RemoveBType, 
    typename boost::mpl::advance<
      typename boost::mpl::begin<RemoveBType>::type, 
      IndexB
    >::type,
    ItemAType
  >::type type;
  // BOOST_MPL_ASSERT_MSG(false, Swap, (type));
};

template<typename S, typename StartIndexP>
struct MaxHeapify{
  // BOOST_MPL_ASSERT_MSG(false, Test, (S, StartIndexP));
  typedef S dS;
  typedef StartIndexP StartIndex;
  typedef typename boost::mpl::at<dS, StartIndex>::type ParentNodeType;
  typedef typename boost::mpl::plus<
    typename boost::mpl::times<
      StartIndex,
      boost::mpl::int_<2>
    >::type,
    boost::mpl::int_<1>
  >::type RightNodeIndex;
  typedef typename boost::mpl::plus<
    typename boost::mpl::times<
      StartIndex,
      boost::mpl::int_<2>
    >::type,
    boost::mpl::int_<2>
  >::type LeftNodeIndex;

  typedef typename boost::mpl::greater<
    boost::mpl::size<dS>, 
    RightNodeIndex
  >::type RightExistType;

  typedef typename boost::mpl::greater<
    typename boost::mpl::size<dS>::type, 
    LeftNodeIndex
  >::type LeftExistType;
  
  typedef typename boost::mpl::if_<
    LeftExistType,
    typename boost::mpl::eval_if<
      typename boost::mpl::greater<
        typename boost::mpl::eval_if<
          RightExistType,
          boost::mpl::at<dS, RightNodeIndex>,
          boost::mpl::int_<-1>
        >::type,
        typename boost::mpl::eval_if<
          LeftExistType,
          boost::mpl::at<dS, LeftNodeIndex>,
          boost::mpl::int_<-1>
        >::type
      >::type,
      RightNodeIndex,
      LeftNodeIndex
    >::type,
    typename boost::mpl::if_<
      RightExistType,
      RightNodeIndex,
      boost::mpl::int_<-1>
    >::type
  >::type BiggerNodeIndex;

  typedef typename boost::mpl::eval_if<
    typename boost::mpl::equal_to<
      BiggerNodeIndex,
      boost::mpl::int_<-1>
    >::type,
    boost::mpl::bool_<false>,
    boost::mpl::less_equal<
      ParentNodeType,
      typename boost::mpl::at<
        dS, BiggerNodeIndex
      >::type
    >
  >::type IsSwapType;

  typedef typename boost::mpl::eval_if<
    IsSwapType,
    swap_in_vector<dS, StartIndex, BiggerNodeIndex>,
    dS
  >::type SwapedS;

  typedef typename boost::mpl::eval_if<
    IsSwapType,
    MaxHeapify<SwapedS, BiggerNodeIndex>,
    SwapedS
  >::type type;
  // BOOST_MPL_ASSERT_MSG(StartIndexP::value != 0, Test, (SwapedS, BiggerNodeIndex, ParentNodeType));
  // BOOST_MPL_ASSERT_MSG((typename boost::mpl::equal_to<
  //     BiggerNodeIndex,
  //     boost::mpl::int_<-1>
  //   >::type), Test, );
};

template<typename S>
struct SetMaxHeapify {
  typedef typename boost::mpl::reverse_iter_fold<
    S,
    S,
    MaxHeapify<
      boost::mpl::placeholders::_1, 
      boost::mpl::distance< 
        typename boost::mpl::begin<S>::type,
        boost::mpl::placeholders::_2
      >
    >
  >::type type;
};

template<typename S>
struct SelectHeapify{
  typedef typename boost::mpl::front<S>::type FrontType;
  typedef typename boost::mpl::pop_front<S>::type TailType;
  typedef typename boost::mpl::back<S>::type BackType;
  typedef typename boost::mpl::pop_back<TailType>::type NoType;
  typedef typename boost::mpl::push_front<NoType, BackType>::type OverType;

  typedef typename MaxHeapify<OverType, boost::mpl::int_<0>>::type NextType;

  typedef typename boost::mpl::eval_if<
    typename boost::mpl::less_equal<
      typename boost::mpl::size<OverType>::type,
      boost::mpl::int_<1>
    >::type,
    OverType,
    SelectHeapify<NextType>
  >::type NextResultType;
  typedef typename boost::mpl::push_back<NextResultType, FrontType>::type type;
};

template<typename S>
struct HeapSort {
  typedef typename SetMaxHeapify<S>::type HeapedS;
  typedef typename SelectHeapify<HeapedS>::type type;
  // typedef S type;
};

typedef typename VectorPrint<
  typename HeapSort<input_vector_type>::type
>::type FinallyResult;

int main() {
  FinallyResult s;
  s.print();
  return 0;
}