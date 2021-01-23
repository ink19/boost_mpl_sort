#ifndef __VECTOR_PRINT_H
#define __VECTOR_PRINT_H

#include <array>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/placeholders.hpp>
#include <iostream>
#include <boost/mpl/front.hpp>

template <typename T, T... _data> struct DataPrint {
  constexpr static std::array<T, sizeof...(_data)> data = {_data...};
  void print() {
    for (auto &item : data) {
      std::cout << item << ",";
    }
    std::cout << std::endl;
  }
};

template <typename T, typename S> struct DataPrintOp;

template <typename T, T... _data, typename S>
struct DataPrintOp<DataPrint<T, _data...>, S> {
  typedef DataPrint<T, _data..., S::value> type;
};

template<typename T>
struct VectorPrint {
  typedef typename boost::mpl::fold<
    T, 
    DataPrint<typename boost::mpl::front<T>::type::value_type>,
    DataPrintOp<boost::mpl::_1, boost::mpl::_2>
  >::type type;
};

#endif
