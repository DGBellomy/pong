#include "pch.h"
#include "Win32App.h"
#include "Pong.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	Pong* pong = new Pong(800, 600);
	return Win32App::Run(pong, hInstance, showCmd);
}