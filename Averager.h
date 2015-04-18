#ifndef AVERAGER_H
#define AVERAGER_H

/**
 * Maintain an average (mean) of a bunch of values.
 */ 
template<typename T = double>
class Averager {
  unsigned long count_;
  T total_;

public:
  Averager() : count_(0), total_() {}

  /** Add a value to the averager. */
  void add(T value) {
    total_ += value;
    count_ += 1;
  }

  /** Get the current average (mean) of all of the added values. */
  T average() const { return total_ / static_cast<T>(count_); }

  /** Get the total sum of all of the added values. */
  T total() const { return total_; }

  /** Get how many values have been added. */
  unsigned long count() const { return count_; }
};

/**
 * Maintain a running average of the last VALUES values.
 */
template<unsigned VALUES, typename T = double>
class RunningAverager {
  T values_[VALUES]; // this could be moved to the heap to allow for very large numbers of values
  T running_total_;
  unsigned next_;

  inline void increment(unsigned* variable) {
    *variable = (*variable == VALUES-1) ? 0 : *variable + 1;
  }

public:
  RunningAverager() : values_{}, running_total_(), next_(0) {}

  /** Add a value to the averager. */
  void add(const T& value) {
    running_total_ -= values_[next_];
    running_total_ += value;
    values_[next_] = value;

    increment(&next_);
  }

  /** Get the current average (mean) of all of the added values. */
  T average() const { return running_total_ / static_cast<T>(VALUES); }

  /** Get the total sum of all of the added values. */
  T total() const { return running_total_; }

  /** Get how many values are being looked at in the running average. */
  unsigned count() const { return VALUES; }

  // iterator: examine the values
  typedef const T* const_iterator;
  const_iterator begin() const { return values_; }
  const_iterator cbegin() const { return values_; }
  const_iterator end() const { return &values_[VALUES]; }
  const_iterator cend() const { return &values_[VALUES]; }
};

#endif
