/*
 * Copyright (c) 2016 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_STATUS_H
#define _RTEMS_SCORE_STATUS_H

#include <rtems/score/basedefs.h>

#include <errno.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Status code parts for the Classic API.
 *
 * Must be in synchronization with rtems_status_code.
 */
typedef enum {
  STATUS_CLASSIC_INCORRECT_STATE = 14,
  STATUS_CLASSIC_INTERNAL_ERROR = 13,
  STATUS_CLASSIC_INVALID_NUMBER = 10,
  STATUS_CLASSIC_INVALID_PRIORITY = 19,
  STATUS_CLASSIC_INVALID_SIZE = 8,
  STATUS_CLASSIC_NO_MEMORY = 26,
  STATUS_CLASSIC_NOT_DEFINED = 11,
  STATUS_CLASSIC_NOT_OWNER_OF_RESOURCE = 23,
  STATUS_CLASSIC_OBJECT_WAS_DELETED = 7,
  STATUS_CLASSIC_PROXY_BLOCKING = 29,
  STATUS_CLASSIC_RESOURCE_IN_USE = 12,
  STATUS_CLASSIC_SUCCESSFUL = 0,
  STATUS_CLASSIC_TIMEOUT = 6,
  STATUS_CLASSIC_TOO_MANY = 5,
  STATUS_CLASSIC_UNSATISFIED = 13
} Status_Classic;

/**
 * @brief Macro to build a status code from Classic and POSIX API parts.
 */
#define STATUS_BUILD( classic_status, posix_status ) \
  ( ( ( (unsigned int) ( posix_status ) ) << 8 ) | ( classic_status ) )

/**
 * @brief Macro to get the Classic API status code.
 */
#define STATUS_GET_CLASSIC( status ) \
  ( ( status ) & 0xff )

/**
 * @brief Macro to get the POSIX API status code.
 *
 * Performs an arithmetic shift to reconstruct a negative POSIX status.
 */
#define STATUS_GET_POSIX( status ) \
  ( ( ( (int) ( status ) ) | 0xff ) >> 8 )

/**
 * @brief Status codes.
 */
typedef enum {
  STATUS_BARRIER_AUTOMATICALLY_RELEASED =
    STATUS_BUILD( STATUS_CLASSIC_SUCCESSFUL, PTHREAD_BARRIER_SERIAL_THREAD ),
  STATUS_DEADLOCK =
    STATUS_BUILD( STATUS_CLASSIC_INCORRECT_STATE, EDEADLK ),
  STATUS_FLUSHED =
    STATUS_BUILD( STATUS_CLASSIC_UNSATISFIED, EAGAIN ),
  STATUS_INCORRECT_STATE =
    STATUS_BUILD( STATUS_CLASSIC_INCORRECT_STATE, EINVAL ),
  STATUS_INTERRUPTED =
    STATUS_BUILD( STATUS_CLASSIC_INTERNAL_ERROR, EINTR ),
  STATUS_INVALID_NUMBER =
    STATUS_BUILD( STATUS_CLASSIC_INVALID_NUMBER, EINVAL ),
  STATUS_INVALID_PRIORITY =
    STATUS_BUILD( STATUS_CLASSIC_INVALID_PRIORITY, EINVAL ),
  STATUS_MAXIMUM_COUNT_EXCEEDED =
    STATUS_BUILD( STATUS_CLASSIC_INTERNAL_ERROR, EOVERFLOW ),
  STATUS_MESSAGE_INVALID_SIZE =
    STATUS_BUILD( STATUS_CLASSIC_INVALID_SIZE, EMSGSIZE ),
  STATUS_MESSAGE_QUEUE_WAIT_IN_ISR =
    STATUS_BUILD( STATUS_CLASSIC_INTERNAL_ERROR, EAGAIN ),
  STATUS_MESSAGE_QUEUE_WAS_DELETED =
    STATUS_BUILD( STATUS_CLASSIC_OBJECT_WAS_DELETED, EBADF ),
  STATUS_MINUS_ONE =
    -1,
  STATUS_MUTEX_CEILING_VIOLATED =
    STATUS_BUILD( STATUS_CLASSIC_INVALID_PRIORITY, EINVAL ),
  STATUS_NESTING_NOT_ALLOWED =
    STATUS_BUILD( STATUS_CLASSIC_UNSATISFIED, EDEADLK ),
  STATUS_NO_MEMORY =
    STATUS_BUILD( STATUS_CLASSIC_NO_MEMORY, EINVAL ),
  STATUS_NOT_DEFINED =
    STATUS_BUILD( STATUS_CLASSIC_NOT_DEFINED, EINVAL ),
  STATUS_NOT_OWNER =
    STATUS_BUILD( STATUS_CLASSIC_NOT_OWNER_OF_RESOURCE, EPERM ),
  STATUS_OBJECT_WAS_DELETED =
    STATUS_BUILD( STATUS_CLASSIC_OBJECT_WAS_DELETED, EINVAL ),
  STATUS_PROXY_BLOCKING =
    STATUS_BUILD( STATUS_CLASSIC_PROXY_BLOCKING, EINVAL ),
  STATUS_RESOURCE_IN_USE =
    STATUS_BUILD( STATUS_CLASSIC_RESOURCE_IN_USE, EBUSY ),
  STATUS_RESULT_TOO_LARGE =
    STATUS_BUILD( STATUS_CLASSIC_UNSATISFIED, ERANGE ),
  STATUS_SUCCESSFUL =
    STATUS_BUILD( STATUS_CLASSIC_SUCCESSFUL, 0 ),
  STATUS_TIMEOUT =
    STATUS_BUILD( STATUS_CLASSIC_TIMEOUT, ETIMEDOUT ),
  STATUS_TOO_MANY =
    STATUS_BUILD( STATUS_CLASSIC_TOO_MANY, EAGAIN ),
  STATUS_UNAVAILABLE =
    STATUS_BUILD( STATUS_CLASSIC_UNSATISFIED, EBUSY ),
  STATUS_UNSATISFIED =
    STATUS_BUILD( STATUS_CLASSIC_UNSATISFIED, EAGAIN )
} Status_Control;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTEMS_SCORE_STATUS_H */
