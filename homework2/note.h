// Generated XML Schema Binding file
// UChicago MPCS51045

#include <xml/parser>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <optional>

struct date {
    int year;
    int month;
    int day;
    date() = default;
    date(std::string const& date) {
        year = std::stoi(date.substr(0, 4));
        month = std::stoi(date.substr(5, 2));
        day = std::stoi(date.substr(8, 2));
    }
};

template<typename T>
T fromXML(xml::parser &p, std::string name = "", bool alreadyInElement = false);
// skip over whitespace in XML file
void munchSpace(xml::parser &p)
{
    while(p.peek() == p.characters) {
        p.next();
        auto s = p.value();
        if(!std::all_of(s.begin(),s.end(),std::isspace))
            throw std::runtime_error("Unexpected characters: " + s);
    }
}
struct note_type {
    std::string to;
    std::optional<std::string> cc;
    std::string from;
    std::string heading;
    std::string body;
    int count;
    double fraction;
    date date_today;
};

template<>
note_type fromXML<note_type>(xml::parser &p, std::string name, bool alreadyInElement);
template<>
date fromXML<date>(xml::parser &p, std::string name, bool alreadyInElement);
template<>
double fromXML<double>(xml::parser &p, std::string name, bool alreadyInElement);
template<>
int fromXML<int>(xml::parser &p, std::string name, bool alreadyInElement);
template<>
std::string fromXML<std::string>(xml::parser &p, std::string name, bool alreadyInElement);


template<>
note_type fromXML<note_type>(xml::parser &p, std::string name, bool alreadyInElement) {
    if(name.empty()) name = "note";
    note_type result;
    if(!alreadyInElement)
        p.next_expect(xml::parser::start_element);
    alreadyInElement = false;
    if(p.name() != name)
        throw std::runtime_error("expected " + name + ". Got " + p.name());
    if(!alreadyInElement) munchSpace(p);
    result.to = fromXML<std::string>(p, "to", alreadyInElement);
    alreadyInElement = false;

    if(!alreadyInElement) munchSpace(p);
    if(!alreadyInElement)
        p.next_expect(xml::parser::start_element);
    alreadyInElement = true;
    if(p.name() == "cc") {
        result.cc = fromXML<std::string>(p, "cc", alreadyInElement);
    alreadyInElement = false;
    }

    if(!alreadyInElement) munchSpace(p);
    result.from = fromXML<std::string>(p, "from", alreadyInElement);
    alreadyInElement = false;

    if(!alreadyInElement) munchSpace(p);
    result.heading = fromXML<std::string>(p, "heading", alreadyInElement);
    alreadyInElement = false;

    if(!alreadyInElement) munchSpace(p);
    result.body = fromXML<std::string>(p, "body", alreadyInElement);
    alreadyInElement = false;

    if(!alreadyInElement) munchSpace(p);
    result.count = fromXML<int>(p, "count", alreadyInElement);
    alreadyInElement = false;

    if(!alreadyInElement) munchSpace(p);
    result.fraction = fromXML<double>(p, "fraction", alreadyInElement);
    alreadyInElement = false;

    if(!alreadyInElement) munchSpace(p);
    result.date_today = fromXML<date>(p, "date_today", alreadyInElement);
    alreadyInElement = false;

    munchSpace(p);
    p.next_expect(xml::parser::end_element);
    return result;
}

template<>
date fromXML<date>(xml::parser &p, std::string name, bool alreadyInElement) {
    if(!alreadyInElement)
        p.next_expect(xml::parser::start_element);
    p.next();
    date result = p.value();
    p.next_expect(xml::parser::end_element);
    return result;        
}

template<>
double fromXML<double>(xml::parser &p, std::string name, bool alreadyInElement) {
    if(!alreadyInElement)
        p.next_expect(xml::parser::start_element);
    p.next();
    double result = std::stod(p.value());
    p.next_expect(xml::parser::end_element);
    return result;        
}

template<>
int fromXML<int>(xml::parser &p, std::string name, bool alreadyInElement) {
    if(!alreadyInElement)
        p.next_expect(xml::parser::start_element);
    p.next();
    int result = std::stoi(p.value());
    p.next_expect(xml::parser::end_element);
    return result;        
}

template<>
std::string fromXML<std::string>(xml::parser &p, std::string name, bool alreadyInElement) {
    if(!alreadyInElement)
        p.next_expect(xml::parser::start_element);
    std::string result;
    while(p.peek() == xml::parser::characters) {
        p.next();
        result += p.value();
    }
    p.next_expect(xml::parser::end_element);
    return result;        
}

