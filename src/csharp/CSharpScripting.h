#pragma once
#include <string>
static class CSharpScripting
{
public:
	static void InitMono();
	static void CleanupMono();
	static void RunCSharpMethod(std::string method);
};

