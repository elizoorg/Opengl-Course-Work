

#define GLEW_STATIC
#include <GL/glew.h>

#include "Core.h"

int main(void)
{
	
	Core core;
	core.Init();
	core.Loop();
	
	return 0;
}


