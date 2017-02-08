#ifndef UTIL_H
#define UTIL_H
#include <iostream>

using namespace std;

typedef struct Hora{
	string::size_type sz; 
	int h;
	int m;
	Hora(){
		h = 0;
		m = 0;
	}
	Hora(vector<string> str){
		h = stoi(str[0], &sz);
		m = stoi(str[1], &sz);	
	}
	~Hora(){}
	Hora(int h, int m): h(h),m(m){}

	int get_minutos(){
		return 60 * h + m;
	}

}Hora;

typedef struct Data{
	string::size_type sz; 
	int d;
	int m;
	int a;
	Data(){}
	Data(vector<string> str){
		d = stoi(str[0], &sz);
		m = stoi(str[1], &sz);	
		a = stoi(str[2], &sz);	
	}
	~Data(){}

}Data;

#endif