#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std::chrono;

using std::cout;
using std::endl;
using std::vector;
using std::cin;
using std::ofstream;

long long numberOfComparisons;
long long numberOfAssignments;

#define NUMBER_OF_TRIES 10

template<class iter>
iter partition(iter begin, iter end, bool log) {

	int positionToSwap = rand() % (end - begin);
	std::swap(*begin, *(begin + positionToSwap));
	numberOfAssignments += 2;


	if(log) {
		cout<<"LOG: Got a chunk to partition. Chunk contents: ";
		iter it3 = begin;
		while(it3 != end) {
			cout<<*it3<<" ";
			it3++;
		}
		cout<<endl;
	}

	typename iter::value_type pivot = *begin;
	iter i = begin - 1;
	iter j = end;

	while(true) {
		do {
			i++;
			numberOfComparisons++;
		} while(*i < pivot);

		do {
			j--;
			numberOfComparisons++;
		} while(*j > pivot);

		if (i >= j) {

			if(log) {
				cout<<"LOG: Partitioned chunk contents: ";
				iter it3 = begin;
				while(it3 != end) {
					cout<<*it3<<" ";
					it3++;
				}
				cout<<endl<<"LOG: Returning position "<<(j - begin)<< " (value "<<*j<<")"<<endl;
			}
			return j;
		}

		numberOfAssignments += 2;
		std::swap(*i, *j);
	}
}


//quick sort
template<class iter>
void sort(iter begin, iter end, bool log) {

	if(log) {
		cout<<"LOG: Got a chunk to sort. Chunk contents: ";
		iter it3 = begin;
		while(it3 != end) {
			cout<<*it3<<" ";
			it3++;
		}
		cout<<endl;
	}

	if(begin + 1 != end) {
		iter p = partition(begin, end, log);
		sort(begin, p + 1, log);
		sort(p + 1, end, log);
	}


	if(log) {
		cout<<"LOG: Sorted chunk contents: ";
		iter it3 = begin;
		while(it3 != end) {
			cout<<*it3<<" ";
			it3++;
		}
		cout<<endl;
	}


}

template<class iter>
bool verifySorted(iter begin, iter end) {
	iter it = begin + 1;

	while(it != end) {
		if(*(it - 1) > *it) {
			return false;
		}
		it++;
	}

	return true;
}

int main() {
	srand(time(NULL));
	std::vector<int> aa;
	cout<<"Welcome to the sorting implementation presentation\nSelect input data size:"<<endl;

	int n;
	cin>>n;

	while(aa.empty()) {
		cout<<"Select input data type"<<endl;
		cout<<"1) randomly filled list"<<endl<<"2) Inversely sorted list"<<endl;

		int a;
		cin>>a;
		if(a == 1) {
			for(int i = 0; i < n; i++) {
				aa.push_back(rand() % 1000);
			}
		} else if(a == 2) {
			for(int i = 0; i < n; i++) {
				aa.push_back(n - i);
			}
		}
	}

	sort(aa.begin(), aa.end(), true);

	cout<<"Continue with generating graph data? (y/n)"<<endl;
	char c;
	cin>>c;

	if(c != 'Y' && c != 'y') {
		return 0;
	}

	long long totalTimeSorting = 0;

	ofstream file;
	file.open("quickdata.txt");

	for(int i = 100; i <= 100000; i += 100) {
		cout<<"Sorting... "<<i<<endl;
		numberOfAssignments = 0;
		numberOfComparisons = 0;
		long long micros = 0;
		aa.clear();
		aa.assign(i, 0);

		for(int iiii = 0; iiii < NUMBER_OF_TRIES; iiii++) {

			for(int j = 0; j < i; j++) {
				aa[j] = rand();
			}

			auto timeStart = high_resolution_clock::now();
			sort(aa.begin(), aa.end(), false);
			auto timeElapsed = high_resolution_clock::now() - timeStart;

			if(verifySorted(aa.begin(), aa.end()) == false) {
				cout<<" List sorted incorrectly! ";
			}

			micros += duration_cast<microseconds>(timeElapsed).count();
		}
		numberOfAssignments /= NUMBER_OF_TRIES;
		numberOfComparisons /= NUMBER_OF_TRIES;
		micros /= NUMBER_OF_TRIES;

//		cout<<"Took "<<millis<<" milliseconds, "<<numberOfAssignments<<" key assignments and "
//			<<numberOfComparisons<<" comparisons"<<endl;

		file<<i<<"\t"<<micros<<"\t"<<numberOfAssignments<<"\t"<<numberOfComparisons<<endl;

		totalTimeSorting += micros;
	}

	cout<<"In total it took "<<totalTimeSorting/1000000<<" seconds to sort the lists"<<endl;
}