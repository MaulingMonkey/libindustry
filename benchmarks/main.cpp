#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif //def _MSC_VER
#include <iostream>


void benchmark_ruby();

int main() {
	using namespace std;

#ifdef _DEBUG
#ifdef _MSC_VER
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_INTENSITY );
#endif //def _MSC_VER
	cout << "                                        \n";
	cout << " !!! NOTE:  BENCHMARKING DEBUG MODE !!! \n";
	cout << "                                        \n";
#ifdef _MSC_VER
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );
#endif //def _MSC_VER
	cout << "\n";
#endif //def _DEBUG

	benchmark_ruby();
}
