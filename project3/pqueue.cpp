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

}
void PQueue::clear()
{

}

PQueue::PQueue(const PQueue& rhs){

}

PQueue& PQueue::operator=(const PQueue& rhs) {

}
void PQueue::mergeWithQueue(PQueue& rhs) {

}

void PQueue::insertPatient(const Patient& patient) {
    m_heap = merge(m_heap, new Node(patient));
}

int PQueue::numPatients() const
{
    return m_size;
}

prifn_t PQueue::getPriorityFn() const {

}

Patient PQueue::getNextPatient() {

}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
    m_priorFunc = priFn;
    m_heapType = heapType;
}

void PQueue::setStructure(STRUCTURE structure) {
    m_structure = structure;
}

STRUCTURE PQueue::getStructure() const {
    return m_structure;
}

HEAPTYPE PQueue::getHeapType() const {
    return m_heapType;
}

void PQueue::printPatientQueue() const {

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

void PQueue::merge(Node& h1) {

    m_heap = merge(m_heap, &h1);
}

Node* PQueue::merge(Node* h1, Node* h2) {
    Node * temp = nullptr;

    if(h1 == nullptr)
		return h2;
    if(h2 == nullptr)
        return h1;
    //check if the heap is a minheap or maxheap
    if (m_heapType == MINHEAP) {
        //check if the heap is a skew or leftist
        if (m_structure == SKEW) {
            //check if the priority of h1 is greater than h2
            //if it is, swap the two
            if (m_priorFunc(h1->m_patient) < m_priorFunc(h2->m_patient)) {
				temp = h1;
				h1 = h2;
				h2 = temp;
            }
            temp = h1->m_left;
            h1->m_left = h1->m_right;
            h1->m_right = temp;
            h1->m_left = merge(h2,h1->m_left);
				
		}
        else if (m_structure == LEFTIST) {

                    
        }

    }
    else if (m_heapType == MAXHEAP) {

	}
	return h1;
}


void PQueue::swapChildren(Node* h1) {
    Node* temp = h1->m_right;
	h1->m_right = h1->m_left;
	h1->m_left = temp;
}