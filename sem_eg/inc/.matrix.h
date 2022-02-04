// Copyright 2021 CSCE 240
#ifndef LECTURE10_INC_MATRIX_H_
#define LECTURE10_INC_MATRIX_H_


#include <cstddef>  // size_t
#include <initializer_list>
#include <iostream>
#include <thread>  // NOLINT
#include <vector>

namespace csce240 {


template <class T>
class Matrix {
  typedef std::vector<T> RowType;

 public:
  Matrix(std::initializer_list<std::initializer_list<T>> m);

  // Multiplies calling Matrix object's entries by double scalar
  //   NOTE: use operator for to generate new Matrix object
  void Scale(double);

  // Multiplies calling Matrix object by parameter's entries
  //   NOTE: use operator for to generate new Matrix object
  // Precondition: The calling object is an n x m matrix and the parameter
  //   object is an m x p matrix.
  // Returns: Given the precondition, an n x p matrix
  void Multiply(const Matrix<T>&);

  // As above
  //   Note: const method
  const Matrix operator*(const Matrix<T>& rhs) const;

  std::ostream& Extract(std::ostream*) const;

 private:
  static void ScaleRow(double scalar,
                       typename RowType::iterator begin,
                       typename RowType::iterator end);

  static void MultiplyRow(const std::vector<RowType>& lhs,
                          const std::vector<RowType>& rhs,
                          size_t row,
                          size_t col,
                          std::vector<RowType>* product);
  
  std::vector<RowType> rows_;
};

template <class T>
const Matrix<T> operator*(double lhs, const Matrix<T>& rhs);


// IMPLEMENTATION FOLLOWS


template <class T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> rows)
    : rows_(std::vector<RowType>(rows.begin(), rows.end())) {
  // empty
}


template <class T>
void Matrix<T>::Scale(double scalar) {
  std::vector<std::thread> threads(rows_.size());

  for (RowType &row : rows_)
    threads.push_back(std::thread(ScaleRow, scalar, row.begin(), row.end()));

  for (std::thread &thread : threads)
    if (thread.joinable())
      thread.join();
}


template <class T>
void Matrix<T>::ScaleRow(double scalar,
                         typename RowType::iterator begin,
                         typename RowType::iterator end) {
  for (; begin != end; ++begin)
    *begin *= scalar;
}


template <class T>
void Matrix<T>::Multiply(const Matrix<T>& that) {
  std::vector<RowType> rows(rows_.size(),
                            std::vector<T>(that.rows_.front().size()));

  std::vector<std::thread> threads(rows_.size() * that.rows_.front().size());

  // for each row in rows_
  for (size_t i = 0; i < rows_.size(); ++i)
    // for each column in that.rows_
    for (size_t ii = 0; ii < that.rows_.front().size(); ++ii)
      threads.push_back(
        std::thread(this->MultiplyRow, rows_, that.rows_, i, ii, &rows));

  for (std::thread &thread : threads)
    if (thread.joinable())
      thread.join();

  rows_ = rows;
}

template <class T>
void Matrix<T>::MultiplyRow(const std::vector<RowType>& lhs,
                            const std::vector<RowType>& rhs,
                            size_t row,
                            size_t col,
                            std::vector<RowType>* product) {
  T sum = 0.0;
  for (size_t i = 0 ; i < lhs.front().size(); ++i) {
    sum += lhs[row][i] * rhs[i][col];
  }
  product->at(row).at(col) = sum;
}


template <class T>
const Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const {
  Matrix<T> tmp = *this;
  tmp.Multiply(rhs);
  return tmp;
}


template <class T>
std::ostream& Matrix<T>::Extract(std::ostream* ostream_ptr) const {
  for (auto row = rows_.begin(); row != rows_.end(); ++row) {
    for (auto col = row->begin(); col != row->end(); ++col) {
      *ostream_ptr << *col;

      if (col + 1 != row->end())
        *ostream_ptr << '\t';
    }

    if (row + 1 != rows_.end())
        *ostream_ptr << '\n';
  }

  return *ostream_ptr;
}


template <class T>
const Matrix<T> operator*(double lhs, const Matrix<T>& rhs) {
  Matrix tmp = rhs;
  tmp.Scale(lhs);
  return tmp;
}


template <class T>
std::ostream& operator<<(std::ostream& lhs, const Matrix<T>& matrix) {
  return matrix.Extract(&lhs);
}

}  // namespace csce240

#endif  // LECTURE10_INC_MATRIX_H_
