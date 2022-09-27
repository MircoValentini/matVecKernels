/*
 * timing.H
 *
 *  Created on: Apr 27, 2022
 *      Author: valentini
 */

#ifndef __TIMING_H__
#define __TIMING_H__

#include <time.h>
#include <stdio.h>

class timeProfile
{
  public:
    double m_dclock;
    double m_dtime;
    timeProfile():m_dclock(0.0),m_dtime(0.0){};
    void print()
    {
      printf( " Execution time: %lf [s], Clock time: %lf [s]\n", m_dtime, m_dclock );
      return;
    };

    timeProfile& operator+=( const timeProfile& rhs )
    {
      this->m_dclock += rhs.m_dclock;
      this->m_dtime  += rhs.m_dtime;
      return( *this );
    };
};


class simpleClock_t
{
  private:
    clock_t m_clock;
    struct timespec  m_epoch;
  public:
    simpleClock_t():m_epoch({0,0}),m_clock(0){};

    void tic()
    {
      m_clock=clock();
      clock_gettime(CLOCK_MONOTONIC, &m_epoch);
      return;
    };

    timeProfile toc(  )
    {
      timeProfile result;
      // Local variables
      struct timespec  curr_time;
      clock_t now = clock();
      clock_gettime(CLOCK_MONOTONIC, &curr_time );
      // Diff clock
      unsigned long long diff = (now-m_clock)*1000;
      double dclock = ((double)diff)/CLOCKS_PER_SEC;
      // printf( "%llu, %llu %llu\n", now, m_clock, diff );
      // Diff time
      double dtime =
        ((double)curr_time.tv_sec + 1.0e-9*curr_time.tv_nsec) -
        ((double)m_epoch.tv_sec + 1.0e-9*m_epoch.tv_nsec);

      // Fill structure
      result.m_dclock = dclock/1000;
      result.m_dtime  = dtime;
      // Exit point
      return( result );
    };

};


#endif /* INCLUDE_TIMING_H_ */
