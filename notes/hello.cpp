#include<cstdio>
#include<iostream>

//inclue this to nor have to use std:: all the time
using namespace std;

int main(){
	
	printf("helloo world!\n");
	
	//without this its gcc compatible
	std::cout<<"helloo world!\n";
		
		//end flushes and new line
	std::cout<<"helloo world!" << std::endl;
		
	cout<<"helloo world!" << endl;
	
	const int i =7;
	
	//tricks the compile to not error to try to change
	//still dosent change
	*(int *) &i =5;
	
	cout<< i << endl;
	
	//const volatile only if hardware is the onlyone to change it
	
	//int &r=i; yes  //int &r; no
	
	
	int j=5;
	int k=7;
	
	cout <<j<<" " << k << endl;
	cswap(&j,&k);
	cout << j << " " << k << endl;
	cppswap(j,k);
	cout << j << " " << k << endl;
	
	return 0;
}

void cswap(int *x, int *y)
{
	//c style swap
	int temp;
	
	temp=*x;
	*x=*y;
	*y=temp;
}

void cppswap(int &x, int &y)
{
	int temp;
	
	temp = x;
	x=y;
	y=temp;
}