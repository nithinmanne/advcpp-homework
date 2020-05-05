// homework4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "homework4.h"

#include <iostream>
#include <string>
#include <memory>
#include <utility>

using namespace cspp51045;

struct TrainCar {
    std::shared_ptr<TrainCar> next = nullptr;
    void setNext(std::shared_ptr<TrainCar> next) {
        this->next = std::move(next);
    }
    std::shared_ptr<TrainCar> getNext() {
        return next;
    }
    virtual ~TrainCar() = default;
};

struct Locomotive : public TrainCar {
    double horsepower;
    Locomotive(double horsepower) : horsepower{ horsepower } {}
};

struct FreightCar : public TrainCar {
    long capacity;
    FreightCar(long capacity) : capacity{ capacity } {}
};

struct Caboose : public TrainCar {
    virtual std::string use() = 0;
};

struct ModelLocomotive : public Locomotive {
    using Locomotive::Locomotive;
};

struct ModelFreightCar : public FreightCar {
    using FreightCar::FreightCar;
};

struct ModelCaboose : public Caboose {
    virtual std::string use() {
        return "Looks cool, I guess...";
    }
};

struct RealLocomotive : Locomotive {};

struct RealFreightCar : public FreightCar {};

struct RealCaboose : public Caboose {
    virtual std::string use() {
        return "Nothing";
    }
};


using train_factory = flexible_abstract_factory<Locomotive(double horsepower), FreightCar(long capacity), Caboose>;

using model_train_factory = concrete_factory<train_factory, ModelLocomotive, ModelFreightCar, ModelCaboose>;
using real_train_factory = concrete_factory<train_factory, RealLocomotive, RealFreightCar, RealCaboose>;


int main()
{
    /* Use real_train_factory here, to change the entire code to use real trains. */
    std::unique_ptr<train_factory> factory = std::make_unique<model_train_factory>();

    std::shared_ptr<Locomotive> locomotive = factory->create<Locomotive>(1.5);
    std::shared_ptr<FreightCar> firstFreightCar = factory->create<FreightCar>(10l);
    std::shared_ptr<FreightCar> secondFreightCar = factory->create<FreightCar>(10l);
    std::shared_ptr<FreightCar> thirdFreightCar = factory->create<FreightCar>(10l);
    std::shared_ptr<Caboose> caboose = factory->create<Caboose>();

    locomotive->setNext(firstFreightCar);
    firstFreightCar->setNext(secondFreightCar);
    secondFreightCar->setNext(thirdFreightCar);
    thirdFreightCar->setNext(caboose);

    std::cout << "Locomotive Horsepower: " << locomotive->horsepower << std::endl;
    std::cout << "Caboose Usage: " << caboose->use() << std::endl;
}