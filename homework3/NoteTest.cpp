#include <iostream>
#include <fstream>
#include <stdexcept>
#include "note.h"

using namespace std;
using namespace xml;
int main()
{
    try {
        note_type note;
        note.to = "Students";
        note.from = "Mike";
        note.cc = "Nithin";
        note.heading = "XML Data Binding Program";
        note.body = R"(
    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque fermentum augue in sem
    suscipit, et posuere ligula sollicitudin. Nullam venenatis ex ac bibendum egestas. 
    Interdum et malesuada fames ac ante ipsum primis in faucibus. Nullam et lacus porta, 
    posuere lacus aliquam, auctor felis. Etiam vitae nibh sagittis, ultrices magna nec, 
    faucibus urna. Morbi at quam sed dolor egestas luctus sed at dui. Fusce turpis elit, 
    tristique at finibus luctus, fermentum in nisi. Vestibulum dignissim nunc mauris, 
    nec vestibulum sapien porttitor eget. Maecenas varius tempor elit. 
    Sed nec lacinia nunc. In bibendum sagittis posuere. Sed ipsum nisi, 
    congue nec metus ut, bibendum accumsan mauris.
  )";
        note.count = 100;
        note.fraction = .56789;
        note.date_today = date("2020-06-04");
        cout << toXML(note) << endl;
    } catch (std::exception &e) {
        cout << e.what();
    }
}

