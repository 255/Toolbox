// Definition of the Vector2 class
template<typename Scalar>
constexpr inline Vector2<Scalar>::Vector2() : x(), y() {}

template<typename Scalar>
constexpr inline Vector2<Scalar>::Vector2(Scalar x_value, Scalar y_value) : x(x_value), y(y_value) {}

template<typename Scalar>
constexpr inline Vector2<Scalar>::Vector2(Scalar same_x_y) : x(same_x_y), y(same_x_y) {}

template<typename Scalar>
inline Vector2<Scalar>::Vector2(Scalar length, const Vector2<Scalar>& direction) {
  Scalar originalLength = direction.length();

  x = direction.x/originalLength * length;
  y = direction.y/originalLength * length;
}

template<typename Scalar>
inline Vector2<Scalar>& Vector2<Scalar>::operator=(const Vector2<Scalar>& other) {
  x = other.x;
  y = other.y;
  return *this;
}

template<typename Scalar>
inline Vector2<Scalar>& Vector2<Scalar>::operator-=(const Vector2<Scalar>& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

template<typename Scalar>
inline Vector2<Scalar>& Vector2<Scalar>::operator+=(const Vector2<Scalar>& other) {
  x += other.x;
  y += other.y;
  return *this;
}

template<typename Scalar>
inline Vector2<Scalar>& Vector2<Scalar>::operator*=(Scalar n) {
  x *= n;
  y *= n;
  return *this;
}

template<typename Scalar>
inline Vector2<Scalar>& Vector2<Scalar>::operator/=(Scalar n) {
  x /= n;
  y /= n;
  return *this;
}

template<typename Scalar>
inline bool Vector2<Scalar>::operator==(const Vector2<Scalar>& other) const {
  return (x == other.x) && (y == other.y);
}

template<typename Scalar>
inline bool Vector2<Scalar>::operator!=(const Vector2<Scalar>& other) const {
  return (x != other.x) || (y != other.y);
}

template<typename Scalar>
inline Vector2<Scalar> Vector2<Scalar>::operator+(const Vector2<Scalar>& other) const {
  return Vector2(x + other.x, y + other.y);
}

template<typename Scalar>
inline Vector2<Scalar> Vector2<Scalar>::operator-(const Vector2<Scalar>& other) const {
  return Vector2(x - other.x, y - other.y);
}

template<typename Scalar>
inline Vector2<Scalar> Vector2<Scalar>::operator-() const {
  return Vector2(-x, -y);
}

template<typename Scalar>
inline Vector2<Scalar> Vector2<Scalar>::operator*(Scalar n) const {
  return Vector2(x * n, y * n);
}

template<typename Scalar>
inline Vector2<Scalar> Vector2<Scalar>::operator/(Scalar n) const {
  return Vector2(x / n, y / n);
}

template<typename Scalar>
inline bool Vector2<Scalar>::operator<(const Vector2<Scalar>& rhs) const {
  if (x < rhs.x) return true;
  if (x > rhs.x) return false;
  return (y < rhs.y);
}

template<typename Scalar>
inline Scalar Vector2<Scalar>::dotProduct(const Vector2<Scalar>& other) const {
  return x * other.x + y  * other.y;
}

template<typename Scalar>
inline Scalar Vector2<Scalar>::dotProduct(const Vector2<Scalar>& left, const Vector2<Scalar>& right) {
  return left.x * right.x + left.y * right.y;
}

template<typename Scalar>
template<typename T>
inline T Vector2<Scalar>::length() const {
  return sqrt(x*x + y*y);
}

template<typename Scalar>
template<typename T>
inline T Vector2<Scalar>::distance(const Vector2<Scalar>& vec1, const Vector2<Scalar>& vec2) {
  Scalar dx = vec1.x - vec2.x;
  Scalar dy = vec1.y - vec2.y;

  return sqrt(dx*dx + dy*dy);
}

template<typename Scalar>
inline Scalar Vector2<Scalar>::squared() const {
  return x*x + y*y;
}

template<typename Scalar>
inline Vector2<Scalar> Vector2<Scalar>::normalized() const {
  return Vector2(x/length(), y/length());
}

template<typename Scalar>
inline Scalar Vector2<Scalar>::angle() const {
  return atan2(y, x);
}

template<typename Scalar>
inline void Vector2<Scalar>::clear() {
  x = 0;
  y = 0;
}

// TODO: TEST THESE FUNCTIONS
template<typename Scalar>
inline void Vector2<Scalar>::addLength(Scalar delta) {
  // cos(arctan(y/x)) == x/hypotenuse
  if (x != 0 || y != 0) {
    Scalar hypotenuse = length();
    x += x/hypotenuse * delta;
    y += y/hypotenuse * delta;
  } else {
    x = delta;
  }
}

template<typename Scalar>
inline void Vector2<Scalar>::setLength(Scalar newLength) {
  Scalar len = length();
  // cos(arctan(y/x)) == x/length
  x = x/len * newLength;
  y = y/len * newLength;
}

template<typename Scalar>
inline bool Vector2<Scalar>::inRange(Scalar x_min, Scalar x_max, Scalar y_min, Scalar y_max) const {
  return x >= x_min && x < x_max && y >= y_min && y < y_max;
}

template<typename Scalar>
inline Scalar Vector2<Scalar>::distancePointToLine(const Vector2<Scalar>& point, const Vector2<Scalar>& linePt1, const Vector2<Scalar>& linePt2) {
  // handle vertical lines
  if (linePt1.x == linePt2.x) {
    return abs(point.x - linePt1.x);
  }

  Scalar m = (linePt1.y - linePt2.y) / (linePt1.x - linePt2.x);
  Scalar b = linePt1.y - m*linePt1.x;

  // Thanks http://math.ucsd.edu/~wgarner/math4c/derivations/distance/distptline.htm
  return abs(point.y - m*point.x - b / sqrt(m*m + 1));
}

template<typename Scalar>
template<typename U>
inline U Vector2<Scalar>::to() const {
  return U(x, y);
}

template<typename Scalar>
inline std::string Vector2<Scalar>::toString(char open_paren, char close_paren) const {
  return open_paren + std::to_string(x) + ", " + std::to_string(y) + close_paren;
}

template<typename Scalar, typename T>
inline Vector2<Scalar> operator*(T n, const Vector2<Scalar>& vec) {
  return Vector2<Scalar>(n * vec.x, n * vec.y);
}

template<typename Scalar>
inline std::ostream& operator<<(std::ostream& os, const Vector2<Scalar>& vec) {
  return os << '[' << vec.x << ' ' << vec.y << ']';
}
