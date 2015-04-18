#ifndef CYCLETIMER_H
#define CYCLETIMER_H

#include <cstdint>

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define X86_64
#endif

/**
 * Record elapsed times with cycle precision.
 * Uses the time stamp counter (TSC) register in an x86-64 CPU.
 * This register increments once per clock cycle at a fixed clock rate on recent processors.
 * Older processors allowed the TSC's clock to vary with the rest of the processor.
 *
 * Most of the assembly was lovingly borrowed from Intel's How to Benchmark Code Execution Times white paper.
 *
 * @note This only works with (fairly recent) x86-64 CPUs!  The X86_64 macro will not be defined for other
 * architectures, and this code will not function. I should add a fall-back high_resolution_clock-based timer or
 * something.
 *
 * @param CLOCK_RATE the clock rate for converting to seconds in time() function calls
 * @note A Timer typedef is provided with a default clock rate. If the clock rate is not known or not needed,
 *       this typedef can be used to avoid needing to provide a template argument.
 */
template<unsigned long CLOCK_RATE>
class CycleTimer {
private:
  uint32_t start_low_, start_high_;
  uint32_t stop_low_, stop_high_;

  static inline uint64_t combineUint32(uint32_t low, uint32_t high) {
    return (static_cast<uint64_t>(high) << 32) | low;
  }

public:
  /** The time unit to use for calls that return the calculated time */
  enum Unit : unsigned {
    s  = 1u,
    ms = 1000u,
    us = 1000000u,
    ns = 1000000000u,
  };

  constexpr CycleTimer() : start_low_(0), start_high_(0), stop_low_(0), stop_high_(0) {}

  /** Get the value of the clock rate (template paramater, for the time being) */
  constexpr inline unsigned long clockRate() const { return CLOCK_RATE; }

  /**
   * Return how many clock cycles have passed since the CPU started up.
   * Reads the TSC.
   * @return the value of the time stamp counter register
   */
  static inline uint64_t cycleCount() {
    uint32_t low, high;
#ifdef X86_64
    asm volatile("rdtsc" : "=a" (low), "=d" (high));
#endif
    return static_cast<uint64_t>(high) << 32 | low;
  }

  /**
   * Return how many time has passed since the CPU started up.
   * Reads the TSC, gets the time based on the specified clock rate.
   * @return the value of the time stamp counter register
   */
  template<Unit U = s>
  static inline double runningTime() {
    return static_cast<double>(cycleCount()) / (CLOCK_RATE / static_cast<double>(U));
  }

  /**
   * Start the timer.
   */
  inline void start() {
    // per Intel, CPUID blocks out-of-order execution -- call before and after
#ifdef X86_64
    asm volatile(
      "cpuid\n\t"
      "rdtsc\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1"
      : "=r" (start_high_), "=r" (start_low_)
      :: "%rax", "%rbx", "%rcx", "%rdx"
    );
#endif
  }

  /**
   * Stop the timer.
   */
  inline void stop() {
    // rdtscp serializes execution
#ifdef X86_64
    asm volatile(
      "rdtscp\n\t" // rdtscp (serializing rdtsc) only works on newer CPUs -- lfence will work for older cpus: "lfence;rdtsc\n\t" 
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      "cpuid"
      : "=r" (stop_high_), "=r" (stop_low_)
      :: "%rax", "%rbx", "%rcx", "%rdx"
    );
#endif
  }

  /**
   * Return the current time without stopping the clock.
   * This will affect the accuracy of the timer, but only a little tiny bit.
   */
  inline uint64_t checkCycles() const {
    uint32_t high;
    uint32_t low;

    // rdtscp serializes execution
#ifdef X86_64
    asm volatile(
      "rdtscp\n\t" // rdtscp (serializing rdtsc) only works on newer CPUs -- lfence will work for older cpus: "lfence;rdtsc\n\t" 
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      "cpuid"
      : "=r" (high), "=r" (low)
      :: "%rax", "%rbx", "%rcx", "%rdx"
    );
#endif

    return combineUint32(low, high) - combineUint32(start_low_, start_high_);
  }

  /**
   * Get the number of cycles recorded on this timer.
   * @return the number of cycles recorded between the previous start and stop calls.
   */
  inline uint64_t cycles() const {
    return combineUint32(stop_low_, stop_high_) - combineUint32(start_low_, start_high_);
  }

  /**
   * Get the elapsed time recorded on this timer.
   * @param U the unit to use for time: s (default), ms, us, or ns.
   * @return the elapsed time between the previous start and stop calls.
   */
  template<Unit U = s>
  inline double time() const {
    return static_cast<double>(cycles()) / (clockRate() / static_cast<double>(U));
  }

  /**
   * Return the current time in the specified units without stopping the clock.
   * @param U the unit to use for time: s (default), ms, us, or ns.
   * @return the elapsed time since the previous start call.
   */
  template<Unit U = s>
  inline double checkTime() const {
    return static_cast<double>(checkCycles()) / (clockRate() / static_cast<double>(U));
  }
};

typedef CycleTimer<4000000000ul> Timer; // this should be set to your CPU's clock rate

#endif
