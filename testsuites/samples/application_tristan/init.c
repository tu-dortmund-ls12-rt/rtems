#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";

void function1(void);
void function2(void);
void function3(void);

void function1(void) {
  printf("I'm function1\n");
}

void function2(void) {
  printf("I'm function2\n");
}

void function3(void) {
  printf("I'm function3\n");
}

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
  Segmented Task:
  Name = Seg1
  Priorität = 2
  StackSize = Min
  Modes = Default
  Attributes = Default

  Segmente = 3
  Segment1:
    Function = function1
    Priorität = 3
  Segment 2:
    Function = function2
    Priorität = 4
  Segment 3:
    Function = function3
    Priorität = 5
  */
  rtems_name taskName = rtems_build_name('S', 'e', 'g', '1');
  rtems_task_priority taskPriority = 2;
  size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
  rtems_mode taskModes = RTEMS_DEFAULT_MODES;
  rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;
  uint32_t numberOfSegments = 3;
  void (*segmentFunctions[]) (void) = {function1, function2, function3};
  rtems_task_priority segmentPriorities[] = {3, 4, 5};
  
  Segmented_Task_SLFP_Task taskCopy;

  status = rtems_task_create_segmented_slfp(taskName, taskPriority, taskStackSize, taskModes,
                                            taskAttributes, numberOfSegments, segmentFunctions,
                                            segmentPriorities, &taskCopy);
  directive_failed(status, "ErrorMsg: ");

  rtems_test_assert(taskCopy.base.taskName == taskName);
  rtems_test_assert(taskCopy.base.taskPriority == 3);
  rtems_test_assert(taskCopy.base.taskStackSize == taskStackSize);
  rtems_test_assert(taskCopy.base.taskModes == taskModes);
  rtems_test_assert(taskCopy.base.taskAttributes == taskAttributes);
  rtems_test_assert(taskCopy.base.numberOfSegements == numberOfSegments);
  rtems_test_assert(taskCopy.base.segments[0].function == function1);
  rtems_test_assert(taskCopy.base.segments[1].function == function2);
  rtems_test_assert(taskCopy.base.segments[2].function == function3);

  printf("Entering the Priorityproblem\n---------------\n");
  printf("TaskCopy.priorities Address: %p", taskCopy.priorities);
  printf("TaskCopy.priorities[0]: %u -> Should be 3\n", taskCopy.priorities[0]);
  rtems_test_assert((taskCopy.priorities)[0] == 3);
  printf("TaskCopy.priorities[1]: %u -> Should be 4\n", taskCopy.priorities[1]);
  rtems_test_assert(taskCopy.priorities[1] == 4);
  printf("TaskCopy.priorities[2]: %u -> Should be 5\n", taskCopy.priorities[2]);
  rtems_test_assert(taskCopy.priorities[2] == 5);

  (*(taskCopy.base.segments[0].function))();
  (*(taskCopy.base.segments[1].function))();
  (*(taskCopy.base.segments[2].function))();

  status = rtems_task_create(rtems_build_name('I', 'D', 'L', 'E'), (rtems_task_priority) 254, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES,
                                    RTEMS_DEFAULT_ATTRIBUTES, &task_id);
  directive_failed(status, "Task creation failed");

  status = rtems_task_start(task_id, Task, (rtems_task_argument) NULL);
  directive_failed(status, "Task start failed");

  puts("Exiting task");
  rtems_task_exit();
}