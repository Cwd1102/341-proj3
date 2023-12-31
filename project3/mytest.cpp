// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

const string M_FAIL = "-----FAILED------";
const string M_PASS = "PASSED";

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient& patient);
int priorityFn2(const Patient& patient);

// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green",
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong",
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// We can use the Random class to generate the test data randomly!
enum RANDOM { UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE };
class Random {
public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL) {
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum) {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int>& array) {
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i <= m_max; i++) {
            array.push_back(i);
        }
        shuffle(array.begin(), array.end(), m_generator);
    }

    void getShuffle(int array[]) {
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i <= m_max; i++) {
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it = temp.begin(); it != temp.end(); it++) {
            array[i] = *it;
            i++;
        }
    }

    int getRandNum() {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL) {
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT) {
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum() {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

int priorityFn1(const Patient& patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient& patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}

class Tester {
public:

    bool testInsertMinNormal();
    bool testInsertMaxNormal();
    bool testMinRemove();
    bool testMaxRemove();
    bool testLeftistHeap();
    bool testStructChange();
    bool testPriorityChange();
    bool testMerge();
    bool testCopyNormal();
    bool testCopyEdge();
    bool testAssignmentNormal();
    bool testAssignmentEdge();
    bool testDequeue();
    bool testMergeError();

};

int main() {
    Tester test;
    if(test.testInsertMinNormal())
		cout << "testInsertMinNormal " << M_PASS << endl;
	else
		cout << "testInsertMinNormal " << M_FAIL << endl;

    if (test.testInsertMaxNormal()) {
        cout << "testInsertMaxNormal " << M_PASS << endl;
    }
    else {
		cout << "testInsertMaxNormal " << M_FAIL << endl;
	}

    if (test.testMinRemove()) {
		cout << "testMinRemove " << M_PASS << endl;
	}
    else {
		cout << "testMinRemove " << M_FAIL << endl;
	}

    if (test.testMaxRemove()) {
		cout << "testMaxRemove " << M_PASS << endl;
	}
    else {
		cout << "testMaxRemove " << M_FAIL << endl;
	}

    if (test.testLeftistHeap()) {
		cout << "testLeftistHeap " << M_PASS << endl;
	}
    else {
		cout << "testLeftistHeap " << M_FAIL << endl;
	}

    if (test.testStructChange()) {
		cout << "testStructChange " << M_PASS << endl;
	}
    else {
		cout << "testStructChange " << M_FAIL << endl;
	}

    if (test.testPriorityChange()) {
		cout << "testPriorityChange " << M_PASS << endl;
	}
    else {
		cout << "testPriorityChange " << M_FAIL << endl;
	}

    if (test.testMerge()) {
        cout << "testMerge " << M_PASS << endl;
    }
    else {
		cout << "testMerge " << M_FAIL << endl;
	}

    if (test.testCopyNormal()) {
		cout << "testCopyNormal " << M_PASS << endl;
	}
    else {
		cout << "testCopyNormal " << M_FAIL << endl;
	}

    if (test.testCopyEdge()) {
		cout << "testCopyEdge " << M_PASS << endl;
	}
    else {
		cout << "testCopyEdge " << M_FAIL << endl;
	}

    if (test.testAssignmentNormal()) {
		cout << "testAssignmentNormal " << M_PASS << endl;
	}
    else {
		cout << "testAssignmentNormal " << M_FAIL << endl;
	}

    if (test.testAssignmentEdge()) {
		cout << "testAssignmentEdge " << M_PASS << endl;
	}
    else {
		cout << "testAssignmentEdge " << M_FAIL << endl;
	}

    if (test.testDequeue()) {
		cout << "testDequeue " << M_PASS << endl;
	}
    else {
		cout << "testDequeue " << M_FAIL << endl;
	}

    if (test.testMergeError()) {
		cout << "testMergeError " << M_PASS << endl;
	}
    else {
		cout << "testMergeError " << M_FAIL << endl;
	}

	return 0;

}

//tests insert function for min heap and normal priority function



bool Tester::testInsertMinNormal() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);

    PQueue test(priorityFn2, MINHEAP, SKEW);
    //patient with highest priority
    //temp is 35, oxygen is 70, respiratory is 10, blood pressure is 70, nurse opinion is 1
    Patient temp("Ismail Carter", 35, 70, 10, 70, 1);
    test.insertPatient(temp);

    for (int i = 0; i < 499; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }

    //check if the first patient is the highest priority
    if (test.getNextPatient().getPatient() != "Ismail Carter") {
        return false;
    }
    

    //check if the size is correct
    if (test.numPatients() != 499) {
        return false;
    }

    PQueue test2(priorityFn2, MINHEAP, LEFTIST);
    Patient temp2("Ismail Carter", 35, 70, 10, 70, 1);
    test2.insertPatient(temp2);

    for (int i = 0; i < 499; i++) {
		Patient temp(nameDB[nameGen.getRandNum()],
            			temperatureGen.getRandNum(),
            			oxygenGen.getRandNum(),
            			respiratoryGen.getRandNum(),
            			bloodPressureGen.getRandNum(),
            			nurseOpinionGen.getRandNum());
		test2.insertPatient(temp);
	}


    if (!test2.checkNPL()) {
		return false;
	}

    //check if the first patient is the highest priority
    if (test2.getNextPatient().getPatient() != "Ismail Carter") {
		return false;
	}

    //check if the size is correct
    if (test2.numPatients() != 499) {
        return false;
    }



    return true;
}

bool Tester::testInsertMaxNormal() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);

    PQueue test(priorityFn1, MAXHEAP, SKEW);
    //patient with highest priority
    //temp is 42, oxygen is 40, respiratory is 40, blood pressure is 160, nurse opinion is 10
    Patient temp("Ismail Carter", 42, 101, 40, 160, 10);
    test.insertPatient(temp);

    for (int i = 0; i < 499; i++) {
		Patient temp(nameDB[nameGen.getRandNum()],
            			temperatureGen.getRandNum(),
            			oxygenGen.getRandNum(),
            			respiratoryGen.getRandNum(),
            			bloodPressureGen.getRandNum(),
            			nurseOpinionGen.getRandNum());
		test.insertPatient(temp);
	}
    //check if the first patient is the highest priority
    if (test.getNextPatient().getPatient() != "Ismail Carter") {
        return false;
    }

    //check if the size is correct
    if (test.numPatients() != 499) {
		return false;
	}

    PQueue test2(priorityFn1, MAXHEAP, LEFTIST);
    Patient temp2("Ismail Carter", 42, 101, 40, 160, 10);
	test2.insertPatient(temp2);

    for (int i = 0; i < 499; i++) {
		Patient temp(nameDB[nameGen.getRandNum()],
            						temperatureGen.getRandNum(),
            						oxygenGen.getRandNum(),
            						respiratoryGen.getRandNum(),
            						bloodPressureGen.getRandNum(),
            						nurseOpinionGen.getRandNum());
		test2.insertPatient(temp);
	}

    //test2.dump();

    if (!test2.checkNPL()) {
        return false;
    }

	//check if the first patient is the highest priority
    if (!test2.checkRootNum()) {
		return false;
	}

	//check if the size is correct
    if (test2.numPatients() != 500) {
		return false;
	}

	return true;
}

bool Tester::testMinRemove() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);

    PQueue test(priorityFn1, MINHEAP, LEFTIST);


    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }


    for (int i = 0; i < 10; i++) {
		test.getNextPatient();
        if (test.checkNPL() == false) {
			return false;
		}
	}
    if (test.checkRootNum() == false) {
        return false;
    }

    return true;
}

bool Tester::testMaxRemove() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);

    PQueue test(priorityFn1, MAXHEAP, LEFTIST);


    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }


    for (int i = 0; i < 10; i++) {
        test.getNextPatient();
        if (test.checkNPL() == false) {
            return false;
        }
    }
    if (test.checkRootNum() == false) {
        return false;
    }

}

bool Tester::testLeftistHeap() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);
    PQueue test(priorityFn1, MAXHEAP, LEFTIST);

    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }

    if (!test.checkNPL()) {
		return false;
	}

    return true;

}

bool Tester::testStructChange() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);
    PQueue test(priorityFn1, MAXHEAP, LEFTIST);
    int size = 0;
    int root = 0;

    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }
    
    root = test.m_priorFunc(test.m_heap->getPatient());
    size = test.m_size;
    test.setStructure(SKEW);

    if (test.m_size != size) {
		return false;
	}

    if (!test.checkRootNum()) {
        return false;
    }

    if (test.m_priorFunc(test.m_heap->getPatient()) != root) {
        return false;
    }

    return true;
}

bool Tester::testPriorityChange() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);
    PQueue test(priorityFn1, MAXHEAP, LEFTIST);
    int values[15]{ 0 };
    int size = 0;
    int root = 0;
    int patientNum = 0;


    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
        values[i] = test.m_priorFunc(temp);
    }

    size = test.m_size;
    test.setPriorityFn(priorityFn2, MINHEAP);
    root = test.m_priorFunc(test.m_heap->getPatient());
    size = test.m_size;

    for (int i = 0; i < 15; i++) {
        if (root > values[i])
            return false;
    }

    return true;

}

bool Tester::testMerge() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);
    PQueue test(priorityFn1, MAXHEAP, LEFTIST);
    PQueue test2(priorityFn1, MAXHEAP, LEFTIST);

    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }

    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test2.insertPatient(temp);
    }

    test.mergeWithQueue(test2);

    if (test.m_size != 30) {
		return false;
	}
}

bool Tester::testCopyNormal() {
	Random nameGen(0, NUMNAMES - 1);
	Random temperatureGen(MINTEMP, MAXTEMP);
	Random oxygenGen(MINOX, MAXOX);
	Random respiratoryGen(MINRR, MAXRR);
	Random bloodPressureGen(MINBP, MAXBP);
	Random nurseOpinionGen(MINOPINION, MAXOPINION);
	PQueue test(priorityFn1, MAXHEAP, LEFTIST);

    for (int i = 0; i < 15; i++) {
		Patient temp(nameDB[nameGen.getRandNum()],
            			temperatureGen.getRandNum(),
            			oxygenGen.getRandNum(),
            			respiratoryGen.getRandNum(),
            			bloodPressureGen.getRandNum(),
            			nurseOpinionGen.getRandNum());
		test.insertPatient(temp);
	}

    PQueue test2(test);

    if (test2.m_priorFunc(test2.m_heap->getPatient()) != test.m_priorFunc(test.m_heap->getPatient())) {
        return false;
    }

    return true;

}

bool Tester::testCopyEdge() {
	Random nameGen(0, NUMNAMES - 1);
	Random temperatureGen(MINTEMP, MAXTEMP);
	Random oxygenGen(MINOX, MAXOX);
	Random respiratoryGen(MINRR, MAXRR);
	Random bloodPressureGen(MINBP, MAXBP);
	Random nurseOpinionGen(MINOPINION, MAXOPINION);
	PQueue test(priorityFn1, MAXHEAP, LEFTIST);

    for (int i = 0; i < 15; i++) {
		Patient temp(nameDB[nameGen.getRandNum()],
            			temperatureGen.getRandNum(),
            			oxygenGen.getRandNum(),
            			respiratoryGen.getRandNum(),
            			bloodPressureGen.getRandNum(),
            			nurseOpinionGen.getRandNum());
		test.insertPatient(temp);
	}

	PQueue test2(test);

    if (test2.m_priorFunc(test2.m_heap->getPatient()) != test.m_priorFunc(test.m_heap->getPatient())) {
		return false;
	}

	return true;
}

bool Tester::testAssignmentNormal() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);
    PQueue test(priorityFn1, MAXHEAP, LEFTIST);

    for (int i = 0; i < 15; i++) {
        Patient temp(nameDB[nameGen.getRandNum()],
            temperatureGen.getRandNum(),
            oxygenGen.getRandNum(),
            respiratoryGen.getRandNum(),
            bloodPressureGen.getRandNum(),
            nurseOpinionGen.getRandNum());
        test.insertPatient(temp);
    }

    PQueue test2 = test;

    if (test2.m_priorFunc(test2.m_heap->getPatient()) != test.m_priorFunc(test.m_heap->getPatient())) {
		return false;
	}

    return true;
}

bool Tester::testAssignmentEdge() {
	Random nameGen(0, NUMNAMES - 1);
	Random temperatureGen(MINTEMP, MAXTEMP);
	Random oxygenGen(MINOX, MAXOX);
	Random respiratoryGen(MINRR, MAXRR);
	Random bloodPressureGen(MINBP, MAXBP);
	Random nurseOpinionGen(MINOPINION, MAXOPINION);
	PQueue test(priorityFn1, MAXHEAP, LEFTIST);

    for (int i = 0; i < 15; i++) {
		Patient temp(nameDB[nameGen.getRandNum()],
            			temperatureGen.getRandNum(),
            			oxygenGen.getRandNum(),
            			respiratoryGen.getRandNum(),
            			bloodPressureGen.getRandNum(),
            			nurseOpinionGen.getRandNum());
		test.insertPatient(temp);
	}

	PQueue test2 = test;

    if (test2.m_priorFunc(test2.m_heap->getPatient()) != test.m_priorFunc(test.m_heap->getPatient())) {
		return false;
	}

	return true;
}

bool Tester::testDequeue() {
	Random nameGen(0, NUMNAMES - 1);
	Random temperatureGen(MINTEMP, MAXTEMP);
	Random oxygenGen(MINOX, MAXOX);
	Random respiratoryGen(MINRR, MAXRR);
	Random bloodPressureGen(MINBP, MAXBP);
	Random nurseOpinionGen(MINOPINION, MAXOPINION);

	PQueue test(priorityFn1, MAXHEAP, LEFTIST);

    try {
		test.getNextPatient();
	}
	catch (std::exception& e) {
		return true;
}
	return false;
}

bool Tester::testMergeError() {
    Random nameGen(0, NUMNAMES - 1);
    Random temperatureGen(MINTEMP, MAXTEMP);
    Random oxygenGen(MINOX, MAXOX);
    Random respiratoryGen(MINRR, MAXRR);
    Random bloodPressureGen(MINBP, MAXBP);
    Random nurseOpinionGen(MINOPINION, MAXOPINION);

    PQueue test(priorityFn1, MAXHEAP, LEFTIST);
    PQueue test2(priorityFn2, MINHEAP, LEFTIST);

    try {
		test.mergeWithQueue(test2);
	}
    catch (std::exception& e) {
		return true;
	}

    return false;

}



