#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";
rtems_name segmentedTaskName = rtems_build_name('S', 'e', 'g', ' ');
rtems_name bufferTaskName = rtems_build_name('B', 'u', 'f', ' ');


//----------------------



//----------------------

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

    status = rtems_task_resume_segmented_slfp(taskId);
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
  rtems_task_exit();
}



// ------ Given schedule test ------

// ---------------------------------

// -------- Custom scenario --------

// ---------------------------------

void executeCustomScenario(void) {
  
}

rtems_task Init(
	rtems_task_argument argument
)
{
  given_schedule_test_execute();
}