#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";

void function1();
void function2();
void function3();

rtems_name name = rtems_build_name('T', 'i', 's', 't');

static void createP(rtems_id* id, uint32_t** bufferAddress) {
  uint32_t numberOfElements = 10;

  RTEMS_ALIGNED( RTEMS_PARTITION_ALIGNMENT ) uint32_t buffer[numberOfElements];

  buffer[0] = 23;
  *bufferAddress = &buffer[0];
  printf("Content of %p: %u\n", &buffer[0], buffer[0]);
  printf("Doublecheck Content of %p: %u\n", *bufferAddress, **bufferAddress);

  printf("Creating partition\n");
  rtems_status_code status = rtems_partition_create(name, buffer, sizeof(buffer), sizeof(buffer), RTEMS_DEFAULT_ATTRIBUTES, id);

  printf("Afterwards -> Content of %p: %u\n", &buffer[0], buffer[0]);
  printf("Afterwards -> Doublecheck Content of %p: %u\n", *bufferAddress, **bufferAddress);
}

static void getBuffer(rtems_id id, uint32_t** givenBufferAddress) {
  rtems_status_code status;
  
  printf("Getting buffer...\n");
  status = rtems_partition_get_buffer(id, (void**) givenBufferAddress);
  directive_failed(status, "ErrorMsg: ");
  printf("Buffer received. It's adress is: %p\n", *givenBufferAddress);
}

static void manipulateBuffer(uint32_t* buffer) {
  printf("Setting the buffers content to 42...\n");
  *buffer = 42;
  printf("Checking the buffers content: %u\n", *buffer);
}

static rtems_task Task(rtems_task_argument argument) {
  rtems_id ids;
  uint32_t* bufferAddress;

  createP(&ids, &bufferAddress);
  printf("Left method\n");
  printf("ID: %u\n", ids);
  printf("BufferAddress: %p\n", bufferAddress);
  printf("BufferContent: %u\n", *bufferAddress);
  printf("Entering next method!\n");

  uint32_t* givenBufferAddress;
  getBuffer(ids, &givenBufferAddress);

  if(givenBufferAddress == bufferAddress) {
    printf("Both Addresses are equal\n");
  } else {
    printf("Missmatching addresses!\n");
    printf("BufferAddress is: %p", bufferAddress);
    printf("GivenBufferAddress is: %p", givenBufferAddress);
  }

  printf("The given Buffers content is: %u\n", *givenBufferAddress);

  printf("Entering next method!\n");
  manipulateBuffer(givenBufferAddress);
  printf("Buffers Content after leaving the manipulation method: %u\n", *givenBufferAddress);

  if(true) {
    return;
  }

  puts("HELLO WORLD!!!");
  //rtems_create_seg_task(name, prio, numberOfFunctions, arrayWithFunctions)
  uint32_t numberOfFunctions = 3;

  //struct needs to be created -> create partition which contains the struct
  //needs to have size of struct default size + sizeofpointer * numOfFunctions
  rtems_status_code status;
  rtems_name name = rtems_build_name('P', '1', ' ', ' ');
  uintptr_t buffer[numberOfFunctions];
  rtems_id id;

  status = rtems_partition_create(name, buffer, sizeof(buffer), sizeof(buffer), RTEMS_DEFAULT_ATTRIBUTES, &id);
  directive_failed(status, "ErrorMsg");

  printf("Partition ID: %u", id);

  puts("FINISHED");
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

  puts("Exiting task");
  rtems_task_exit();
}