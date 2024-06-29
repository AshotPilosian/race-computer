#include "ColoredSectorTimesPanel.h"

ColoredSectorTimesPanel::ColoredSectorTimesPanel(lv_obj_t *parent,
                                                 const int32_t height): parent(parent), height(height) {
    initStyles();
    init();
}

ColoredSectorTimesPanel::~ColoredSectorTimesPanel() {
    delete widget.sector1;
    delete widget.sector2;
    delete widget.sector3;
}

void ColoredSectorTimesPanel::update(const SectorTimesUpdateData &data) const {
    widget.sector1->update(data.sector1);
    widget.sector2->update(data.sector2);
    widget.sector3->update(data.sector3);
}

// Private
void ColoredSectorTimesPanel::initStyles() {
    lv_style_init(&sectorTimesContainerStyle);
    lv_style_set_radius(&sectorTimesContainerStyle, 0);
    lv_style_set_pad_all(&sectorTimesContainerStyle, 0);
    lv_style_set_pad_column(&sectorTimesContainerStyle, 0);
    lv_style_set_pad_row(&sectorTimesContainerStyle, 0);
    lv_style_set_border_side(&sectorTimesContainerStyle, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(&sectorTimesContainerStyle, 1);
}

void ColoredSectorTimesPanel::init() {
    widget.container = lv_obj_create(parent);
    lv_obj_add_style(widget.container, &sectorTimesContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(widget.container, lv_pct(100), height);
    lv_obj_set_pos(widget.container, 0, 0);
    lv_obj_set_flex_flow(widget.container, LV_FLEX_FLOW_ROW);
    lv_obj_set_scrollbar_mode(widget.container, LV_SCROLLBAR_MODE_OFF);

    widget.sector1 = new SectorTimeColoredWidget(widget.container, lv_pct(33));
    widget.sector2 = new SectorTimeColoredWidget(widget.container, lv_pct(35));
    widget.sector3 = new SectorTimeColoredWidget(widget.container, lv_pct(33));
}
