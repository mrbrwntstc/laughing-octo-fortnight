#ifndef __LINEAR_ALGEBRA_H__
#define __LINEAR_ALGEBRA_H__

#include <iostream>
#include <cstddef>
#include <array>
#include <cmath>
#include <cassert>
#include <random>
#include <vector>
#include <iomanip>  // For std::setw

namespace math_util
{
  // [lhs,rhs]
  double random_double(double lhs, double rhs)
  {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(lhs, std::nextafter(rhs, __DBL_MAX__));

    return dist(mt);
  }
}

namespace matrix
{
  template<std::size_t R, std::size_t C>
  struct instance
  {
    std::array<std::array<float, C>, R> _data;

    instance()
    {
      for(auto& row : _data)
        row.fill(0.f);
    }
    // matrix::instance<2, 3> mat2{{ {1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f} }};
    instance(std::initializer_list<std::initializer_list<float>> list)
    {
      auto iterator_row = list.begin();
      for(std::size_t index_row = 0; index_row < R && iterator_row != list.end(); index_row++, iterator_row++)
      {
        auto iterator_col = iterator_row->begin();
        for(std::size_t index_col = 0; index_col < C && iterator_col != iterator_row->end(); index_col++, iterator_col++)
          this->_data[index_row][index_col] = *iterator_col;
      }
    }

    // mat1[0][0] = 5.0f
    std::array<float, C>& operator[](std::size_t index_row)
    {
      assert(index_row < R);
      return this->_data[index_row];
    }

    const std::array<float,C>& operator[](std::size_t index_row) const
    {
      assert(index_row < R);
      return this->_data[index_row];
    }
  };

  template<std::size_t R, std::size_t C>
  std::ostream& operator<<(std::ostream& out, const instance<R, C>& mat)
  {
    int padding = 3;
    // Now print the matrix with each element aligned
    for (std::size_t i = 0; i < R; ++i)
    {
      out << "|";  // Start of a row
      for (std::size_t j = 0; j < C; ++j)
      {
          // Use std::setw to set the width for each number, ensuring alignment
          out << std::setw(padding) << mat._data[i][j];
      }
      out << " |" << std::endl;  // End of a row with new line
    }
    return out;
  }

  // 5.4 matrices - the transpose operation
  template<std::size_t R, std::size_t C>
  instance<C,R> transpose(const instance<R,C>& matrix)
  {
    instance<C,R> result;
    for(std::size_t row = 0; row < R; ++row)
      for(std::size_t col = 0; col < C; ++col)
        result[col][row] = matrix[row][col];
    
    return result;
  }
} // namespace matrix

namespace vector
{
  template<std::size_t D>
  // 2.2 Vector: Geometry and Algebra
  struct instance
  {
    std::array<float, D> _data;
    enum class vector_type {column, row} type;

    // constructors
    // the book assumes all vectors will be column vectors unless otherwise stated
    // ---
    instance() : type(vector_type::column) { _data.fill(0.f); }

    // vector<3> v = {1,2,3}
    instance(std::initializer_list<float> list) : type(vector_type::column)
    {
      std::size_t i = 0;
      for(auto val : list)
        _data[i++] = val;
    }
    // ---

    // member functions
    // ---
    const char* vector_type_str() const
    {
      switch(type)
      {
        case vector_type::column:
          return "column";
        case vector_type::row:
          return "row";
      }
    }
    // 2.3 Transpose Operation
    void transpose() const
    {
      this->type = (this->type == vector_type::column) ? vector_type::row : vector_type::column;
    }
    // 3.1 vector dot product: algebra ||v||^2
    float magnitude_squared() const
    {
      return dot_product(*this, *this);
    }
    float magnitude() const
    {
      return std::sqrtf(magnitude_squared());
    }
    float length() const
    {
      return this->magnitude();
    }
    // ---

    // member operator overloads
    // ---
    // setter
    float& operator[](std::size_t index)
    {
      assert(index < D);
      return _data[index];
    }
    // getter
    const float& operator[](std::size_t index) const
    {
      assert(index < D);
      return _data[index]; 
    }
    // v1 = -v1
    instance operator-() const
    {
      instance result;
      for(std::size_t i = 0; i < D; ++i)
        result[i] = -_data[i];
      
      return result;
    }
    // 2.4 Vector addition and subtraction
    // ---
    // v1 += v2 (v1 = v1 + v2)
    instance& operator+=(const instance& other)
    {
      assert(this->type == other.type); // cannot add a row and column vector together
      assert(this->_data.max_size() == other._data.max_size()); // both vectors must have the same number of dimensions
      for(std::size_t i = 0; i < D; ++i)
        this->_data[i] += other._data[i];
      
      return *this; // v1 += v2 += v3 += ...
    }
    // v1 -= v2 (v1 = v1 - v2)
    instance& operator-=(const instance& other)
    {
      return *this += -other;
    }
    // ---
    // 2.5 vector-scalar multiplication
    // ---
    instance operator*(float scalar) const
    {
      instance result;
      for(std::size_t i = 0; i < D; ++i)
        result[i] = this->_data[i] * scalar;
      
      return result;
    }
    // v1 *= scalar_float (v1 = v1 * scalar_float)
    instance& operator*=(float scalar)
    {
      for(std::size_t i = 0; i < D; ++i)
        this->_data[i] *= scalar;
      
      return *this;
    }
    // v1 /= scalar_float (v1 = v1 / scalar_float)
    instance& operator/=(float scalar)
    {
      return *this *= 1/scalar;
    }
    // ---
    // ---
  };

  // non-member operator overloads
  // ---
  template<std::size_t D>
  std::ostream& operator<<(std::ostream& out, const instance<D>& v)
  {
    out << '[';
    for (std::size_t i = 0; i < D; ++i)
    {
        out << v._data[i];
        if (i != D - 1) out << ", "; // Add comma between elements, but not after the last one
    }
    out << ']';
    return out;
  }

  template<std::size_t D>
  instance<D> operator*(float scalar, const instance<D>& vector)
  {
    return vector * scalar;
  }

  template<std::size_t D>
  instance<D> operator+(const instance<D>& lhs, const instance<D>& rhs)
  {
    instance<D> result;
    for(std::size_t i = 0; i < D; ++i)
      result[i] = lhs[i] + rhs[i];
    
    return result;
  }

  template<std::size_t D>
  bool operator==(const instance<D>& lhs, const instance<D>& rhs)
  {
    if(&lhs == &rhs)
      return true;
    if(lhs.type != rhs.type)
      return false;
    
    for(std::size_t i = 0; i < D; ++i)
      if(lhs[i] != rhs[i])
        return false;

    return true;
  }
  // ---

  template<std::size_t D>
  float dot_product(const instance<D>& v1, const instance<D>& v2)
  {
    assert(v1._data.max_size() == v2._data.max_size());
    float product = 0.f;
    for(std::size_t i = 0; i < D; ++i)
      product += v1[i] * v2[i];
    
    return product;
  }

  // 3.5 linear weighted combination
  template<std::size_t D>
  instance<D> linear_weighted_combination(std::vector<float> weights, std::vector<instance<D>> vectors)
  {
    assert(weights.size() == vectors.size());
    instance<D> result;
    for(std::size_t i = 0; i < weights.size(); ++i)
    {
      instance<D> tmp = vectors[i];
      float weight = weights[i];
      tmp *= weight;
      result += tmp;
    }
    return result;
  }

  template<std::size_t D>
  void print(instance<D>& v)
  {
    std::cout << D << "-dimensional " << v.vector_type_str() << " vector" << std::endl;
    std::cout << '[';
    for (std::size_t i = 0; i < D; ++i)
    {
        std::cout << v._data[i];
        if (i != D - 1) std::cout << ", "; // Add comma between elements, but not after the last one
    }
    std::cout << ']';
  }

  // 3.6 The outer product
  template<std::size_t D1, std::size_t D2>
  matrix::instance<D1,D2> outer_product(const instance<D1>& v1, const instance<D2>& v2)
  {
    matrix::instance<D1,D2> result;
    for(std::size_t row = 0; row < D1; ++row)
      for(std::size_t col = 0; col < D2; ++col)
        result[row][col] = v1[row] * v2[col];
    
    return result;
  }

  // 3.7 element-wise (Hadamard) product vector
  template<std::size_t D>
  instance<D> hadamard_product(const instance<D>& v1, const instance<D>& v2)
  {
    instance<D> result;
    for(std::size_t i = 0; i < D; ++i)
      result[i] = v1[i] * v2[i];
    
    return result;
  }

  // 3.8 cross product
  template<std::size_t D>
  instance<D> cross_product(const instance<D>& v1, const instance<D>& v2)
  {
    assert(D == 3); // the cross product is defined only for two 3-element vectors
    instance<D> result;
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];

    return result;
  }

  // 3.9 unit vector
  template<std::size_t D>
  instance<D> unit_vector(const instance<D>& v1)
  {
    return (1 / v1.magnitude()) * v1;
  }
} // namespace vector

#endif // __LINEAR_ALGEBRA_H__