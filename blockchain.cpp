#include "blockchain.h"
#include "picosha2.h"
using namespace std;

Blockchain::Blockchain(){
    head=NULL;
}

Transaction* Blockchain::getHead() {
    return this->head;
}

void Blockchain::addTransaction(int amount, string sender, string receiver) {
    Transaction* t1 = new Transaction();
    t1->setAmount(amount);
    t1->setSender(sender);
    t1->setReceiver(receiver);
    t1->setNonce(findNonce(amount,sender,receiver));
    if(head!=NULL) {
        t1->setHash(hashCode(to_string(head->getAmount())+head->getSender()+head->getReceiver()+head->getNonce()));
    }
    t1->setNext(head);
    head = t1;
    t1->printTransaction();
}

void Blockchain::findTransaction(string sender) {
    cout<<"\nHere are all the transactions from " + sender +":"<<endl;
    Transaction* tmp = this->head;
    while(tmp){
      if(tmp->getSender() == sender){
        cout<<"\n"<<endl;
        tmp->printTransaction();
      }
      tmp = tmp->getNext();
    }
}

bool Blockchain::verifyAndPrint() {
    Transaction* tmp = head;
    Transaction* print = head;
    while(tmp) {
        if(tmp->getNext() != NULL){
            if(tmp->getHash() != hashCode(to_string(tmp->getNext()->getAmount())+tmp->getNext()->getSender() +tmp->getNext()->getReceiver()+tmp->getNext()->getNonce())) {
                cout<<"Found an error, here are the mismatch transactions:\n"<<endl;
                tmp->printTransaction();
                tmp->getNext()->printTransaction();
                return false;
            }
        }
        tmp=tmp->getNext();
    }

    cout<<"All values match the hashes."<<endl;
    cout<<"\nHere is the full chain: \n";

    while(print) {
        print->printTransaction();
        cout<<"\n";
        print=print->getNext();
    }
    return true;
}

string Blockchain::findNonce(int amount, string sender, string receiver) {
    string nonce;
    srand(time(NULL));
    nonce = char(rand() % 26 + 97);
    nonce += char(rand() % 26 + 97);
    string full = to_string(amount) + sender + receiver + nonce;
    string code = hashCode(full);
    while(isalpha(code[code.length()-1]) || (code[code.length()-1] - '0' > 4 )) {
        nonce = char(rand() % 26 + 97);
        nonce += char(rand() % 26 + 97);
        full = to_string(amount) + sender + receiver + nonce;
        code = hashCode(full);
    }
    return nonce;
}

string Blockchain::hashCode(string transaction) {
    string hash;
    picosha2::hash256_hex_string(transaction,hash);
    return hash;
}
