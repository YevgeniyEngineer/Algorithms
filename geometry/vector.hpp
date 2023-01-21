#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <ostream>
#include <type_traits>

#include <vector>

// Class that only uses floats / doubles
namespace geom
{
template <typename coordinate_type, size_t dimension,
          typename std::enable_if<std::is_floating_point<coordinate_type>::value>::type * = nullptr>
class Vector
{
    static_assert(dimension > 1, "Vector dimension must be at least 2D");

  private:
    std::array<coordinate_type, dimension> coords_;

  public:
    Vector() = default;
    Vector(const std::array<coordinate_type, dimension> _coords_) : coords_(_coords_){};
    Vector(coordinate_type _x, coordinate_type _y, coordinate_type _z) : coords_({_x, _y, _z}){};
    Vector(coordinate_type _x, coordinate_type _y) : coords_({_x, _y}){};
    virtual ~Vector() = default;

    const std::array<coordinate_type, dimension> &coords() const
    {
        return coords_;
    }

    std::array<coordinate_type, dimension> &coords()
    {
        return coords_;
    }

    const coordinate_type &operator[](std::size_t i) const
    {
        return coords_[i];
    }

    // Equal
    bool operator==(const Vector<coordinate_type, dimension> &rhs)
    {
        for (std::size_t i = 0; i < dimension; ++i)
        {
            if (isEqual(coords_[i], rhs.coords()[i]))
            {
                return false;
            }
        }
        return true;
    }

    // Not equal
    bool operator!=(const Vector<coordinate_type, dimension> &rhs)
    {
        return !(*this == rhs);
    }

    // Addition
    Vector operator+(const Vector<coordinate_type, dimension> &rhs) const
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp_array[i] = coords_[i] + rhs.coords()[i];
        }
        return Vector(temp_array);
    }

    // Subtraction
    Vector operator-(const Vector<coordinate_type, dimension> &rhs) const
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp_array[i] = coords_[i] - rhs.coords()[i];
            return Vector(temp_array);
        }
    }

    // Inplace addition
    Vector &operator+=(const Vector<coordinate_type, dimension> &rhs)
    {
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] += rhs.coords()[i];
        }
        return *this;
    }

    // Inplace subtraction
    Vector &operator-=(const Vector<coordinate_type, dimension> &rhs)
    {
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] -= rhs.coords()[i];
        }
        return *this;
    }

    // Less than operator
    bool operator<(const Vector<coordinate_type, dimension> &rhs)
    {
        for (std::size_t i = 0; i < dimension; ++i)
        {
            if (coords_[i] < rhs.coords()[i])
            {
                return true;
            }
            else if (coords_[i] > rhs.coords()[i])
            {
                return false;
            }
        }
        return false;
    }

    // Greater operator
    bool operator>(const Vector<coordinate_type, dimension> &rhs)
    {
        for (std::size_t i = 0; i < dimension; ++i)
        {
            if (coords_[i] > rhs.coords()[i])
            {
                return true;
            }
            else if (coords_[i] < rhs.coords()[i])
            {
                return false;
            }
        }
        return false;
    }

    // Add scalar
    Vector operator+(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp_array[i] = coords_[i] + scalar;
        }
        return Vector(temp_array);
    }

    // Subtract scalar
    Vector operator-(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp_array[i] = coords_[i] - scalar;
        }
        return Vector(temp_array);
    }

    // Multiply by scalar
    Vector operator*(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp_array[i] = coords_[i] * scalar;
        }
        return Vector(temp_array);
    }

    // Divide by scalar
    Vector operator/(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp_array[i] = coords_[i] / scalar;
        }
        return Vector(temp_array);
    }

    // Inplace add scalar
    Vector &operator+=(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] += scalar;
        }
        return *this;
    }

    // Inplace subtract scalar
    Vector &operator-=(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] -= scalar;
        }
        return *this;
    }

    // Inplace multiply by scalar
    Vector &operator*=(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] *= scalar;
        }
        return *this;
    }

    // Inplace divide by scalar
    Vector &operator/=(coordinate_type scalar)
    {
        std::array<coordinate_type, dimension> temp_array;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] /= scalar;
        }
        return *this;
    }

    // Magnitude squarred
    const coordinate_type magnitudeSqr() const
    {
        coordinate_type magnitude_squarred = 0;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            magnitude_squarred += (coords_[i] * coords_[i]);
        }
        return magnitude_squarred;
    }

    // Magnitude
    const coordinate_type magnitude() const
    {
        return std::sqrt(this->magnitudeSqr());
    }

    // Normalize vector
    void normalize()
    {
        auto mag_ = magnitude();
        for (std::size_t i = 0; i < dimension; ++i)
        {
            coords_[i] /= mag_;
        }
    }

    // Get normalized (copy)
    Vector getNormalized()
    {
        auto mag_ = magnitude();
        std::array<coordinate_type, dimension> temp;
        for (std::size_t i = 0; i < dimension; ++i)
        {
            temp[i] = coords_[i] /= mag_;
        }
        return Vector(temp);
    }

    // Size of the Vector
    const std::size_t size() const
    {
        return coords_.size();
    }

    // ostream without newline character
    friend std::ostream &operator<<(std::ostream &ostream, const Vector<coordinate_type, dimension> &rhs)
    {
        for (std::size_t i = 0; i < rhs.size(); ++i)
        {
            ostream << rhs[i] << " ";
        }
        return ostream;
    }
};

template <typename coordinate_type, std::size_t dimension>
coordinate_type dotProduct(const Vector<coordinate_type, dimension> &v1, const Vector<coordinate_type, dimension> &v2)
{
    coordinate_type dot_product = 0;
    for (std::size_t i = 0; i < dimension; ++i)
    {
        dot_product += (v1[i] * v2[i]);
    }
    return dot_product;
}

template <typename coordinate_type, std::size_t dimension = 2>
coordinate_type crossProduct(const Vector<coordinate_type, dimension> &v1, const Vector<coordinate_type, dimension> &v2)
{
    return (v1[0] * v2[1] - v1[1] * v2[0]);
}

template <typename coordinate_type>
Vector<coordinate_type, 3> crossProduct(const Vector<coordinate_type, 3> &v1, const Vector<coordinate_type, 3> &v2)
{
    coordinate_type x = (v1[1] * v2[2] - v1[2] * v2[1]);
    coordinate_type y = (v1[2] * v2[0] - v1[0] * v2[2]);
    coordinate_type z = (v1[0] * v2[1] - v1[1] * v2[0]);
    return Vector(x, y, z);
}

} // namespace geom

#endif // VECTOR_HPP_