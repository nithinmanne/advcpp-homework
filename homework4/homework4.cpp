// homework4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include <utility>

#include <vector>
#include <list>


namespace Nithin::Manne {
    /* Store Const Reference instead of Copy, unless explicitly Moved in */
    class Container {
    private:
        struct AbstractContainer {
            virtual std::size_t size() const noexcept = 0;
            virtual std::size_t max_size() const noexcept = 0;
            virtual bool empty() const noexcept = 0;
            virtual ~AbstractContainer() = default;
        };
        template<typename T>
        struct ConcreteContainerReference : AbstractContainer {
            T const& container;
            explicit ConcreteContainerReference(T const& container) : container(container) {}
            std::size_t size() const noexcept override {
                return container.size();
            }
            std::size_t max_size() const noexcept override {
                return container.max_size();
            }
            bool empty() const noexcept override {
                return container.empty();
            }
        };
        template<typename T>
        struct ConcreteContainerMoved : AbstractContainer {
            T container;
            explicit ConcreteContainerMoved(T&& container) : container(std::move(container)) {}
            std::size_t size() const noexcept override {
                return container.size();
            }
            std::size_t max_size() const noexcept override {
                return container.max_size();
            }
            bool empty() const noexcept override {
                return container.empty();
            }
        };
        std::unique_ptr<AbstractContainer> container;
    public:
        template<typename T>
        Container(T& container) {
            this->container = std::make_unique<ConcreteContainerReference<T>>(container);
        }
        template<typename T>
        Container& operator=(T& container) {
            this->container = std::make_unique<ConcreteContainerReference<T>>(container);
            return *this;
        }
        template<typename T>
        Container(T const& container) {
            this->container = std::make_unique<ConcreteContainerReference<T>>(container);
        }
        template<typename T>
        Container& operator=(T const& container) {
            this->container = std::make_unique<ConcreteContainerReference<T>>(container);
            return *this;
        }
        template<typename T>
        Container(T&& container) {
            this->container = std::make_unique<ConcreteContainerMoved<T>>(std::move(container));
        }
        template<typename T>
        Container& operator=(T&& container) {
            this->container = std::make_unique<ConcreteContainerMoved<T>>(std::move(container));
            return *this;
        }
        std::size_t size() const noexcept {
            return container->size();
        }
        std::size_t max_size() const noexcept {
            return container->max_size();
        }
        virtual bool empty() const noexcept {
            return container->empty();
        }
    };
}


int main()
{
    auto v = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    auto const l = std::list<char>();
    Nithin::Manne::Container a{ v };
    Nithin::Manne::Container b{ l };
    Nithin::Manne::Container c = std::vector<double>(100);
    std::cout << "a(v).size = " << a.size()
        << " a(v).max_size = " << a.max_size()
        << " a(v).empty = " << a.empty() << std::endl;
    std::cout << "b(l).size = " << b.size()
        << " b(l).max_size = " << b.max_size()
        << " b(l).empty = " << b.empty() << std::endl;
    std::cout << "c.size = " << c.size()
        << " c.max_size = " << c.max_size()
        << " c.empty = " << c.empty() << std::endl;
    v.push_back(11);
    std::cout << "a(v).size = " << a.size() << std::endl;
}
