#pragma once

template<typename T>
struct Maybe {
	T value;
	bool present;

	operator bool() const {
		return present;
	}

	operator T&() {
		return value;
	}

	operator const T&() const {
		return value;
	}
};

template<typename T>
auto maybe() -> Maybe<T> {
	return Maybe<T>{
		.value = {},
		.present = false,
	};
}

template<typename T>
auto maybe(T&& t) -> Maybe<T> {
	return Maybe<T>{
		.value = t,
		.present = true,
	};
}
