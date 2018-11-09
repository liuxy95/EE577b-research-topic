//#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

#define ALUOP 1
#define LONGALUOP 2
#define MEMLD 3
#define MEMST 4
#define CBranch 5
#define UCBranch 6

#define LATENCY 5
#define CacheHitLatency 2
#define CacheMissPenalty 50

#define RAND_RANGE 10
#define MISSRATE 3 // miss rate = MISSRATE/RAND_RANGE
#define TAKENRATE 5 // for branch taken or not

class THREAD{
public:
	THREAD(int threadID, int warpID); // set threadID and warpID
	void setWarpID(int warpID); // set warpID, maybe useful in dynamic sceduling
	void activeThread(int activeMask);
	int instFetch(vector< vector<int> > *instMem); // fetch instruction with PC_ from instruction memory
	int instEx(); /* execute instrcutions, include ALU op, long latency ALU op,
				   	mem access(cache hit and miss), conditional branch, unconditional branch
				   	return latency. return 0 if no latency*/


private:
	int threadID_;
	int warpID_;
	int PC_; // program counter
	int activeFlag_; // thread active when set to 1, idle when set to 0
	vector<int> inst_; // instruction fetched from instruction memory
};

THREAD::THREAD(int threadID, int warpID){
	threadID_ = threadID;
	warpID_ = warpID;
}

void THREAD::setWarpID(int warpID){
	warpID_ = warpID;
}

void THREAD::activeThread(int activeMask){
	activeFlag_ = activeMask;
}

int THREAD::instFetch(vector< vector<int> > *instMem){
	if(PC_>= (instMem->size())) 
		return -1; // return -1 when PC exceed instruction memory boundary
	inst_ = *instMem[PC_]; // the first item indicate inst type, second is Rd, 3rd Ra, 4th Rb, 5th immaddress for branch
	return 0;
}

int THREAD::instEx(){
	switch(inst_[0]){
		case ALUOP:{ 
			PC_++;
			return 0;
		}
		case LONGALUOP:{
			PC_++;
			return LATENCY;
		}
		case MEMLD:{	// cache hit or miss is decided by random number, maybe we can improve it later
			if((rand() % RAND_RANGE) <= MISSRATE){
				PC_++;
				return CacheMissPenalty;
			}
			else{
				PC_++;
				return CacheHitLatency;
			}
		}
		case MEMST:{
			if((rand() % RAND_RANGE) <= MISSRATE){
				PC_++;
				return CacheMissPenalty;
			}
			else{
				PC_++;
				return CacheHitLatency;
			}
		}
		case CBranch:{
			if((rand() % RAND_RANGE) <= TAKENRATE){ // branch taken
				PC_ = inst_[4];
				return 0;
			}
			else{  // branch not taken
				PC_++;
				return 0;
			}
		}
		default: // something wrong, return -1
			return -1;
	}
}