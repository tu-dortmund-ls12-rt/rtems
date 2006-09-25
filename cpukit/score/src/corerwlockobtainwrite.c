/*
 *  SuperCore RWLock Handler -- Obtain RWLock for writing
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/system.h>
#include <rtems/score/corerwlock.h>
#include <rtems/score/states.h>
#include <rtems/score/thread.h>
#include <rtems/score/watchdog.h>

/*PAGE
 *
 *  _CORE_rwlock_Obtain_for_writing
 *
 *  This function waits for the rwlock to become available.  Optionally,
 *  a limit may be placed on the duration of the spin.  
 *
 *  Input parameters:
 *    the_rwlock    - the rwlock control block to initialize
 *    timeout_allowed - true if timeout allowed
 *    timeout         - the maximum number of ticks to spin
 *
 *  Output parameters:  NONE
 */

void _CORE_RWLock_Obtain_for_writing(
  CORE_RWLock_Control                 *the_rwlock,
  Objects_Id                           id,
  boolean                              wait,
  Watchdog_Interval                    timeout,
  CORE_RWLock_API_mp_support_callout   api_rwlock_mp_support
)
{
  ISR_Level       level;
  Thread_Control *executing = _Thread_Executing;

  /*
   *  If unlocked, then OK to read.
   *  Otherwise, we have to block.
   *  If locked for reading and no waiters, then OK to read.
   *  If any thread is waiting, then we wait.
   */

  _ISR_Disable( level );
    switch ( the_rwlock->current_state ) {
      case CORE_RWLOCK_UNLOCKED:
	the_rwlock->current_state = CORE_RWLOCK_LOCKED_FOR_WRITING;
	_ISR_Enable( level );
	executing->Wait.return_code = CORE_RWLOCK_SUCCESSFUL;
	return;

      case CORE_RWLOCK_LOCKED_FOR_READING:
      case CORE_RWLOCK_LOCKED_FOR_WRITING:
        break;
    }

    /*
     *  If the thread is not willing to wait, then return immediately.
     */

    if ( !wait ) {
      _ISR_Enable( level );
      executing->Wait.return_code = CORE_RWLOCK_UNAVAILABLE;
    }

    /*
     *  We need to wait to enter this critical section
     */

    _Thread_queue_Enter_critical_section( &the_rwlock->Wait_queue );
    executing->Wait.queue       = &the_rwlock->Wait_queue;
    executing->Wait.id          = id;
    executing->Wait.option      = CORE_RWLOCK_THREAD_WAITING_FOR_WRITE;
    executing->Wait.return_code = CORE_RWLOCK_SUCCESSFUL;
    _ISR_Enable( level );

    _Thread_queue_Enqueue( &the_rwlock->Wait_queue, timeout );

    /* return to API level so it can dispatch and we block */
}
