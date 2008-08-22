#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

double bin_width = 1.0;
double range_min = 0.0;
double range_max = 0.0;

void print_usage(char **argv) {
	cerr
		<< "Syntax: " << argv[0] << " [-w <width>] [-r <range>]" << endl
		<< "A histogram over the values in stdin is written to stdout." << endl
		<< endl
		<< "Options:" << endl
		<< "  -h show this usage information" << endl
		<< "  -w <width> set width of bins (default: " << bin_width << ")" << endl
		<< "  -r <range> set range to be written in any case (e.g. \"-r 0..20\")" << endl;
}

class InvalidRangeException {};

pair<double,double> string_to_range(char* str) {
	string s(str);
	string::size_type n = s.find("..", 0);
	if (n == string::npos) {
		throw InvalidRangeException();
	}
	string min_s = s.substr(0, n);
	string max_s = s.substr(n+2);
	return make_pair(atof(min_s.c_str()), atof(max_s.c_str()));
}


void handleargs(int argc, char **argv) {
	extern char *optarg;
	extern int optopt;
	const char * options = "hw:r:";
	int c;

	while ((c = getopt(argc, argv, options)) != -1) {
		switch (c) {
		case 'h':
			print_usage(argv);
			exit(0);
		case 'w':
			bin_width = atof(optarg);
			if (bin_width <= 0.0) {
				cerr << "Width of bin must be > 0" << endl;
				exit(-1);
			}
			break;
		case 'r':
			try {
				pair<double,double> range = string_to_range(optarg);
				range_min = range.first;
				range_max = range.second;
			}
			catch (InvalidRangeException) {
				cerr << "Invalid range: \"" << optarg << "\"" << endl;
				exit(-1);
			}
			break;
		case '?':
			cerr << "Option \"-" << char(optopt) << "\" invalid" << endl;
			exit(-1);
		default:
			exit(-1);
		}
	}
}


int main(int argc, char *argv[]) {

	handleargs(argc, argv);

	// positive part of histogram, position 0 contains value for 0
	vector<int> histogram_pos((size_t)fabs(range_max/bin_width));
	// negative part of histogram, position 0 remains unused
	vector<int> histogram_neg((size_t)fabs(range_min/bin_width));
	int total_count = 0;

	string line;
	while (getline(cin, line)) {
		string::size_type n = line.find_first_not_of(" \t", 0);
		// parse line if it isn't empty
		if (n != string::npos) {
			// remove all leading tabs and whitespaces from input line
			line.erase(0, n);
			istringstream iss(line);
			double d;
			iss >> d;
			// TODO: where is trunc on the suns ?
			// int value = (int)trunc((d + bin_width/2) / bin_width);
			int value;
			if (d >= 0.0) {
				value = (int)((d + bin_width/2.0) / bin_width);
			} else {
				value = (int)((d - bin_width/2.0) / bin_width);
			}
			// cout << d << " --> value=" << value << endl;

			if (value >= 0) {
				if ((int)histogram_pos.size() <= value) {
					histogram_pos.resize(value+1, 0);
				}
				++total_count;
				++histogram_pos[value];
			} else {
				value = -value;
				if ((int)histogram_neg.size() <= value) {
					histogram_neg.resize(value+1, 0);
				}
				++total_count;
				++histogram_neg[value];
			}
		}
	}

	// write output
	for (int i = histogram_neg.size()-1; i > 0; --i) {
		cout << -bin_width*i << '\t' << histogram_neg[i]/(bin_width*total_count) << endl;
	}
	for (size_t j = 0; j < histogram_pos.size(); ++j) {
		cout << bin_width*j << '\t' << histogram_pos[j]/(bin_width*total_count) << endl;
	}
}
