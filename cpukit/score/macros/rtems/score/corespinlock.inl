/*  macros/rtems/score/corespinlock.inl
 *
 *  This include file contains all of the inlined routines associated
 *  with core spinlocks.
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

#ifndef _RTEMS_SCORE_CORESPINLOCK_INL
#define _RTEMS_SCORE_CORESPINLOCK_INL

#define _CORE_spinlock_Is_busy(_the_spinlock) \
  ((_the_spinlock)->users != 0)

#endif
/* end of include file */
