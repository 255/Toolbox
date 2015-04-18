#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>

/** A vector of 2 dimensions */
template<typename Scalar = double>
class Vector2 {
public:
  Scalar x;
  Scalar y;

  /** Initialize vector to set value */
  constexpr Vector2();

  /** Initialize vector to set value */
  constexpr Vector2(Scalar xValue, Scalar yValue);

  /** Initialize vector to set value */
  constexpr Vector2(Scalar same_x_y);

  /** Create a vector of specified length in the same direction as the provided vector */
  Vector2(Scalar length, const Vector2<Scalar>& direction);

  /** Assign the values of one vector to this vector */
  Vector2<Scalar>& operator=(const Vector2<Scalar>& other);

  /** Subtract a vector from this vector */
  Vector2<Scalar>& operator-=(const Vector2<Scalar>& other);

  /** Add a vector from this vector */
  Vector2<Scalar>& operator+=(const Vector2<Scalar>& other);

  /** Multiply this vector by a scalar */
  Vector2<Scalar>& operator*=(Scalar n);

  /** Divide this vector by a scalar */
  Vector2<Scalar>& operator/=(Scalar n);

  /** Compare this vector to another vector */
  bool operator==(const Vector2<Scalar>& other) const;

  /** Compare this vector to another vector */
  bool operator!=(const Vector2<Scalar>& other) const;

  /** Add two vectors */
  Vector2<Scalar> operator+(const Vector2<Scalar>& other) const;

  /** Subtract two vectors */
  Vector2<Scalar> operator-(const Vector2<Scalar>& other) const;

  /** Negate a vector */
  Vector2<Scalar> operator-() const;

  /** Multiply a vector by a scalar */
  Vector2<Scalar> operator*(Scalar n) const;

  /** Divide a vector by a scalar */
  Vector2<Scalar> operator/(Scalar n) const;

  /** Compare two vectors */
  bool operator<(const Vector2<Scalar>& rhs) const;

  /** Find the length of this vector */
  template<typename T = Scalar>
  T length() const;

  /** Find the distance between two vectors */
  template<typename T = Scalar>
  static T distance(const Vector2<Scalar>& vec1, const Vector2<Scalar>& vec2);

  /** Find the dot product of two vectors */
  Scalar dotProduct(const Vector2<Scalar>& other) const;

  /** Find the dot product of two vectors */
  static Scalar dotProduct(const Vector2<Scalar>& left, const Vector2<Scalar>& right);

  /** The magnitude of the vector squared, or the vector's dot product with itself */
  Scalar squared() const;

  /** Return a normalize version of this vector */
  Vector2<Scalar> normalized() const;

  // TODO: make sure this works...
  /** The angle with respect to the x-axis */
  Scalar angle() const;

  /** Clear the vector (set it to 0) */
  void clear();

  /** Add to the the length but not the angle of the vector */
  void addLength(Scalar delta);

  /** Scale the length but not the angle of the vector */
  void scaleLength(Scalar f);

  /** Set the length but not the angle of the vector */
  void setLength(Scalar newLength);

  /** Determine whether this vector's values within the specified range
   *  Checks if each coordinate is in the range [min, max).
   */
  bool inRange(Scalar x_min, Scalar x_max, Scalar y_min, Scalar y_max) const;

  /** Distance from point to line defined by two points */
  static Scalar distancePointToLine(const Vector2<Scalar>& point, const Vector2<Scalar>& linePt1, const Vector2<Scalar>& linePt2);

  /**
   * Convert to any type that takes two paramters (x, y) in its constructor.
   */
  template<typename U>
  U to() const;
  
  /** Represent as a string */
  std::string toString(char open_paren = '[', char close_paren = ']') const;
};

/** Multiply a vector by a scalar */
template<typename Scalar, typename T>
Vector2<Scalar> operator*(T n, const Vector2<Scalar>& vec);

/** Output the contents of a vector */
template<typename Scalar>
std::ostream& operator<<(std::ostream& os, const Vector2<Scalar>& vec);

// define the functions
#include "Vector2.inl"

#endif //VECTOR2_H
