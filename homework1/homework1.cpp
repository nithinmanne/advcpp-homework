// homework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>


class Student {
private:
    std::string name;
    std::vector<int> hw;
    int finalProject;
public:
    Student(std::string const& name) : name{ name }, finalProject{ 0 } {}
    Student(std::string&& name) : name{ std::move(name) }, finalProject{ 0 } {}

    void setName(std::string const& name) {
        this->name = name;
    }
    void setName(std::string&& name) {
        this->name = std::move(name);
    }
    const std::string& getName() const& {
        return name;
    }
    std::string getName()&& {
        return std::move(name);
    }

    void setHw(std::vector<int> const& hw) {
        this->hw = hw;
    }
    void setHw(std::vector<int>&& hw) {
        this->hw = std::move(hw);
    }
    const std::vector<int>& getHw() const& {
        return hw;
    }
    std::vector<int> getHw()&& {
        return std::move(hw);
    }

    void setFinalProject(int finalProject) {
        this->finalProject = finalProject;
    }
    int getFinalProject() {
        return finalProject;
    }

    int getTotal() {
        int total = finalProject;
        for (int score : hw) total += score;
        return total;
    }

};

std::ostream& operator<<(std::ostream& os, Student student) {
    os << "Student{ " << student.getName() << ", Total=" << student.getTotal() << " }";
    return os;
}

int main()
{
    Student student{ "Nithin" };
    student.setHw({10, 10, 10, 10, 10, 10});
    student.setFinalProject(40);
    std::cout << student << std::endl;
    std::cout << "Scores: ";
    for (int score : student.getHw()) std::cout << score << ", ";
    std::cout << student.getFinalProject() << std::endl;
}
