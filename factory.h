#ifndef FACTORY_H
#define FACTORY_H
#include<tuple>
#include<memory>
#include<utility>
using std::tuple;
using std::unique_ptr;
using std::make_unique;
using std::forward;

namespace advcppfinal {
	template<typename...> struct TT {};

	template<typename T>
	struct abstract_creator {
		virtual unique_ptr<T> doCreate(TT<T>&&) = 0;
	};
	template<typename T, typename... TArgs>
	struct abstract_creator<T(TArgs...)> {
		virtual unique_ptr<T> doCreate(TT<T, TArgs...>&&, TArgs&&...) = 0;
	};

	template<typename... Ts>
	struct flexible_abstract_factory : public abstract_creator<Ts>... {
		template<class U>
		unique_ptr<U> create() {
			abstract_creator<U>& creator = *this;
			return creator.doCreate(TT<U>());
		}
		template<typename U, typename... UArgs>
		unique_ptr<U> create(UArgs&&... uargs) {
			abstract_creator<U(UArgs...)>& creator = *this;
			return creator.doCreate(TT<U, UArgs...>(), forward<UArgs>(uargs)...);
		}
		virtual ~flexible_abstract_factory() {}
	};

	template<typename AbstractFactory, typename Abstract, typename Concrete>
	struct concrete_creator : virtual public AbstractFactory {
		unique_ptr<Abstract> doCreate(TT<Abstract>&&) override {
			return make_unique<Concrete>();
		}
	};
	template<typename AbstractFactory, typename Abstract, typename Concrete, typename... Args>
	struct concrete_creator<AbstractFactory, Abstract(Args...), Concrete> : virtual public AbstractFactory {
		unique_ptr<Abstract> doCreate(TT<Abstract, Args...>&&, Args&&... args) override {
			return make_unique<Concrete>(forward<Args>(args)...);
		}
	};

	template<typename AbstractFactory, typename... ConcreteTypes>
	struct concrete_factory;

	template<typename... AbstractTypes, typename... ConcreteTypes>
	struct concrete_factory
		<flexible_abstract_factory<AbstractTypes...>, ConcreteTypes...>
		: public concrete_creator<flexible_abstract_factory<AbstractTypes...>,
		AbstractTypes, ConcreteTypes>... {
	};


}
#endif