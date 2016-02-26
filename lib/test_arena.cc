#include "arena.h"
#include <iostream>
#include <cstdio>

using namespace Ucloud;
int main() {
	Arena* arena = new Arena();		
	for (int i = 1; i < 100000; ++i) {
		char* data = arena->Allocate(i);	
		//printf("data_size:%d\n", sizeof(data));
	}
	return 0;
}
