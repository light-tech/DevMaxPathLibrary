#include "DevMax.h"

#include "../PathNormalize.cpp"

#define MAX_PATH 1024
static char buf[MAX_PATH];

void TestNormalize(const char *path, const char *expected)
{
	Normalize(path, buf, MAX_PATH);
	if (strcmp(buf, expected))
	{
		PrintString("[FAIL] Normalize ");
		PrintString(path);
		PrintString("\nWe expect: ");
		PrintString(expected);
		PrintString("\nBut you return: ");
		PrintString(buf);
		PrintString("\n");
	}
	else
	{
		PrintString("[SUCCESS]\n");
	}
}

int main(int argc, const char **argv)
{
	TestNormalize("C:\\a", "C:\\a");
	TestNormalize("C:\\b\\..\\c\\a.txt", "C:\\b\\a.txt");
	TestNormalize("C:\\b\\..\\.\\c\\a.txt", "C:\\b\\a.txt");
	// Can't go further than C: so we should either return error or stops there
	TestNormalize("C:\\b\\..\\..\\c\\a.txt", "C:\\b\\a.txt");
	return 0;
}