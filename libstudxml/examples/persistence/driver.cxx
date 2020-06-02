// file      : examples/persistence/driver.cxx
// copyright : not copyrighted - public domain

#include <fstream>
#include <iostream>

#include <xml/parser>
#include <xml/serializer>

#include "position.hxx"

using namespace std;

int
main (int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "usage: " << argv[0] << " <xml-file>" << endl;
    return 1;
  }

  try
  {
    // Load the object model state from XML.
    //
    ifstream ifs (argv[1]);
    xml::parser p (ifs, argv[1]);

    object o (p);

    // Save the object model state back to XML.
    //
    xml::serializer s (cout, "output");

    o.serialize (s);
  }
  // This handler will handle both parsing (xml::parsing) and serialization
  // (xml::serialization) exceptions.
  //
  catch (const xml::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}
