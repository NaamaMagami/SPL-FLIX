#include <iostream>
#include "../include/Session.h"
using namespace std;

int main(int argc, char** argv){
    if(argc!=2)
    {
        cout << "usage splflix input_file" << endl;
        return 0;
    }
    Session* s1 = new Session(argv[1]); //const
    s1->start();
    Session* s2 = new Session(argv[1]);
    s2->start();
    *s2 = std::move(*s1); //move assignment
    delete(s1);
    s2->start();
    Session* s3(new Session(argv[1])); //move const
    s3->start();
    *s2 = *s3; //operator=
    s2->start();
    delete (s3);
    Session* s4 = new Session(argv[1]);
    s4->start();
    Session* s5 = s4;
    *s5 = std::move(*s4); //self assignment
    s5->start();
    *s2 = * s4; //operator=
    delete(s4);
    s2->start();
    delete(s2);
    return 0;
}
