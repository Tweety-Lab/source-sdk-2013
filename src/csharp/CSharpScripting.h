#pragma once
#include <string>

class CSharpScripting
{
public:
    static void InitMono();
    static void CleanupMono();
    static void RunCSharpMethod(const std::string& method);
};