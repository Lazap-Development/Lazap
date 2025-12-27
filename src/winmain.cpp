#ifdef _WIN32
#include <windows.h>

int main();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) { return main(); }

#endif