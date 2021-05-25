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
  printf("QUICK DEBUG Outside: %zu\n", sizeof(Segmented_Task_SLFP_Task));
  printf("QUICK DEBUG OUTSIDE: %u", CONFIGURE_MAXIMUM_SEGMENTS);

  printf("---- DEBUGGING BEFORE THE TASK CREATION ----");
  printf("Printing all informations of the taskCopy.\n\n");
  printf("TaskCopy adress: %p\n", &taskCopy);
  printf("TaskCopy size: %zu\n", sizeof(taskCopy));
  printf("Underlying struct size: %zd\n", sizeof(Segmented_Task_SLFP_Task));
  printf("Finished debugging before\n");

  status = rtems_task_create_segmented_slfp(taskName, taskPriority, taskStackSize, taskModes,
                                            taskAttributes, numberOfSegments, segmentFunctions,
                                            segmentPriorities, &taskCopy);
  directive_failed(status, "ErrorMsg: ");

  /*rtems_test_assert(taskCopy.base.taskName == taskName);
  rtems_test_assert(taskCopy.base.taskPriority == 3);
  rtems_test_assert(taskCopy.base.taskStackSize == taskStackSize);
  rtems_test_assert(taskCopy.base.taskModes == taskModes);
  rtems_test_assert(taskCopy.base.taskAttributes == taskAttributes);
  rtems_test_assert(taskCopy.base.numberOfSegments == numberOfSegments);
  rtems_test_assert(taskCopy.base.segments[0].function == function1);
  rtems_test_assert(taskCopy.base.segments[1].function == function2);
  rtems_test_assert(taskCopy.base.segments[2].function == function3);*/


  /*
  Somehow there is a problem with the priority assignment of the struct copy.
  Debugging it.
  */

  printf("----------- DEBUGGING -----------\n");
  printf("Printing all informations of the taskCopy.\n\n");
  printf("TaskCopy adress: %p\n", &taskCopy);
  printf("TaskCopy size: %zu\n", sizeof(taskCopy));
  printf("Underlying struct size: %zd\n", sizeof(Segmented_Task_SLFP_Task));

  printf("Expected taskCopy name: %u -> Received name: %u\n", taskName, taskCopy.base.taskName);
  printf("Checkking assertion: ");
  rtems_test_assert(taskName == taskCopy.base.taskName);
  printf("PASS!\n");

  printf("Expected taskCopy taskPriority: %u -> Received priority: %u\n", segmentPriorities[0], taskCopy.base.taskPriority);
  printf("Checkking assertion: ");
  rtems_test_assert(segmentPriorities[0] == taskCopy.base.taskPriority);
  printf("PASS!\n");

  printf("Expected taskCopy taskStackSize: %zu -> Received priority: %zu\n", taskStackSize, taskCopy.base.taskStackSize);
  printf("Checkking assertion: ");
  rtems_test_assert(taskStackSize == taskCopy.base.taskStackSize);
  printf("PASS!\n");

  printf("Expected taskCopy taskModes: %u -> Received modes: %u\n", taskModes, taskCopy.base.taskModes);
  printf("Checkking assertion: ");
  rtems_test_assert(taskModes == taskCopy.base.taskModes);
  printf("PASS!\n");

  printf("Expected taskCopy taskAttributes: %u -> Received attributes: %u\n", taskAttributes, taskCopy.base.taskAttributes);
  printf("Checkking assertion: ");
  rtems_test_assert(taskAttributes == taskCopy.base.taskAttributes);
  printf("PASS!\n");

  printf("Expected taskCopy numberOfSegments: %u -> Received number: %u\n", numberOfSegments, taskCopy.base.numberOfSegments);
  printf("Checkking assertion: ");
  rtems_test_assert(numberOfSegments == taskCopy.base.numberOfSegments);
  printf("PASS!\n\n");

  printf("--- Checking the segments ---\n\n");
  for(uint32_t i = 0; i < numberOfSegments; i++) {
    printf("Segment: %u\n", i);
    printf("Segments adress: %p\n", &(taskCopy.base.segments[i]));
    
    printf("Expected taskCopy segment %u function pointer adress: %p -> Received adress: %p\n", i, function1, taskCopy.base.segments[i].function);
    printf("Checkking assertion: ");
    rtems_test_assert(function1 == taskCopy.base.segments[i].function);
    printf("PASS!\n");

    printf("Expected taskCopy segment %u priority: %u -> Received priority: %u\n", i, segmentPriorities[i], taskCopy.priorities[i]);
    printf("Checkking assertion: ");
    rtems_test_assert(segmentPriorities[i] == taskCopy.priorities[i]);
    printf("PASS!\n");
  }

  printf("\nPassed the debugging!\n");

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