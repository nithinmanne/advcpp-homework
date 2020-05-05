#ifndef FACTORY_H
#define FACTORY_H
#include<tuple>
#include<memory>
#include<utility>
using std::tuple;
using std::unique_ptr;
using std::make_unique;
using std::forward;

namespace cspp51045 {
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

	template<typename AbstractFactory, typename Abstract, template<typename> typename ConcreteTemplate>
	struct concrete_creator : virtual public AbstractFactory {
		unique_ptr<Abstract> doCreate(TT<Abstract>&&) override {
			return make_unique<ConcreteTemplate<Abstract>>();
		}
	};
	template<typename AbstractFactory, typename Abstract, template<typename> typename ConcreteTemplate, typename... Args>
	struct concrete_creator<AbstractFactory, Abstract(Args...), ConcreteTemplate> : virtual public AbstractFactory {
		unique_ptr<Abstract> doCreate(TT<Abstract, Args...>&&, Args&&... args) override {
			return make_unique<ConcreteTemplate<Abstract>>(forward<Args>(args)...);
		}
	};

	template<typename AbstractFactory, template<typename> typename ConcreteTemplate>
	struct parameterized_factory;

	template<typename... AbstractTypes, template<typename> typename ConcreteTemplate>
	struct parameterized_factory
		<flexible_abstract_factory<AbstractTypes...>, ConcreteTemplate>
		: public concrete_creator<flexible_abstract_factory<AbstractTypes...>,
		AbstractTypes, ConcreteTemplate>... {
	};


}
#endif