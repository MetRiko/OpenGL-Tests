#pragma once

#include <type_traits>

#include <glm/vec2.hpp>

namespace gl::traits {

	// Remove reference and qualifiers from <T>

	template <class T>
	struct remove_cvref {
		using type = std::remove_cv_t<std::remove_reference_t<T>>;
	};
	template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

	// Read data (size, type, precision) from glm::vec 

	template <class X>
	struct read_glm_vec {

		// Declaration
		template <class T> struct Impl;

		// Instance (partial specialization)
		template <template <int, typename, glm::qualifier> class V, int S, typename T, glm::qualifier P>
		struct Impl<V<S, T, P>> {
		    static constexpr int size = S;
		    using type = T;
		    static constexpr glm::qualifier precision = P;
		};

		using data = Impl<remove_cvref_t<X>>;
	};
	template <class V> using read_glm_vec_d = typename read_glm_vec<V>::data;

	// Check if <T> is glm::vec

	template <class>
	struct is_glm_vec : std::false_type {};

	template <int S, typename T, glm::qualifier P>
	struct is_glm_vec<glm::vec<S,T,P>> : std::true_type {};

	template <class X>
	static constexpr bool is_glm_vec_v = is_glm_vec<X>::value;
}