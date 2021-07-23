#ifndef __TRISTAN_SYS_CONFIG_H
#define __TRISTAN_SYS_CONFIG_H

/**
 * There are some header files that need to be included
 * before the definition of the configuration.
 * Because I want my includations to be grouped, 
 * those includations here are duplicates, which should
 * not be the cause of problems.
 */
#include <tmacros.h>

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK     1000
#define CONFIGURE_MAXIMUM_TASKS             10
#define CONFIGURE_MAXIMUM_PERIODS           10
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT

#define CONFIGURE_MAXIMUM_SEGMENTS 20
#define CONFIGURE_MAXIMUM_COMMUNICATION_MEMORY 512

/**
 * There are some header files that need to be included
 * after the definition of the configuration.
 * Because I want my includations to be grouped, 
 * those includations here are duplicates, which should
 * not be the cause of problems.
 */
#include <rtems/confdefs.h>

#endif