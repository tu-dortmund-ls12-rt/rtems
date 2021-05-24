#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";

void function1(void);
void function2(void);
void function3(void);

rtems_name name = rtems_build_name('T', 'r', 'S', 'e');

static rtems_task Task(rtems_task_argument argument) {

}

rtems_task Init(
	rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_id task_id;

  TEST_BEGIN();

  /*
  Segmented Task Information
  */
  rtems_name taskName = rtems_build_name('S', 'e', 'g', '1');
  rtems_task_priority taskPriority = 2;
  size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
  rtems_mode taskModes = RTEMS_DEFAULT_MODES;
  rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;

  status = rtems_task_create_segmented_slfp(taskName, taskPriority, taskStackSize, taskModes, taskAttributes);
  directive_failed(status, "ErrorMsg: ");

  status = rtems_task_create(rtems_build_name('I', 'D', 'L', 'E'), (rtems_task_priority) 254, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES,
                                    RTEMS_DEFAULT_ATTRIBUTES, &task_id);
  directive_failed(status, "Task creation failed");

  status = rtems_task_start(task_id, Task, (rtems_task_argument) NULL);
  directive_failed(status, "Task start failed");

  puts("Exiting task");
  rtems_task_exit();
}