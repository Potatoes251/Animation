// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Engine.h"
#include "MySimulation.h"

int main()
{
	MySimulation simulation;
	Run(&simulation, 1400, 800);

	return 0;
}

