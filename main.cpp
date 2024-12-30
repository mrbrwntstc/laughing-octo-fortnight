#include "linear_algebra.h"

#include <iostream>

int main()
{
  {
    vector::instance<1> v1;
    vector::instance<3> v2 = {1,2,3};

    std::cout << '[' << v1._data[0] << ']' << std::endl;
    std::cout << '[' << v2._data[0] << ',' << v2._data[1] << ',' << v2._data[2] << ']' << std::endl;

    v1[0] = 5;
    std::cout << '[' << v1._data[0] << ']' << std::endl;

    v2 = -v2;
    std::cout << '[' << v2._data[0] << ',' << v2._data[1] << ',' << v2._data[2] << ']' << std::endl;

    vector::instance<3> v3 = {4,5,6};
    v2 += v3;
    std::cout << '[' << v2._data[0] << ',' << v2._data[1] << ',' << v2._data[2] << ']' << std::endl;

    v2 -= v3;
    std::cout << '[' << v2._data[0] << ',' << v2._data[1] << ',' << v2._data[2] << ']' << std::endl;

    v3 *= 5;
    std::cout << v3 << std::endl;
  }

  { // 3.1 vectors: algebra
    {
      vector::instance<4> v1 = {1,2,3,4};
      vector::instance<4> v2 = {5,6,7,8};
      float dot_product = vector::dot_product(v1, v2); // 70
      std::cout << "Dot product: " << dot_product << std::endl;

      // the dot product between a vector and itself is called the magnitude-squared of the vector
      vector::instance<3> v3 = {2,2,2};
      std::cout << "Dot product between v3 and itself: " << v3.magnitude_squared() << std::endl; // 12
    }  

    // the dot product is not associative

    {
      vector::instance<4> v1 = {1,2,3,4};
      vector::instance<4> v2 = {5,6,7,8};
      bool is_commutative = vector::dot_product(v1, v2) == vector::dot_product(v2, v1); // true
      std::cout << "the dot product is commutative: " << (is_commutative ? "true" : "false") << std::endl; // true
    }

    {
      vector::instance<2> u = {1,2};
      vector::instance<2> v = {1,3};
      vector::instance<2> w = {2,3};

      // [2, 3]([1, 2]+[1, 3])
      std::cout << w << '(' << u << '+' << v << ')' << std::endl;
      float dot_1 = vector::dot_product(w, u+v);
      // ([2, 3][1, 2]) + ([2, 3][1, 3])
      std::cout << '(' << w << u << ") + " << '(' << w << v << ')' << std::endl;
      float dot_2 = vector::dot_product(w,u) + vector::dot_product(w, v);
      std::cout << dot_1 << " = " << dot_2 << std::endl;
      std::cout << "Is dot product distributive: " << ((dot_1 == dot_2) ? "true" : "false") << std::endl;
    }
    
  }

  { // 3.5 linear distribution
    vector::instance<3> v1 = {4,5,1};
    vector::instance<3> v2 = {-4,0,-4};
    vector::instance<3> v3 = {1,3,2};

    std::vector<float> weights = {1, 2, -3};
    std::vector<vector::instance<3>> vectors = {v1, v2, v3};

    vector::instance<3> v4 = vector::linear_weighted_combination(weights, vectors);
    std::cout << v4 << std::endl;
  }

  { // 3.6 outer product
    vector::instance<4> v1 = {1,2,3,4};
    vector::instance<3> v2 = {4,1,-1};
    matrix::instance<4,3> mat1 =  vector::outer_product(v1, v2);
    matrix::instance<3,4> mat2 = vector::outer_product(v2,v1);

    std::cout << mat1 << std::endl;
    std::cout << mat2 << std::endl;
  }

  { // 3.7 hadamard product vector
    vector::instance<4> v1 = {3,6,9,0};
    vector::instance<4> v2 = {2,-2,4,-4};
    vector::instance<4> hadamard_product = vector::hadamard_product(v1, v2);

    std::cout << hadamard_product << std::endl;
  }

  { // 3.8 cross product
    vector::instance<3> v1 = {5,3,4};
    vector::instance<3> v2 = {-2,1,-1};
    vector::instance<3> cross = vector::cross_product(v1,v2);

    std::cout << cross << std::endl;
    std::cout << std::endl;
  }

  { // 3.9 unit vector
    vector::instance<2> v1 = {0,2};
    vector::instance<2> v1_unit = vector::unit_vector(v1);

    std::cout << v1_unit << std::endl;
    std::cout << std::endl;
  }

  { // 5.4 matrices - the transpose operation
    matrix::instance<2,3> matrix = {{1,2,3},{4,5,6}};
    matrix::instance<3,2> transpose = matrix::transpose(matrix);

    std::cout << matrix << std::endl;
    std::cout << transpose << std::endl;
  }

  return 0;
}
