#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";

static rtems_task Task(rtems_task_argument argument) {
    if(abs_double(-3.3) == 3.3) {
      puts("True");
    } else {
      puts("False");
    }
}

rtems_task Init(
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