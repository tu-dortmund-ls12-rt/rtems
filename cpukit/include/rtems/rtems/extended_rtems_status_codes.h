#ifndef _RTEMS_RTEMS_EXTENDED_RTEMS_STATUS_CODES_H
#define _RTEMS_RTEMS_EXTENDED_RTEMS_STATUS_CODES_H

/**
 * @ingroup RTEMSAPIClassicStatus
 *
 * @brief This enumeration provides status codes for directives of the Classic
 *   API.
 */
typedef enum {
  /**
   * @brief This status code indicates successful completion of a requested
   *   operation.
   */
  RTEMS_EXTENDED_SUCCESSFUL = 0,

  /**
   * @brief This status code indicates that a thread exitted.
   */
  RTEMS_EXTENDED_TASK_EXITTED = 1,

  /**
   * @brief This status code indicates that multiprocessing was not configured.
   */
  RTEMS_EXTENDED_MP_NOT_CONFIGURED = 2,

  /**
   * @brief This status code indicates that an object name was invalid.
   */
  RTEMS_EXTENDED_INVALID_NAME = 3,

  /**
   * @brief This status code indicates that an object identifier was invalid.
   */
  RTEMS_EXTENDED_INVALID_ID = 4,

  /**
   * @brief This status code indicates you have attempted to create too many
   *   instances of a particular object class.
   */
  RTEMS_EXTENDED_TOO_MANY = 5,

  /**
   * @brief This status code indicates that a blocking directive timed out.
   */
  RTEMS_EXTENDED_TIMEOUT = 6,

  /**
   * @brief This status code indicates the object was deleted while the thread
   *   was blocked waiting.
   */
  RTEMS_EXTENDED_OBJECT_WAS_DELETED = 7,

  /**
   * @brief This status code indicates that a specified size was invalid.
   */
  RTEMS_EXTENDED_INVALID_SIZE = 8,

  /**
   * @brief This status code indicates that a specified address was invalid.
   */
  RTEMS_EXTENDED_INVALID_ADDRESS = 9,

  /**
   * @brief This status code indicates that a specified number was invalid.
   */
  RTEMS_EXTENDED_INVALID_NUMBER = 10,

  /**
   * @brief This status code indicates that the item has not been initialized.
   */
  RTEMS_EXTENDED_NOT_DEFINED = 11,

  /**
   * @brief This status code indicates that the object still had resources in
   *   use.
   */
  RTEMS_EXTENDED_RESOURCE_IN_USE = 12,

  /**
   * @brief This status code indicates that the request was not satisfied.
   */
  RTEMS_EXTENDED_UNSATISFIED = 13,

  /**
   * @brief This status code indicates that an object was in wrong state for the
   *   requested operation.
   */
  RTEMS_EXTENDED_INCORRECT_STATE = 14,

  /**
   * @brief This status code indicates that the thread was already suspended.
   */
  RTEMS_EXTENDED_ALREADY_SUSPENDED = 15,

  /**
   * @brief This status code indicates that the operation was illegal on the
   *   calling thread.
   */
  RTEMS_EXTENDED_ILLEGAL_ON_SELF = 16,

  /**
   * @brief This status code indicates that the operation was illegal on a remote
   *   object.
   */
  RTEMS_EXTENDED_ILLEGAL_ON_REMOTE_OBJECT = 17,

  /**
   * @brief This status code indicates that the operation should not be called
   *   from this execution environment.
   */
  RTEMS_EXTENDED_CALLED_FROM_ISR = 18,

  /**
   * @brief This status code indicates that an invalid thread priority was
   *   provided.
   */
  RTEMS_EXTENDED_INVALID_PRIORITY = 19,

  /**
   * @brief This status code indicates that a specified date or time was invalid.
   */
  RTEMS_EXTENDED_INVALID_CLOCK = 20,

  /**
   * @brief This status code indicates that a specified node identifier was
   *   invalid.
   */
  RTEMS_EXTENDED_INVALID_NODE = 21,

  /**
   * @brief This status code indicates that the directive was not configured.
   */
  RTEMS_EXTENDED_NOT_CONFIGURED = 22,

  /**
   * @brief This status code indicates that the caller was not the owner of the
   *   resource.
   */
  RTEMS_EXTENDED_NOT_OWNER_OF_RESOURCE = 23,

  /**
   * @brief This status code indicates the directive or requested portion of the
   *   directive is not implemented.
   *
   * This is a hint that you have stumbled across an opportunity to submit code
   * to the RTEMS Project.
   */
  RTEMS_EXTENDED_NOT_IMPLEMENTED = 24,

  /**
   * @brief This status code indicates that an internal RTEMS inconsistency was
   *   detected.
   */
  RTEMS_EXTENDED_INTERNAL_ERROR = 25,

  /**
   * @brief This status code indicates that the directive attempted to allocate
   *   memory but was unable to do so.
   */
  RTEMS_EXTENDED_NO_MEMORY = 26,

  /**
   * @brief This status code indicates a device driver IO error.
   */
  RTEMS_EXTENDED_EXTENDED_IO_ERROR = 27,

  /**
   * @brief This status code is used internally by the implementation to indicate
   *   a blocking device driver call has been interrupted and should be reflected
   *   to the caller as interrupted.
   */
  RTEMS_EXTENDED_INTERRUPTED = 28,

  /**
   * @brief This status code is used internally by the implementation when
   *   performing operations on behalf of remote tasks.
   *
   * This is referred to as proxying operations and this status indicates that
   * the operation could not be completed immediately and the proxy is blocking.
   *
   * @par Notes
   * This status will not be returned to the user.
   */
  RTEMS_EXTENDED_PROXY_BLOCKING = 29,

  /**
   * @brief This status code is used internally by the implementation
   * when the given pointer is a null pointer.
   *
   * @par Notes
   * This status will not be returned to the user.
   * 
   * @par Notes
   * When this extended status code is casted to a RTEMS_STATUS_CODE
   * it will be mapped on RTEMS_INVALID_ADDRESS.
   */
  RTEMS_EXTENDED_NULL_POINTER = RTEMS_EXTENDED_INVALID_ADDRESS,

  /**
   * @brief This status code is used internally by the implementation
   * when the number of the maximum segments is exceeded.
   *
   * @par Notes
   * This status will not be returned to the user.
   * 
   * @par Notes
   * When this extended status code is casted to a RTEMS_STATUS_CODE
   * it will be mapped on RTEMS_TOO_MANY.
   */
  RTEMS_EXTENDED_TOO_MANY_SEGMENTS = RTEMS_EXTENDED_TOO_MANY,

  /**
   * @brief This status code is used internally by the implementation
   * when the index is out of bounds or otherwise invalid.
   *
   * @par Notes
   * This status will not be returned to the user.
   * 
   * @par Notes
   * When this extended status code is casted to a RTEMS_STATUS_CODE
   * it will be mapped on RTEMS_TOO_MANY.
   */
  RTEMS_EXTENDED_INVALID_INDEX = RTEMS_EXTENDED_INVALID_NUMBER,

  /**
   * @brief This status code is used internally by the implementation
   * when the next segment should be executed, but the final segment
   * was already reached.
   *
   * @par Notes
   * This status will not be returned to the user.
   * 
   * @par Notes
   * When this extended status code is casted to a RTEMS_STATUS_CODE
   * it will be mapped on RTEMS_ILLEGAL_ON_SELF.
   */
  RTEMS_EXTENDED_FINAL_SEGMENT = RTEMS_EXTENDED_ILLEGAL_ON_SELF,

  /**
   * @brief This status code is used internally by the implementation
   * when a routine is called while interrupts are still enabled.
   *
   * @par Notes
   * This status will not be returned to the user.
   * 
   * @par Notes
   * When this extended status code is casted to a RTEMS_STATUS_CODE
   * it will be mapped on RTEMS_EXTENDED_CALLED_FROM_ISR.
   */
  RTEMS_EXTENDED_INTERRUPTS_ENABLED = RTEMS_EXTENDED_CALLED_FROM_ISR
} rtems_extended_status_code;

#endif