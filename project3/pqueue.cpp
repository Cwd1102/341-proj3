// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{
    m_priorFunc = priFn;
	m_heapType = heapType;
	m_structure = structure;
	m_size = 0;
	m_heap = nullptr;
}

PQueue::~PQueue()
{
    clear();
}
void PQueue::clear()
{
    //delete all nodes in the heap
    while (m_heap != nullptr) {
        Node* temp = m_heap;
        m_heap = merge(m_heap->m_left, m_heap->m_right);
        delete temp;
        m_size--;
    }
    //re-initialize member variables
    m_priorFunc = nullptr;
    m_heapType = MINHEAP;
    m_structure = SKEW;
}


PQueue::PQueue(const PQueue& rhs){
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_size = rhs.m_size;
    m_heap = nullptr;
    if(rhs.m_heap != nullptr){
        m_heap = new Node(*rhs.m_heap);
    }
}

PQueue& PQueue::operator=(const PQueue& rhs) {
    return *this;
}
void PQueue::mergeWithQueue(PQueue& rhs) {
    if (this == &rhs) {
        throw domain_error("Cannot merge a heap with itself.");
    }
    if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure) {
        throw domain_error("Cannot merge heaps with different priority functions or data structures.");
    }
    if (rhs.m_size == 0) {
        return;
    }
    //merge the heaps
    m_heap = merge(m_heap, rhs.m_heap);
    m_size += rhs.m_size;
    rhs.m_heap = nullptr;
    rhs.m_size = 0;
}


void PQueue::insertPatient(const Patient& patient) {
    m_heap = merge(m_heap, new Node(patient));
    m_size++;
}

int PQueue::numPatients() const
{
    return m_size;
}

prifn_t PQueue::getPriorityFn() const {
    return m_priorFunc;
}

Patient PQueue::getNextPatient() {
    if (m_heap == nullptr) {
        throw out_of_range("Queue is empty.");
    }
    Patient highestPriorityPatient = m_heap->m_patient;
    Node* leftSubtree = m_heap->m_left;
    Node* rightSubtree = m_heap->m_right;
    delete m_heap;
    m_heap = merge(leftSubtree, rightSubtree);
    m_size--;
    return highestPriorityPatient;
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_priorFunc = priFn;
    m_heapType = heapType;
    if (m_size > 1) {
        Node* temp = m_heap;
        m_heap = nullptr;
        m_heap = merge(temp->m_left, temp->m_right);
        delete temp;
    }
}


void PQueue::setStructure(STRUCTURE structure) {
    m_structure = structure;
    if (m_size > 1) {
		Node* temp = m_heap;
		m_heap = nullptr;
		m_heap = merge(temp->m_left, temp->m_right);
		delete temp;
	}
}

STRUCTURE PQueue::getStructure() const {
    return m_structure;
}

HEAPTYPE PQueue::getHeapType() const {
    return m_heapType;
}

void PQueue::printPatientQueue() const {
    if (m_heap == nullptr) {
        cout << "Empty queue." << endl;
        return;
    }
    cout << "conents of the queue:" << endl;
    printPatientQueue(m_heap);
}

void PQueue::dump() const {
    if (m_size == 0) {
        cout << "Empty heap.\n";
    }
    else {
        dump(m_heap);
    }
    cout << endl;
}
void PQueue::dump(Node* pos) const {
    if (pos != nullptr) {
        cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW)
            cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
        else
            cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
    sout << patient.getPatient()
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
    return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
    sout << node.getPatient();
    return sout;
}

//********************************************************************************
//**Helper Functions**
//********************************************************************************


//
Node* PQueue::merge(Node* h1, Node* h2) {
    //base cases
    if (h1 == nullptr) {
        return h2;
    }
    if (h2 == nullptr) {
        return h1;
    }
    //recursive cases
    //if the heap is a skew heap
    if(m_structure == SKEW){ 
    	return mergeSkew(h1, h2);
    }
    //if the heap is a leftist heap
    else if (m_structure == LEFTIST) {
        //if the heap is a min heap
        if (m_heapType == MINHEAP) {
            if (m_priorFunc(h1->m_patient) < m_priorFunc(h2->m_patient)) {
                return mergeLeftist(h1, h2);
            }
            //if the heap is a max heap
            else {
                return mergeLeftist(h2, h1);
            }
        }
        else {
            if (m_priorFunc(h1->m_patient) > m_priorFunc(h2->m_patient)) {
                return mergeLeftist(h1, h2);
            }
            //if the heap is a max heap
            else {
                return mergeLeftist(h2, h1);
            }
        }
    }


}

Node* PQueue::mergeSkew(Node* h1, Node* h2) {
    Node* temp = nullptr;

    if (m_heapType == MAXHEAP) {
        if (m_priorFunc(h1->m_patient) < m_priorFunc(h2->m_patient)) {
            temp = h1;
            h1 = h2;
            h2 = temp;
        }
        swapChildren(h1);
        h1->m_left = merge(h2, h1->m_left);
        return h1;
    }
    else if (m_heapType == MINHEAP) {
        if (m_priorFunc(h1->m_patient) > m_priorFunc(h2->m_patient)) {
			temp = h1;
			h1 = h2;
			h2 = temp;
		}
		swapChildren(h1);
		h1->m_left = merge(h2, h1->m_left);
		return h1;
	}
}


Node* PQueue::mergeLeftist(Node* h1, Node* h2) {
    Node* temp = nullptr;
    //base case
    if (m_heapType == MINHEAP) {
        if (h1->m_left == nullptr) {
            h1->m_left = h2;
		}
		else {
            //recursive case
			h1->m_right = merge(h1->m_right, h2);
            //if the left child has a null path length less than the right child
            if (h1->m_left->m_npl < h1->m_right->m_npl) {
                swapChildren(h1);
			}
			h1->m_npl = min(h1->m_left->m_npl, h1->m_right->m_npl) + 1;
		}
        return h1;
    }
    else if (m_heapType == MAXHEAP) {
        if (h1->m_left == nullptr) {
            h1->m_left = h2;
        }
        else {
            h1->m_right = merge(h1->m_right, h2);
            //if the left child has a null path length greater than the right child
            if (h1->m_left->m_npl < h1->m_right->m_npl) {
                swapChildren(h1);
            }
            h1->m_npl = min(h1->m_left->m_npl, h1->m_right->m_npl) + 1;
        }
        return h1;
    }
}

void PQueue::swapChildren(Node* h1) {
    Node* temp = h1->m_right;
	h1->m_right = h1->m_left;
	h1->m_left = temp;
}

void PQueue::printPatientQueue(Node* pos) const {
    if (pos != nullptr) {
        cout << "[" << m_priorFunc(pos->m_patient) << "] " << pos->m_patient << endl;
        printPatientQueue(pos->m_left);
        printPatientQueue(pos->m_right);
    }
}

bool PQueue::checkNPL() {
	return checkNPL(m_heap);
}

bool PQueue::checkNPL(Node* pos){
    int leftNPL = 0;
    int rightNPL = 0;

    if (pos == nullptr) {
		return true;
	}
    if (pos->m_left == nullptr && pos->m_right == nullptr) {
		return true;
	}
    if (pos->m_left != nullptr) {
		leftNPL = pos->m_left->m_npl;
	}
    if (pos->m_right != nullptr) {
		rightNPL = pos->m_right->m_npl;
	}
    if (leftNPL < rightNPL) {
		return false;
	}
	return checkNPL(pos->m_left) && checkNPL(pos->m_right);
}

bool PQueue::checkRootNum() {
	return checkRootNum(m_heap, m_priorFunc(m_heap->getPatient()));
}

bool PQueue::checkRootNum(Node* pos, int num) {
    if (pos == nullptr) {
        return true;
    }
    if (m_heapType == MINHEAP) {
        if (m_priorFunc(pos->m_patient) < num) {
			return false;
		}
	}
    else if (m_heapType == MAXHEAP) {
        if (m_priorFunc(pos->m_patient) > num) {
			return false;
		}
	}
 
    return checkRootNum(pos->m_left, num) && checkRootNum(pos->m_right, num);

}