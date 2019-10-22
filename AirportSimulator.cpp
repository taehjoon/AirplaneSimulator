//#include "Queue.h"
#include "newQueue.h"
#define sizeofLQ 4
#define sizeofTQ 3	//num of takeoffQueue
#define numofRW 3	
static int sizeOfLPs = 0;	//num of LandingPlanes
static int sizeOfTPs = 0;	//num of waitingPlanes
static int localTime = 0;	//local time 
static int landing_takeoffTime = 0;	//plane's time
static int k1 = 0;	//top of runways....
static int k2 = 0;
static int k3 = 0;
const int lengthoflog = 20;		//RW's length
const int logableCapacity = lengthoflog * 3;
using namespace std;

struct randomInput {
	int timeUnit;
	int numPlanesTakeOff;
	int numPlanesLanding;
	int* remainingFlyTime;
	randomInput() {//constructure of randominput
		landing_takeoffTime += (rand() % 15) + 5;
		timeUnit = landing_takeoffTime;
		numPlanesLanding = (rand() % 10) + 3;
		numPlanesTakeOff = (rand() % 10) + 3;
		remainingFlyTime = new int[numPlanesLanding];
		for (int i = 0; i < numPlanesLanding; i++)
			remainingFlyTime[i] = (rand() % 50) + 10;
		sizeOfLPs += numPlanesLanding;
		sizeOfTPs += numPlanesTakeOff;
	}
};
ostream& operator<<(ostream& os, const randomInput& ri) {
	os << "time :" << ri.timeUnit << endl;
	os << "numof takeoff : " << ri.numPlanesTakeOff << endl;
	os << "numof landing : " << ri.numPlanesLanding << endl;
	os << "each fuel : ";
	for (int i = 0; i < ri.numPlanesLanding; i++)
		os << ri.remainingFlyTime[i] << " ";
	os << endl;
	return os;
}
struct LandingPlane {
	int arrivalTime;
	int IDofLandingPlane;
	int remainingFlyingTime;
};
ostream& operator<<(ostream& os, const LandingPlane& s) {
	os << "time : " << s.arrivalTime << endl;
	os << "ID : " << s.IDofLandingPlane << endl;
	os << "fuel : " << s.remainingFlyingTime << endl;
	return os;
}
struct TakeoffPlane {
	int takeoffTime;
	int IDofTakeoffPlane;
};
ostream& operator<<(ostream& os, const TakeoffPlane& s) {
	os << "time : " << s.takeoffTime << endl;
	os << "Id : " << s.IDofTakeoffPlane << endl;
	return os;
}
struct UseRunway {
	bool takeoff_landing;
	int IDPlane;
	int start;
	int end;
	UseRunway() {
		IDPlane = 0;
		start = localTime;
		end = 0;
		takeoff_landing = true;
	}
};
ostream& operator<<(ostream& os, const UseRunway& s) {
	os << "plane ID: " << s.IDPlane << endl;
	os << "start :" << s.start << " " << "end :" << s.end << endl;
	return os;
}
/*---------------------Structure---------------------------*/
LandingPlane generateLandingPlane(int time, int fuel) {
	LandingPlane newPlane;
	static int landingplaneID = 1;
	newPlane = { time,landingplaneID ,fuel };
	landingplaneID += 2;
	return newPlane;
}
TakeoffPlane generateTakeoffPlane(int time) {
	TakeoffPlane newPlane;
	static int takeoffplaneID = 2;
	newPlane = { time,takeoffplaneID };
	takeoffplaneID += 2;

	return newPlane;
}
//generate each plane's info
int generateInputData(LandingPlane* willLanding, TakeoffPlane* willFlying, randomInput* generate, int num) {
	int count1 = 0;
	int count2 = 0;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < generate[i].numPlanesLanding; j++) {
			willLanding[count1 + j] = generateLandingPlane(generate[i].timeUnit, generate[i].remainingFlyTime[j]);

		}
		for (int j = 0; j < generate[i].numPlanesTakeOff; j++) {
			willFlying[count2 + j] = generateTakeoffPlane(generate[i].timeUnit);
		}
		count1 += generate[i].numPlanesLanding;
		count2 += generate[i].numPlanesTakeOff;
	}
	return 0;
}
//generate index of LPs and TPs
/*--------about Queue----------*/
bool checkQueue(Queue<LandingPlane>* landingQueue) {
	for (int i = 0; i < sizeofLQ; i++) {
		if (landingQueue[i].getTop() != 2)// top is -1~2
			return true;
		else if (landingQueue[i].getTop() < -1 || landingQueue[i].getTop() > 2)	throw "checkQueue exception";
	}
	return false;
}
bool checkQueue(Queue<TakeoffPlane>* takeoffQueue) {
	for (int i = 0; i < sizeofTQ; i++) {
		if (takeoffQueue[i].getTop() != 2)// top is -1~2
			return true;
		else if (takeoffQueue[i].getTop() < -1 || takeoffQueue[i].getTop() > 2)	throw "checkQueue exception";
	}
	return false;
}
// if Queue has empty storage return true;
int findSmallLandingQueue(Queue<LandingPlane>* landingQueue) {	//we select queue smallest top;
	int selectedQueNum = 0;										//fuel >> time
	for (int i = 1; i < sizeofLQ; i++) {
		if (landingQueue[selectedQueNum].getTop() > landingQueue[i].getTop())	selectedQueNum = i;
	}
	if (landingQueue[selectedQueNum].getTop() == 2)	return -1;
	return selectedQueNum;
}
int findSmallTakeoffQueue(Queue<TakeoffPlane>* takeoffQueue) {
	int selectedQueNum = 0;										//fuel >> time
	for (int i = 1; i < sizeofTQ; i++) {
		if (takeoffQueue[selectedQueNum].getTop() > takeoffQueue[i].getTop())	selectedQueNum = i;
	}
	if (takeoffQueue[selectedQueNum].getTop() == 2)	return -1;
	return selectedQueNum;
}
// find smallest size of queue... non is return -1 else 0~Queuesize-1
int putInLandingQueue(Queue<LandingPlane>* landingQueue, LandingPlane& lp) {
	int queueNumber = -1;
	if (checkQueue(landingQueue)) {//0~3
		queueNumber = findSmallLandingQueue(landingQueue);
		if (queueNumber == -1) throw"exception";
		landingQueue[queueNumber].Push(lp);
	}
	return queueNumber;
}
int putInTakeoffQueue(Queue<TakeoffPlane>* takeoffQueue, TakeoffPlane& tp) {
	int queueNumber = -1;
	if (checkQueue(takeoffQueue)) {//0~2
		queueNumber = findSmallTakeoffQueue(takeoffQueue);
		if (queueNumber == -1) throw"exception";
		takeoffQueue[queueNumber].Push(tp);
	}
	return queueNumber;
}
// find smallest size of queue and put it in. non is return -1 else 0~queuesize-1
/*---------------find priority------------------*/
int findPriority(Queue<TakeoffPlane> takeoffQueue) {
	int index = -1;
	int smallestID = 10000000;
	for (int j = 0; j <= takeoffQueue.getTop(); j++) {
		if (takeoffQueue.getElement(j).IDofTakeoffPlane < smallestID) {
			smallestID = takeoffQueue.getElement(j).IDofTakeoffPlane;
			index = j;
		}
	}
	return index;
}
int findPriority(Queue<LandingPlane> landingQueue) {
	int index = -1;
	int smallestfuel = 10000000;
	int smallesttime = 10000000;
	for (int j = 0; j <= landingQueue.getTop(); j++) {			//first fuel, and time.
		if (landingQueue.getElement(j).remainingFlyingTime < smallestfuel) {
			smallestfuel = landingQueue.getElement(j).remainingFlyingTime;
			smallesttime = landingQueue.getElement(j).arrivalTime;
			index = j;
		}
		else if (landingQueue.getElement(j).remainingFlyingTime == smallestfuel) {
			if (landingQueue.getElement(j).arrivalTime < smallesttime) {
				smallesttime = landingQueue.getElement(j).arrivalTime;
				index = j;
			}
		}
	}
	return index;
}
//valid value is 0~2...
int* findTQPriority(Queue<TakeoffPlane>* takeoffQueue) {
	int eachQueue[sizeofTQ];	//the index of min values of each Queue 
	for (int i = 0; i < sizeofTQ; i++) {
		eachQueue[i] = findPriority(takeoffQueue[i]);
	}

	int standard = -1;
	int standardindex = -1;
	for (int i = 0; i < sizeofTQ; i++) {
		if (eachQueue[i]>=0 && eachQueue[i]<=2) {
			standard = eachQueue[i];
			standardindex = i;
			break;
		}
	}

	int* result = new int[2];
	if (standard == -1) {
		result[0] = -1;
		result[1] = -1;
		return result;
	}
	for (int i = 0; i < sizeofTQ; i++) {
		if (eachQueue[i]<0 || eachQueue[i]>2) continue;
		else if (takeoffQueue[standardindex].getElement(standard).IDofTakeoffPlane
	> takeoffQueue[i].getElement(eachQueue[i]).IDofTakeoffPlane) {
			standard = eachQueue[i];
			standardindex = i;
		}
	}
	result = new int[2];
	result[0] = standardindex;
	result[1] = standard;
	return result;
}
int* findLQPriority(Queue<LandingPlane>* landingQueue) {
	int eachQueue[sizeofLQ];	//the index of min values of each Queue 
	for (int i = 0; i < sizeofLQ; i++) {
		eachQueue[i] = findPriority(landingQueue[i]);
	}
	int standard = -1;
	int standardindex = -1;
	for (int i = 0; i < sizeofLQ; i++) {
		if (eachQueue[i]>=0 && eachQueue[i]<=2) {
			standard = eachQueue[i];
			standardindex = i;
			break;
		}
	}
	
	int* result = new int[2];
	if (standard == -1) {
		result[0] = -1;
		result[1] = -1;
		return result;
	}
	for (int i = 0; i < sizeofLQ; i++) {
		if (eachQueue[i] < 0 || eachQueue[i] > 2) continue;
		else if (landingQueue[standardindex].getElement(standard).remainingFlyingTime
	> landingQueue[i].getElement(eachQueue[i]).remainingFlyingTime) {
			standard = eachQueue[i];
			standardindex = i;
		}
		else if (landingQueue[standardindex].getElement(standard).remainingFlyingTime
			== landingQueue[i].getElement(eachQueue[i]).remainingFlyingTime) {
			if (landingQueue[standardindex].getElement(standard).arrivalTime
	> landingQueue[i].getElement(eachQueue[i]).arrivalTime) {
				standard = eachQueue[i];
				standardindex = i;
			}
		}
	}
	result[0] = standardindex;
	result[1] = standard;
	return result;
}
TakeoffPlane TQtoRW(Queue<TakeoffPlane>* takeoffQueue) {
	int* priority = findTQPriority(takeoffQueue);
	TakeoffPlane result;
	if (priority[0]<0 || priority[0]>=sizeofTQ) {
		result = { -1,-1 };
	}else{
		result = takeoffQueue[priority[0]].getElement(priority[1]);
		takeoffQueue[priority[0]].Delete(priority[1]);
	}
	//delete priority;
	return result;
}
LandingPlane LQtoRW(Queue<LandingPlane>* landingQueue) {
	int* priority = findLQPriority(landingQueue);
	LandingPlane result;
	if (priority[0] < 0 || priority[0]>=sizeofLQ) {
		result = { -1,-1, -1};
	}
	else {
		result = landingQueue[priority[0]].getElement(priority[1]);
		landingQueue[priority[0]].Delete(priority[1]);
	}
	//delete priority;
	return result;
}
//move to RW
void usingRW(UseRunway& runway, const TakeoffPlane& TP) {	
	runway.IDPlane = TP.IDofTakeoffPlane;
	runway.start = localTime;
	runway.end = localTime + 2;	//takeoff time....
	runway.takeoff_landing = false;
	cout << TP.IDofTakeoffPlane << "is takeoff!"<<endl;
	cout << "departure time : " << TP.takeoffTime << endl;
	cout << "start time :" << runway.start << "   end time : " << runway.end << endl;
}
void usingRW(UseRunway& runway, const LandingPlane& LP) {	
	runway.IDPlane = LP.IDofLandingPlane;
	runway.start = localTime;
	runway.end = localTime + 2;	///landing time...
	runway.takeoff_landing = false;
	cout << LP.IDofLandingPlane << "is land!" << endl;
	cout << "left fuel time : " << LP.remainingFlyingTime << endl;
	cout << "start time :" << runway.start << "   end time : " << runway.end << endl;
}
// log RW and logged data's bool is false
bool takeRW(UseRunway** runways,const TakeoffPlane& TP) {
	bool check = false;
	if (TP.IDofTakeoffPlane >= 1) {
		for(int i = 0; i < numofRW; i++) {
			int k = (i + 2) % 3;
			if (k == 2 && k3 < lengthoflog) {
				if (k3 == 0 || runways[k][k3 - 1].end < localTime) {
					usingRW(runways[k][k3], TP);
					k3++;
					check = true;
					cout << "using RW3" << endl;
					break;
				}
			}
			else if (k == 0 && k1 < lengthoflog) {
				if (k1 == 0 || runways[k][k1 - 1].end < localTime) {
					usingRW(runways[k][k1], TP);
					k1++;
					check = true;
					cout << "using RW1" << endl;
					break;
				}
			}
			else if (k == 1 && k2 < lengthoflog) {
				if (k2 == 0 || runways[k][k2 - 1].end < localTime) {
					usingRW(runways[k][k2], TP);
					k2++;
					check = true;
					cout << "using RW2" << endl;
					break;
				}
			}
		}
	}
	return check;
}
bool takeRW(UseRunway** runways,LandingPlane LP) {
	bool check = false;
	if (LP.IDofLandingPlane >= 1) {
		for (int i = 0; i < numofRW; i++) {
			if (i == 0 && k1 < lengthoflog) {
				if (k1 == 0 || runways[i][k1 - 1].end < localTime) {
					usingRW(runways[i][k1], LP);
					k1++;
					check = true;
					cout << "using RW1" << endl;
					break;
				}
			}
			else if (i == 1 && k2 < lengthoflog) {
				if (k2 == 0 || runways[i][k2 - 1].end < localTime) {
					usingRW(runways[i][k2], LP);
					k2++;
					check = true;
					cout << "using RW2" << endl;
					break;
				}
			}
			else if (i == 2 && k3 < lengthoflog) {
				if (k3 == 0 || runways[i][k3 - 1].end < localTime) {
					usingRW(runways[i][k3], LP);
					k3++;
					check = true;
					cout << "using RW3" << endl;
					break;
				}
			}
		}
	}
	
	return check;
}
void ControlStation(UseRunway** runways, Queue<TakeoffPlane>* takeoffQueue, Queue<LandingPlane>* landingQueue) {
	if ((k1 + k2 + k3) < logableCapacity) {
		int* priority = findLQPriority(landingQueue);
		if (priority[0] >= 0 && priority[0] < sizeofLQ) {
			LandingPlane one = landingQueue[priority[0]].getElement(priority[1]);
			if (one.remainingFlyingTime <= 5 || one.arrivalTime <= localTime) {
				if(takeRW(runways, one))
					LQtoRW(landingQueue);
			}
		}
		
		priority = findTQPriority(takeoffQueue);
		if (priority[0] >= 0 && priority[0] < sizeofTQ) {
			TakeoffPlane two = takeoffQueue[priority[0]].getElement(priority[1]);
			if (two.takeoffTime <= localTime) {
				if(takeRW(runways, two))
					TQtoRW(takeoffQueue);
			}
		}
	}
	else { cout << "can't log RW!!"; }
}


int main() {
	Queue<LandingPlane>* LQ = new Queue<LandingPlane>[sizeofLQ];
	Queue<TakeoffPlane>* TQ = new Queue<TakeoffPlane>[sizeofTQ];
	int num;
	cin >> num;
	randomInput* generate = new randomInput[num]();
	LandingPlane* flyList = new LandingPlane[sizeOfLPs];
	TakeoffPlane* waitList = new TakeoffPlane[sizeOfTPs];
	UseRunway** runways = new UseRunway * [3];
	for (int i = 0; i < 3; i++)
		runways[i] = new UseRunway[lengthoflog];

	for (int i = 0; i < num; i++)
		cout << generate[i];
	int countLPs=0;
	int countTPs=0;
	generateInputData(flyList, waitList, generate, num);
	//start loop
	
	while (localTime <= 100) {
		int time1 = clock();
		int time2 = clock();
		while ((time2 - time1) <= 100) { time2 = clock(); }//0.1sec
		cout << "*================================*"<<endl;
		cout << "Time is : " << localTime<<endl;
		cout << "*================================*" << endl;
		
		while (checkQueue(LQ) && countLPs < sizeOfLPs) {
			putInLandingQueue(LQ, flyList[countLPs]);
			countLPs++;
		}
		while (checkQueue(TQ) && countTPs < sizeOfTPs) {
			putInTakeoffQueue(TQ, waitList[countTPs]);
			countTPs++;
		}

		ControlStation(runways, TQ, LQ);
		time1 = time2;
		localTime++;
		
		for (int i = 0; i < sizeofLQ; i++)
			for (int j = 0; j <= LQ[i].getTop(); j++) // decresing fuel time.
				LQ[i].setElement(j, { LQ[i].getElement(j).arrivalTime,LQ[i].getElement(j).IDofLandingPlane 
					,LQ[i].getElement(j).remainingFlyingTime - 1 });
	
	}
	cout << "runway 1: " << k1 << "  runway 2:" << k2 << "  runway 3: " << k3<<endl;
	cout << "R1" << endl;
	for (int i = 0; i < k1; i++)
		cout << runways[0][i]<<endl;
	cout << "R2" << endl;
	for (int i = 0; i < k2; i++)
		cout << runways[1][i] << endl;
	cout << "R3" << endl;
	for (int i = 0; i < k3; i++)
		cout << runways[2][i] << endl;
		
	return 0;
}