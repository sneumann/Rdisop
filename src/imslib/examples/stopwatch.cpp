#include <iostream>
#include <vector>

#include <ims/utils/stopwatch.h>

using namespace std;

void do_stuff() {
	for (int j = 0; j < 10; ++j) {
		vector<int> v;
		for (int i = 0; i < 1000000; ++i) {
			v.push_back(i);	
		}
		v.clear();
	}
}


int main() {
	ims::Stopwatch sw;
	ims::ProcessStopwatch psw;
	do_stuff();
	cout << "real time: " << sw.elapsed() << "s\n";
	cout << "CPU time: " << psw.elapsed() << "s\n";
}
