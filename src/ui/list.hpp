#pragma once

#include "ui.hpp"
#include "ui/widget.hpp"

#include <memory>
#include <string>
#include <vector>

namespace ui {

struct List : Widget {
	struct Element {
		std::string string;
		Text text;
	};

	~List();

	virtual auto draw() -> void override;

	virtual auto up() -> void override;
	virtual auto down() -> void override;
	virtual auto yes() -> void;

	auto select(size_t idx) -> void;
	auto unselect(size_t idx) -> void;


	std::vector<Element> elements;
	Polygon marker_border;
	size_t selection = 0;
};

auto list(int x, int y, const std::vector<std::string> &strings) -> Widget*;

}
