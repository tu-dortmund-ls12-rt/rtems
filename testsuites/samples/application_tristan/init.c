#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "system.h"

const char rtems_test_name[] = "APPLICATION TRISTAN";

rtems_task Init(
	rtems_task_argument argument
)
{
  custom_scenario_execute();
}