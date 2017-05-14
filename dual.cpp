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
#define TIPPING_POINT 9


//insertion sort
template <class iter>
void insertionSort(iter begin, iter end, bool log) {

	if(end - begin <= 1) {
		return;
	}

	if(log) {
		cout<<"LOG: Got list of length "<<(end - begin)<<" to sort"<<endl;
	}

	for(iter it = begin + 1; it != end; it++) {
		typename iter::value_type a = *it;

		if(log) {
			cout<<"LOG: Saving "<<a<<" to insert later"<<endl;
		}

		iter it2 = it - 1;

		numberOfComparisons++;
		while(it2 >= begin && *it2 > a) {

			if(log) {
				cout<<"LOG: Shifting "<<*it2<<" one spot to the right, it's bigger than "<<a<<endl;
			}

			numberOfAssignments++;
			numberOfComparisons++;
			*(it2 + 1) = *it2;
			it2--;
		}


		if(log) {
			cout<<"LOG: inserting "<<a<<" to where it should be (position "<<(it2 + 1 - begin)<<")"<<endl;
		}

		numberOfAssignments;
		*(it2 + 1) = a;

		if(log) {
			cout<<"LOG: List after iteration:"<<endl<<"LOG: ";
			iter it3 = begin;
			while(it3 != end) {
				cout<<*it3<<" ";
				it3++;
			}
			cout<<endl;
		}
	}
}

template <class iter>
void dualPivotQuicksort(iter begin, iter end, int div, bool log) {

	if(log) {
		cout<<"LOG: Got a chunk to partition. Chunk contents: ";
		iter it3 = begin;
		while(it3 != end) {
			cout<<*it3<<" ";
			it3++;
		}
		cout<<endl;
	}

	int len = end - begin;

	if(len < TIPPING_POINT) {
		insertionSort(begin, end, log);
		return;
	}
	int third = len/div;

	iter m1 = begin + third;
	iter m2 = end - 1 - third;

	if(m1 <= begin) {
		m1 = begin + 1;
	}

	if(m2 >= (end-1)) {
		m2 = end - 2; //second to last element
	}


	numberOfComparisons++;
	numberOfAssignments += 4;
	if(*m1 < *m2) {
		std::swap(*begin, *m1);
		std::swap(*(end-1), *m2);
	} else {
		std::swap(*begin, *m2);
		std::swap(*(end-1), *m1);
	}

	typename iter::value_type pivot1 = *begin;
	typename iter::value_type pivot2 = *(end-1);

	iter less = begin + 1;
	iter great = end - 2;

	for(iter k = less; k <= great; k++) {
		if(*k < pivot1) {
			numberOfComparisons++;
			numberOfAssignments += 2;
			std::swap(*k, *less);
			less++;
		} else if(*k >pivot2) {
			numberOfComparisons += 2;
			numberOfComparisons++;
			while(k < great && *great > pivot2) {
				numberOfComparisons++;
				great--;
			}
			numberOfAssignments += 2;
			std::swap(*k, *great);
			great--;

			numberOfComparisons++;
			if(*k < pivot1) {
				numberOfAssignments+= 2;
				std::swap(*k, *less);
				less++;
			}
		} else {
			numberOfComparisons += 2;
		}
	}

	int dist = great - less;

	if(dist < 13) {
		div++;
	}

	numberOfAssignments += 4;
	std::swap(*(less - 1), *begin);
	std::swap(*(great + 1), *(end-1));

	//leftmost part - from beginning to less - 2
	//but we give index of the first element too far == (less - 2) + 1
	dualPivotQuicksort(begin, less - 1, div, log);
	//less - 1 is first pivot
	//great + 1 is second pivot
	//rightmost part - great + 2 to end
	dualPivotQuicksort(great + 2, end, div, log);

	if(dist > len - 13 && pivot1 != pivot2) {
		for(iter k = less; k <= great; k++) {

			if(*k == pivot1) {
				numberOfComparisons++;
				numberOfAssignments += 2;
				std::swap(*k, *less);
				less++;
			} else if(*k == pivot2) {
				numberOfComparisons += 2;
				numberOfAssignments += 2;
				std::swap(*k, *great);
				great--;
				numberOfComparisons++;
				if(*k == pivot1) {
					numberOfAssignments += 2;
					std::swap(*k, *less);
					less++;
				}
			} else {
				numberOfComparisons += 2;
			}
		}
	}

	//if the middle section is not just equal elements left to right
	numberOfComparisons++;
	if(pivot1 < pivot2) {
		dualPivotQuicksort(less, great + 1, div, log);
	}

}

//quick sort
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

	dualPivotQuicksort(begin, end, 3, log);


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
	file.open("dualdata.txt");

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