#include<iostream>
#include"OSPFobj.h"

using namespace std;
void main()
{
	OSPFobj obj1;
	obj1.build();
	obj1.~OSPFobj();
}