#pragma once

#include "ui.hpp"
#include "ui/widget.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ui {

struct List : Widget {
	using Fn = std::function<Widget*()>;

	struct Option {
		std::string string;
		Fn fn;
	};

	struct Element {
		Fn fn;
		Text text;
		std::string string;
	};

	~List();

	virtual auto draw() -> void override;

	virtual auto up() -> void override;
	virtual auto down() -> void override;
	virtual auto yes() -> void override;

	auto select(size_t idx) -> void;
	auto unselect(size_t idx) -> void;


	std::vector<Element> elements;
	Polygon marker_border;
	size_t selection = 0;
};

auto list(int x, int y, const std::vector<List::Option> &handlers) -> Widget*;

}
