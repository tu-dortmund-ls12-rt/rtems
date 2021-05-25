#include <rtems/rtems/segtaskslfp.h>
#include <rtems/rtems/segtaskslfpimpl.h>

/*
TaskName -> From the user
TaskPriority -> The Priority of the first segment. TODO: Remove and get from list of segment priorities.
TaskStackSize -> From the user
TaskModes -> From the user
TaskAttributes -> From the user
*/
rtems_status_code rtems_task_create_segmented_slfp(rtems_name taskName, rtems_task_priority taskPriority,
                size_t taskStackSize, rtems_mode initialModes, rtems_attribute taskAttributes, uint32_t numberOfSegments,
                void (*segmentFunctions[]) (void), rtems_task_priority segmentPriorities[], Segmented_Task_SLFP_Task* taskCopy) {
    rtems_status_code status;
    rtems_id id;

    Segmented_Task_SLFP_Task segmentedTask;

    printf("QUICK DEBUG INSIDE: %zu", sizeof(Segmented_Task_SLFP_Task));
    printf("QUICK DEBUG OUTSIDE: %u", CONFIGURE_MAXIMUM_SEGMENTS);

    fillDataIntoSegTaskSLFP(&segmentedTask, taskName, taskStackSize, initialModes,
                taskAttributes, numberOfSegments, segmentFunctions, segmentPriorities);

    status = rtems_task_create(taskName, taskPriority, taskStackSize, initialModes, taskAttributes, &id);

    if(status == RTEMS_SUCCESSFUL) {
        ((Segmented_Task_Task*)(&segmentedTask))->taskId = id;
        *taskCopy = segmentedTask;
    }

    /*
    Debugging
    */

   printf("----------- DEBUGGING Inside Method for original -----------\n");
  printf("Printing all informations of the segmentedTask.\n\n");
  printf("SegmentedTask adress: %p\n", &segmentedTask);
  printf("SegmentedTask size: %zu\n", sizeof(segmentedTask));
  printf("Underlying struct size: %zd\n", sizeof(Segmented_Task_SLFP_Task));
  printf("Expected segmentedTask name: %u -> Received name: %u\n", taskName, segmentedTask.base.taskName);
  printf("Expected segmentedTask taskPriority: %u -> Received priority: %u\n", segmentPriorities[0], segmentedTask.base.taskPriority);
  printf("Expected segmentedTask taskStackSize: %zu -> Received priority: %zu\n", taskStackSize, segmentedTask.base.taskStackSize);
  printf("Expected segmentedTask taskModes: %u -> Received modes: %u\n", initialModes, segmentedTask.base.taskModes);
  printf("Expected segmentedTask taskAttributes: %u -> Received attributes: %u\n", taskAttributes, segmentedTask.base.taskAttributes);
  printf("Expected segmentedTask numberOfSegments: %u -> Received number: %u\n", numberOfSegments, segmentedTask.base.numberOfSegments);

  printf("--- Checking the segments ---\n\n");
  for(uint32_t i = 0; i < numberOfSegments; i++) {
    printf("Segment: %u\n", i);
    printf("Segments adress: %p\n", &(segmentedTask.base.segments[i]));
    printf("Expected segmentedTask segment %u function pointer adress: %p -> Received adress: %p\n", i, segmentFunctions[i], segmentedTask.base.segments[i].function);
    printf("Expected segmentedTask segment %u priority: %u -> Received priority: %u\n", i, segmentPriorities[i], segmentedTask.priorities[i]);
  }

  printf("\nPassed the debugging inside the method for the original one!\n\n");

   //-------------------------------

   printf("----------- DEBUGGING Inside Method for copy -----------\n");
  printf("Printing all informations of the taskCopy.\n\n");
  printf("taskCopy adress: %p\n", taskCopy);
  printf("taskCopy size: %zu\n", sizeof(*taskCopy));
  printf("Underlying struct size: %zd\n", sizeof(Segmented_Task_SLFP_Task));
  printf("Expected taskCopy name: %u -> Received name: %u\n", taskName, taskCopy->base.taskName);
  printf("Expected taskCopy taskPriority: %u -> Received priority: %u\n", segmentPriorities[0], taskCopy->base.taskPriority);
  printf("Expected taskCopy taskStackSize: %zu -> Received priority: %zu\n", taskStackSize, taskCopy->base.taskStackSize);
  printf("Expected taskCopy taskModes: %u -> Received modes: %u\n", initialModes, taskCopy->base.taskModes);
  printf("Expected taskCopy taskAttributes: %u -> Received attributes: %u\n", taskAttributes, taskCopy->base.taskAttributes);
  printf("Expected taskCopy numberOfSegments: %u -> Received number: %u\n", numberOfSegments, taskCopy->base.numberOfSegments);

  printf("--- Checking the segments ---\n\n");
  for(uint32_t i = 0; i < numberOfSegments; i++) {
    printf("Segment: %u\n", i);
    printf("Segments adress: %p\n", &(taskCopy->base.segments[i])); 
    printf("Expected taskCopy segment %u function pointer adress: %p -> Received adress: %p\n", i, segmentFunctions[i], taskCopy->base.segments[i].function);
    printf("Expected taskCopy segment %u priority: %u -> Received priority: %u\n", i, segmentPriorities[i], taskCopy->priorities[i]);
  }

  printf("\nPassed the debugging inside the method for the copy one!\n\n");

    return status;
}

void fillDataIntoSegTaskSLFP(Segmented_Task_SLFP_Task* task,
                rtems_name taskName, size_t taskStackSize,
                rtems_mode initialModes, rtems_attribute taskAttributes,
                uint32_t numberOfSegments, void (*functionPointer[]) (void),
                rtems_task_priority priorities[]) {
    fillDataIntoSegTask((Segmented_Task_Task*)task, taskName, priorities[0], taskStackSize, initialModes, taskAttributes, numberOfSegments, functionPointer);

    for(uint32_t i = 0; i < numberOfSegments; i++) {
        /*printf("New Loop\n");
        printf("Task->Priorities[%u]: %u\n", i, (task->priorities)[i]);
        printf("Priorities[%u]: %u\n", i, priorities[i]);*/
        (task->priorities)[i] = priorities[i];
        /*printf("After Assignment\n");
        printf("Task->Priorities[%u]: %u\n", i, (task->priorities)[i]);*/
    }

    printf("Prioritites were filled!\n");
}