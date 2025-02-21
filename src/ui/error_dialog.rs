use std::ops::Deref;
use eframe::{egui, Frame};
use egui::{Align, Button, Color32, FontFamily, FontId, RichText, TextBuffer, Vec2, ViewportBuilder};
use crate::ui::window::Window;

pub struct ErrorDialog {
    title: String,
    message: String,
}

impl ErrorDialog {
    pub fn new(
        title: String,
        message: String,
    ) -> Self {
        ErrorDialog {
            title,
            message: message.to_string(),
        }
    }

    pub fn open(
        self,
    ) -> eframe::Result {
        Window::default()
            .viewport(ViewportBuilder::default().with_inner_size([300.0, 100.0]).with_resizable(false))
            .set_central_panel(move |ctx, ui, frame, values| {
                egui::CentralPanel::default().show(ctx, |ui| {
                    egui::CentralPanel::default().show(ctx, |ui| {
                        ui.vertical(|ui| {
                            ui.label(RichText::new(self.message.clone()).color(Color32::WHITE).font(FontId::new(13.0, FontFamily::default())));
                            ui.allocate_space(Vec2::new(ui.available_width() - 62.0, ui.available_height() - 32.0));
                            ui.horizontal(|ui| {
                                ui.spacing_mut().item_spacing = Vec2::new(0.0, 0.0);
                                ui.with_layout(egui::Layout::right_to_left(Align::TOP), |ui| {
                                    ui.horizontal_centered(|ui| {
                                        if ui.add(Button::new("Close").min_size(Vec2::new(60.0, 30.0))).clicked() {
                                            ctx.send_viewport_cmd(egui::ViewportCommand::Close)
                                        }
                                    });
                                });
                            });
                        })
                    });
                });
            }).open()
    }
}