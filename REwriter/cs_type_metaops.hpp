#pragma once
#include <boost/hana.hpp>
namespace cs::meta_ops {
	template<typename... Ts>
	static constexpr auto make_typelist() {
		return boost::hana::make_tuple(boost::hana::type_c<Ts>...);
	}

	template<typename TFind,typename TList >
	static constexpr bool typelist_has(TList&& l) {
		auto res = boost::hana::find(l, boost::hana::type_c<TFind>);
		if constexpr (std::is_same_v<decltype(res.value()), void>) {
			return false;
		}
		else {
			return true;
		}
	}

}

