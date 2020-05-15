/* NB: I tried... */
#ifndef FACTORY_H
#define FACTORY_H
#include<tuple>
#include<memory>
#include<utility>

#include<boost/mp11.hpp>

using std::tuple;
using std::unique_ptr;
using std::make_unique;
using std::forward;

namespace bm = boost::mp11;

namespace cspp51045 {
	template<typename...> struct TT {};

	template<typename T>
	struct extract_class {
		using type = TT<T>;
	};
	template<typename T, typename... TArgs>
	struct extract_class<T(TArgs...)> {
		using type = TT<T, TArgs...>;
	};
	template<typename T>
	using extract_class_t = typename extract_class<T>::type;

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
		using type_map = bm::mp_list<bm::mp_list<Ts, extract_class_t<Ts>, abstract_creator<Ts>*>...>;
		template<class U>
		unique_ptr<U> create() {
			return static_cast<bm::mp_third<bm::mp_map_find<type_map, U>>>(this)->doCreate(bm::mp_second<bm::mp_map_find<type_map, U>>());
		}
		template<typename U, typename... UArgs>
		unique_ptr<U> create(UArgs&&... uargs) {
			return static_cast<bm::mp_third<bm::mp_map_find<type_map, U(UArgs...)>>>(this)->doCreate(bm::mp_second<bm::mp_map_find<type_map, U(UArgs...)>>(), forward<UArgs>(uargs)...);
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