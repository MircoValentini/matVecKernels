/*
 * @file timing.H
 *
 * @brief Definition of the routines and data to be used to measure and handle
 * the time measurements in the code
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */

#ifndef __TIMING_H__
#define __TIMING_H__

/**
 * System include
 */
#include <stdio.h>
#include <time.h>


/**
 * @class datatype used to store/accumulate and print the timing informations
 */
class timeProfile
{
  private:

    /**
     * Variables used to store the clock time 
     * (measured with the clock routines)
     */
    double m_dclock;

    /**
     * Variables used to store the elapsed time 
     * (measured with the time routines)
     */
    double m_dtime;

#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
    /**
     * Variables used to store the elapsed time 
     * (measured with the cuda events)
     */
    float m_dcudaTime;
#endif

    /**
     * Definition of the clock class that can access private members to set
     * the values of time.
     * In this way members of htis class can be modified only by the 
     * simpleClock class.
     */
    friend class simpleClock_t;

  public:

    /**
     * @brief empty constructor of the class
     */
    timeProfile():
      m_dclock(0.0)
     ,m_dtime(0.0)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
     ,m_dcudaTime(0.0)
#endif
    {};

    /**
     * @brief copy constructor of the class
     */
    timeProfile
    (
      const timeProfile& src
    ):
      m_dclock(src.m_dclock)
     ,m_dtime(src.m_dtime)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
     ,m_dcudaTime(src.m_dcudaTime)
#endif
    {};

    /**
     * @brief Destructor of the class
     */
    ~timeProfile()
    {
      m_dclock = 0.0;
      m_dtime = 0.0;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
     m_dcudaTime = 0.0;
#endif
      /* Exit point */
      return;
    };


    /**
     * @brief Get the execution time
     */
    double executionTime() const { return(m_dtime); };

    /**
     * @brief GEt the clock time
     */
    double     clockTime() const { return(m_dclock); };

    /**
     * @brief Get the cuda time
     */
    double      cudaTime() const
    {
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) ||  defined(__TEST_OPEN_MP__)
      return(m_dtime);
#else
      return(0.0);
#endif
    };

    /**
     * @brief Destructor of the class
     */
    void
    print()
    {
      /* Print timing data */
#if defined(__TEST_CPU__)
      printf( " Execution time (CPU): %10.6lf [s], Clock time: %10.6lf [s]\n", 
              m_dtime, m_dclock );
#endif
#if defined(__TEST_NATIVE_CUDA__)
      printf( " Execution time (CUDA): %10.6lf [s], Clock time: %10.6lf [s], Cuda time: %10.6f [s]\n", 
              m_dtime, m_dclock, m_dcudaTime/1000 );
#endif
#if defined(__TEST_OPEN_ACC__)
      printf( " Execution time (OPEN_ACC): %10.6lf [s], Clock time: %10.6lf [s], Cuda time: %10.6f [s]\n", 
              m_dtime, m_dclock, m_dcudaTime/1000 );
#endif
#if defined(__TEST_OPEN_MP__)
      printf( " Execution time (OPEN_MP): %10.6lf [s], Clock time: %10.6lf [s]\n", 
              m_dtime, m_dclock );
#endif
      /* Exit point */
      return;
    };



    /**
     * @brief Function used to print the timing measures with the name 
     */
    void
    print
    (
      const char* prefix,
      const char* name,
            idx_k index
    )
    {
      /* Print timing data */
#if defined(__TEST_CPU__)
      printf( " Prefix: %20s, Name: %60s, Index: %6.6d, Execution time (CPU): %10.6lf [s], Clock time: %10.6lf [s]\n", 
              prefix, name, index, m_dtime, m_dclock );
#endif
#if defined(__TEST_NATIVE_CUDA__)
      printf( " Prefix: %20s, Name: %60s, Index: %6.6d, Execution time (CUDA): %10.6lf [s], Clock time: %10.6lf [s], Cuda time: %10.6f [s]\n", 
              prefix, name, index, m_dtime, m_dclock, m_dcudaTime/1000 );
#endif
#if defined(__TEST_OPEN_ACC__)
      printf( " Prefix: %20s, Name: %60s, Index: %6.6d, Execution time (OPEN_ACC): %10.6lf [s], Clock time: %10.6lf [s], Cuda time: %10.6f [s]\n", 
              prefix, name, index, m_dtime, m_dclock, m_dcudaTime/1000 );
#endif
#if defined(__TEST_OPEN_MP__)
      printf( " Prefix: %20s, Name: %60s, Index: %6.6d, Execution time (OPEN_MP): %10.6lf [s], Clock time: %10.6lf [s]\n", 
              prefix, name, index, m_dtime, m_dclock );
#endif
      /* Exit point */
      return;
    };

    /**
     * @brief Accumuate time measures
     */
    timeProfile& 
    operator+=( const timeProfile& rhs )
    {
      /* Sum timing measures */
      this->m_dclock += rhs.m_dclock;
      this->m_dtime  += rhs.m_dtime;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
      this->m_dcudaTime  += rhs.m_dcudaTime;
#endif
      /* Exit point */
      return( *this );
    };
};



/**
 * @class datatype used to manage time measures
 */
class simpleClock_t
{
  private:

    /**
     * Variable used to store the clock reference
     */
    clock_t m_clock;

    /**
     * Variable used to store the time reference
     */
    struct timespec  m_epoch;

#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
    /**
     * Data used to record the time using cuda routines
     */
    cudaEvent_t m_start;
    cudaEvent_t m_stop;
#endif

  public:

    /**
     * @breif empty constructor
     */
    simpleClock_t():
      m_clock(0)
     ,m_epoch({0,0})
    {
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
      cudaEventCreate( &m_start );
      cudaEventCreate( &m_stop );
#endif
      /* Exit point */
      return;
    };


    /**
     * @breif copy constructor
     */
    simpleClock_t
    (
      const simpleClock_t& src
    ):
      m_clock(src.m_clock),
      m_epoch(src.m_epoch)
    {
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
      //// TODO: Verify if a cudaEvent_t can be copied smmothly
      m_start = src.m_start;
      m_stop  = src.m_stop;
#endif
      /* Exit point */
      return;
    };


    /**
     * @breif copy constructor
     */
    ~simpleClock_t(  )
    {
      m_clock = 0;
      m_epoch = {0,0};
      /* Exit point */
      return;
    };


    /**
     * @breif Record the clock reference
     */
    void tic()
    {
      m_clock=clock();
      clock_gettime(CLOCK_MONOTONIC, &m_epoch);
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
      /*
       * Record reference time for cuda
       */
      cudaEventRecord( m_start );
#endif
      /* Exit point */
      return;
    };


    /**
     * @breif Compute elapsed time with respecto tot the reference
     */
    void
    toc
    (
      timeProfile& profileTime
    )
    {
      /*
       * Create a new timeProfile object to store the measurees
       */
      struct timespec  curr_time;
      clock_t now = clock();
      clock_gettime(CLOCK_MONOTONIC, &curr_time );
      /*
       * Compute the elapsed time
       */
      unsigned long long diff = (now-m_clock)*1000;
      double dclock = ((double)diff)/CLOCKS_PER_SEC;
      double dtime =
        ((double)curr_time.tv_sec + 1.0e-9*curr_time.tv_nsec) -
        ((double)m_epoch.tv_sec   + 1.0e-9*m_epoch.tv_nsec);
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
      /*
       * Record end time for cuda
       */
      cudaEventRecord( m_stop );
      cudaEventSynchronize( m_stop );
      float milliseconds = 0;
      cudaEventElapsedTime( &milliseconds, m_start, m_stop );
#endif
      /*
       * Store elapsed time in the output structure
       */
      profileTime.m_dclock = dclock/1000;
      profileTime.m_dtime  = dtime;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__)
      /*
       * Store cuda time
       */
      profileTime.m_dcudaTime = milliseconds;
#endif
      // Exit point
      return;
    };

};

#endif
