#include "main.h"
#include "cmsis_os.h"

void StartDefaultTask(void const *argument)
{
	(void)argument;
	for (;;)
	{
		osDelay(1);
	}
}
