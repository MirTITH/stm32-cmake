#include "main.h"
#include "cmsis_os.h"
#include "user_cpp.hpp"

void StartDefaultTask(void const *argument)
{
	(void)argument;
	hello_cpp();
	for (;;)
	{
		osDelay(1);
	}
}
