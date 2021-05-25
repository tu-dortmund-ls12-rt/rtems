#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";
rtems_name segmentedTaskName = rtems_build_name('S', 'e', 'g', ' ');
rtems_name bufferTaskName = rtems_build_name('B', 'u', 'f', ' ');
rtems_name idleTaskName = rtems_build_name('I', 'd', 'l', 'e');

void function1(void);
void function2(void);
void function3(void);

void function1(void) {
  rtems_status_code status;
  printf("I'm function1\n");
  
  uint32_t x = 42;
  printf("Saving the value of %u into the communication memory", x);

  uint8_t* content = (uint8_t*) &x;
  size_t contentSize = sizeof(x) / sizeof(uint8_t);

  /*
  TODO:
  Continue here! The executing task has no reference on the segmented task
  datastructure and shall not get one. But what it does has is it's own
  task id.

  Concept change: For the API it is only possible for a segmented task to
  write to it's own communcation memory! The underlying implementation may
  look different, but the calling task should not be in need to pass it's
  own segmented task structure (which it does not have access to), nor it's
  own id.
  */
  //status = rtems_task_segmented_write_communication_memory(content, contentSize);
}

void function2(void) {
  printf("I'm function2\n");
}

void function3(void) {
  printf("I'm function3\n");
}

static rtems_task buffer(rtems_task_argument argument) {
  uint32_t secondsToWait = 3;

  rtems_status_code status;

  printf("Buffer is beeing executed.\n");

  for(uint32_t i = 0; i < 2; i++) {
    rtems_interval waitStart;
    rtems_interval currentTime;

    status = rtems_clock_get_seconds_since_epoch(&waitStart);
    directive_failed(status, "Receiving start time failed.");

    currentTime = waitStart;

    while(currentTime - waitStart < secondsToWait) {
      status = rtems_clock_get_seconds_since_epoch(&currentTime);
      directive_failed(status, "Receiving current time failed.");
    }

    // Unsuspend the segmented task (is there to simulate the external event)
    rtems_id taskId;
    status = rtems_task_ident(segmentedTaskName, RTEMS_SEARCH_ALL_NODES, &taskId);
    directive_failed(status, "Couldn't receive task ID.");
    
    printf("Resuming suspended task\n");

    status = rtems_task_resume(taskId);
    directive_failed(status, "Couldn't resume suspended task");
  }

  printf("Simulating active execution in the buffer task for 10 seconds...");
  secondsToWait = 10;
  rtems_interval simStart;
  rtems_interval lastDot;
  rtems_interval simCurrent;

  status = rtems_clock_get_seconds_since_epoch(&simStart);
  directive_failed(status, "Receiving sim start time failed");

  lastDot = simStart;
  simCurrent = simStart;

  while(simCurrent - simStart < secondsToWait) {
    if(simCurrent - lastDot >= 1) {
      printf(".");
      lastDot = simCurrent;
    }

    status = rtems_clock_get_seconds_since_epoch(&simCurrent);
    directive_failed(status, "Receiving current time failed.");
  }

  printf("\n");

  printf("Finished simulating active execution.\n");

  /*printf("Buffer is entering infinite loop");
  while(true);*/
  rtems_task_exit();
}

static rtems_task idle(rtems_task_argument argument) {
  printf("Non-Idle Task finished. It's missing out rtems_task_exit() directive so the terminal doesn't freeze and the system halts.\n");
}

rtems_task Init(
	rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_id segmentedTaskId;
  rtems_id bufferTaskId;
  rtems_id idleTaskId;

  TEST_BEGIN();

  // Clock init
  rtems_time_of_day time;
  time.ticks = 0;
  time.second = 0;
  time.minute = 0;
  time.hour = 0;
  time.day = 1;
  time.month = 1;
  time.year = 2000;

  status = rtems_clock_set(&time);
  directive_failed(status, "Failed to set the system clock");

  /*
  Segmented Task:
  Name = Seg
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

  rtems_task_priority taskPriority = 2;
  size_t taskStackSize = RTEMS_MINIMUM_STACK_SIZE;
  rtems_mode taskModes = RTEMS_DEFAULT_MODES;
  rtems_attribute taskAttributes = RTEMS_DEFAULT_ATTRIBUTES;
  uint32_t numberOfSegments = 3;
  void (*segmentFunctions[]) (void) = {function1, function2, function3};
  rtems_task_priority segmentPriorities[] = {3, 4, 6};

  // Task Creation

  status = rtems_task_create_segmented_slfp(segmentedTaskName, taskPriority, taskStackSize, taskModes,
                                            taskAttributes, numberOfSegments, segmentFunctions,
                                            segmentPriorities, &segmentedTaskId);
  directive_failed(status, "Segmented Task creation failed.");

  status = rtems_task_create(bufferTaskName, 5, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &bufferTaskId);
  directive_failed(status, "Buffer Task creation failed.");

  status = rtems_task_create(idleTaskName, 254, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &idleTaskId);
  directive_failed(status, "Idle Task creation failed.");

  // Task Starting

  status = rtems_task_start_segmented_slfp(segmentedTaskId);
  directive_failed(status, "Start of segmented Task failed.");

  status = rtems_task_start(bufferTaskId, buffer, (rtems_task_argument) NULL);
  directive_failed(status, "Start of buffer Task failed.");

  status = rtems_task_start(idleTaskId, idle, (rtems_task_argument) NULL);
  directive_failed(status, "Start of idle Task failed.");

  puts("Exiting initialization Task");
  rtems_task_exit();
}