// factory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "factory.h"

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
    virtual int power() = 0;
};

struct FreightCar : public TrainCar {
    virtual double capacity() = 0;
};

struct Caboose : public TrainCar {
    virtual std::string use() = 0;
};

struct ModelLocomotive : public Locomotive {
    virtual int power() {
        return 1;
    }
};

struct ModelFreightCar : public FreightCar {
    virtual double capacity() {
        return 1.0;
    }
};

struct ModelCaboose : public Caboose {
    virtual std::string use() {
        return "Looks cool, I guess...";
    }
};

struct RealLocomotive : Locomotive {
    virtual int power() {
        return 100000;
    }
};

struct RealFreightCar : public FreightCar {
    virtual double capacity() {
        return 100000.0;
    }
};

struct RealCaboose : public Caboose {
    virtual std::string use() {
        return "Nothing";
    }
};


using train_factory = abstract_factory<Locomotive, FreightCar, Caboose>;

using model_train_factory = concrete_factory<train_factory, ModelLocomotive, ModelFreightCar, ModelCaboose>;
using real_train_factory = concrete_factory<train_factory, RealLocomotive, RealFreightCar, RealCaboose>;


int main()
{
    /* Use real_train_factory here, to change the entire code to use real trains. */
    std::unique_ptr<train_factory> factory = std::make_unique<model_train_factory>();

    std::shared_ptr<Locomotive> locomotive = factory->create<Locomotive>();
    std::shared_ptr<FreightCar> firstFreightCar = factory->create<FreightCar>();
    std::shared_ptr<FreightCar> secondFreightCar = factory->create<FreightCar>();
    std::shared_ptr<FreightCar> thirdFreightCar = factory->create<FreightCar>();
    std::shared_ptr<Caboose> caboose = factory->create<Caboose>();

    locomotive->setNext(firstFreightCar);
    firstFreightCar->setNext(secondFreightCar);
    secondFreightCar->setNext(thirdFreightCar);
    thirdFreightCar->setNext(caboose);

    std::cout << "Locomotive Power: " << locomotive->power() << std::endl;
    std::cout << "Caboose Usage: " << caboose->use() << std::endl;
}
