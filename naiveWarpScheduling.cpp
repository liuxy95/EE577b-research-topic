#include <vector>
#include <stdlib.h>
#include "thread.h"
using namespace std;

#define BLOCK_SIZE 128 // number of threads assigned to a single SM
#define WARP_SIZE 32

int main(){
	//vector<THREAD> threadList;
	//for(int i=0; i<BLOCK_SIZE; i++){
	//	threadList.push_back(THREAD(i, i % WARP_SIZE));
	//}

	// initial threads and group them into warps
	vector< vector<THREAD> > warpList;
	for(int i=0; i<(BLOCK_SIZE/WARP_SIZE); i++){
		vector<THREAD> threadList;
		for(int j=0; j<WARP_SIZE; j++)
			threadList.push_back(THREAD(i*WARP_SIZE+j, i));
		warpList.push_back(threadList);
	}


	int clk = 0; // clock counter
	vector< vector<int> > instMem;
	/****************************************************
	*****************************************************
	initialize instruction memory here
	*****************************************************
	****************************************************/
	vector< vector<int> > *instMemPtr = &instMem;

	int slectWarpID = 0;
	while(1){
		for(int i=0; i<WARP_SIZE; i++){
			int PC_exceed = warpList[slectWarpID][i].instFetch();
			if(PC_exceed==-1) break;
		}
	}

}