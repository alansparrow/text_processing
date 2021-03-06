// TextProcessing.cpp : Defines the entry point for the console application.
//

#include<string>
#include<iostream>
#include<sstream>

using namespace std;

namespace NS {
	template<typename T>
	string to_string(const T& t) {
		ostringstream os;
		os << t;
		return os.str();
	}

	struct bad_from_string : bad_cast {
		const char* what() const override {
			return "bad cast from string";
		}
	};

	template<typename T>
	T from_string(const string& s) {
		istringstream is{ s };
		T t;
		if (!(is >> t)) throw bad_from_string{};
		return t;
	}

	template<typename Target, typename Source>
	Target to(Source arg) {
		stringstream interpreter;
		Target result;

		if (!(interpreter << arg)
			|| !(interpreter >> result)
			|| !(interpreter >> ws).eof())
			throw runtime_error{ "to<>() failed" };

		return result;
	}
}

void f1() {
	string s1 = NS::to_string<double>(12.3333);
	string s2 = NS::to_string<double>(1 + 5 * 6 - 99 / 7);

	cout << s1.c_str() << endl;
	cout << s2.c_str() << endl;

	try {
		cout << NS::from_string<double>("2323.23") << endl;
		cout << NS::from_string<double>("a2a3") << endl;
	}
	catch (NS::bad_from_string e) {
		cout << "exception: " << e.what() << endl;
	}
}

void f2() {
	string s1{ "123" };
	int i1{ 10 };

	string s2 = NS::to<string, int>(234);
	cout << s2.c_str() << endl;

	int i2 = NS::to<int, string>("534 ");
	cout << i2 << endl;

}

class A {
private:
	int x;
public:
	A() : x{ 0 } {}
	A(int xx) : x{ xx } {}

	int get() const {
		return x;
	}

	friend ostream& operator<<(ostream& os, const A& a) {
		os << a.get();

		return os;
	}
	
};

void f3() {
	A a(323);
	//a.operator<<(cout);
	cout << a << endl;;
}

#include <regex>

void f4() {
	regex pat1{ R"(\w{2}\s*\d{5}(-\d{4})?)" };
	regex pat2{ R"(x.y)" };
	regex pat3{ R"(Ax*)" };
	regex pat4{ R"(Subject: (FW:|Re:)?(.*))" };
	regex pat5{ R"([^ueoaiy]+)" };
	regex pat6{ R"([.]+)" };
	regex pat7{ R"([\\]+)" };
	regex pat8{ R"(\d+)" };
	regex pat9{ R"([[:d:]]+)" };
	regex pat10{ "\\\"[[:alnum:]]+\\\"" };
	regex pat11{ "\\\\abc" }; // "\\abc"

	for (string line; getline(cin, line);) {
		smatch matches;
		if (regex_search(line, matches, pat11)) {
			cout << "matched: " << matches[0] << endl;
		}
		else
			cout << "unmatched" << endl;
	}
}

#include<string>
#include<fstream>
#include<sstream>

void f5() {
	regex pattern;

	string pat;
	cout << "enter pattern: ";
	getline(cin, pat);

	try {
		pattern = pat;
		cout << "pattern: " << pat << endl;
	} 
	catch (exception) {
		cout << pat << " is not a valid regular expression" << endl;
		exit(1);
	}

	cout << "now enter lines: " << endl;
	int lineno = 0;

	for (string line; getline(cin, line);) {
		++lineno;
		smatch matches;
		if (regex_search(line, matches, pattern)) {
			cout << "line " << lineno << ": " << line << endl;
			for (int i = 0; i < matches.size(); ++i) {
				cout << "\tmatches[" << i << "]: " << matches[i] << endl;
			}
		}
		else {
			cout << "didn't match" << endl;
		}
	}
}

/*
namespace NS {
	template<typename T>
	T from_string(const string& str) {
		T x;
		istringstream iss{ str };
		if (!(iss >> x)) throw bad_from_string{};

		return x;
	}
}
*/

#include<string>

int f6() {
	regex header{ R"(^[\w ]+(	[\w ]+)*$)" };
	regex row{ R"(^[\w ]+(	\d+)(	\d+)(	\d+)$)" };

	ifstream in{ "table.txt" };
	if (!in) cerr << "no input" << endl;

	string line;
	int lineno = 0;

	if (getline(in, line)) {
		smatch matches;
		if (!regex_match(line, matches, header))
			cerr << "no header" << endl;
	}

	int boys = 0;
	int girls = 0;

	while (getline(in, line)) {
		++lineno;
		smatch matches;
		if (!regex_match(line, matches, row))
			cerr << "bad line: " << lineno << endl;

		if (in.eof()) cout << "at eof" << endl;

		// check row
		int curr_boy = NS::from_string<int>(matches[1]);
		int curr_girl = NS::from_string<int>(matches[2]);
		int curr_total = NS::from_string<int>(matches[3]);
		if (curr_boy + curr_girl != curr_total)
			cerr << "bad row sum" << endl;
		//"Alle klasser"
		int idx = static_cast<string>(matches[0]).find("Alle klasser");
		if (idx != -1) { // last line
			if (curr_boy != boys) cerr << "boys don't add up" << endl;
			if (curr_girl != girls) cerr << "girls don't add up" << endl;
			if (!(in >> ws).eof()) cerr << "charaters after total line" << endl;
			return 0;
		}

		boys += curr_boy;
		girls += curr_girl;
	}

	cerr << "didn't find total line" << endl;
}


int main()
{
	f6();
	//f5();
	//f4();
	//f3();
	//f2();
	//f1();
    return 0;
}

