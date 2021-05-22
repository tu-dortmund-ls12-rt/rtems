#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/cpuuse.h>
#include <tmacros.h>

#include <rtems/rtems/segtaskdata.h>

const char rtems_test_name[] = "SPRMSCHED 1";

static rtems_task Task(rtems_task_argument argument) {
    puts("Hello World");
}

static rtems_task Init(
	rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_id task_id;

  TEST_BEGIN();

    status = rtems_task_create(rtems_build_name('N', 'R', '1', ' '), (rtems_task_priority) 2, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES,
                                    RTEMS_DEFAULT_ATTRIBUTES, &task_id);
    directive_failed(status, "Task creation failed");

    status = rtems_task_start(task_id, Task, (rtems_task_argument) NULL);
    directive_failed(status, "Task start failed");

  rtems_task_exit();
}

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK     1000
#define CONFIGURE_MAXIMUM_TASKS             2
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION
#define CONFIGURE_INIT

#include <rtems/confdefs.h>