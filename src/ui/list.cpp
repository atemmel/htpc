#include "list.hpp"
#include "ui.hpp"

#include <memory>

namespace ui {

List::~List() {
	for(auto& e : elements) {
		e.text.destroy();
	}
}

auto List::draw() -> void {
	constexpr auto selected_h = 100;
	constexpr auto half_selected_h = selected_h / 2;
	auto selected_text = &elements[selection].text;

	auto sel_rect = SDL_Rect{
		.x = (int)x + selected_text->rect.x,
		.y = (int)y + selected_text->rect.y,
		.w = selected_text->rect.w,
		.h = selected_h,
	};

	marker_border.setOffset(sel_rect.x, sel_rect.y + half_selected_h);
	ui::draw(marker_border);
	marker_border.setOffset(sel_rect.x + sel_rect.w, sel_rect.y + half_selected_h);
	ui::draw(marker_border);

	SDL_SetRenderDrawColor(ui::renderer, fg.r, fg.g, fg.b, fg.a);
	SDL_RenderFillRect(ui::renderer, &sel_rect);

	for(auto& e : elements) {
		ui::draw(e.text, x, y);
	}
}

auto List::up() -> void {
	if(selection == 0) {
		return;
	}
	unselect(selection);
	selection--;
	select(selection);
}

auto List::down() -> void {
	if(selection == elements.size() -1) {
		return;
	}
	unselect(selection);
	selection++;
	select(selection);
}

auto List::yes() -> void {

}

auto List::select(size_t idx) -> void {
	SDL_SetTextureColorMod(elements[idx].text.texture, 50, 50, 50);
}

auto List::unselect(size_t idx) -> void {
	SDL_SetTextureColorMod(elements[idx].text.texture, fg.r, fg.g, fg.b);
}


auto list(int x, int y, const std::vector<std::string> &strings) -> Widget* {
	auto l = std::make_unique<List>();
	l->x = x;
	l->y = y;
	l->selection = 0;
	l->elements.reserve(strings.size());
	{
		Uint32 y = 0;
		for(auto& str : strings) {
			l->elements.push_back({
				.string = str,
				.text = ui::text(ui::font, str.c_str(), fg),
			});
			auto e = &l->elements.back();
			e->text.rect.x = 0;
			e->text.rect.y = y;
			y += 100;
		}
	}
	l->select(l->selection);
	l->marker_border = ui::circle(50, 50.f, {255, 255, 255, 255});
	return ui::registerWidget(std::move(l));
}

}
