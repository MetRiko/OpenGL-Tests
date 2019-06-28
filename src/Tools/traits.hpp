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

	// Check if <T> is glm::vec

	template <class>
	struct is_glm_vec : std::false_type {};

	template <int S, typename T, glm::qualifier P>
	struct is_glm_vec<glm::vec<S,T,P>> : std::true_type {};

	template <class X>
	static constexpr bool is_glm_vec_v = is_glm_vec<X>::value;
}