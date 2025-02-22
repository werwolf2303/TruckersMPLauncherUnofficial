use eframe::emath::Align;
use eframe::epaint::{Color32, FontFamily, FontId};
use eframe::Frame;
use egui::{Button, RichText, Vec2, ViewportBuilder};

pub struct ErrorDialog {
    pub title: String,
    pub message: String,
    pub visible: bool,
}

impl eframe::App for ErrorDialog {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.vertical(|ui| {
                ui.label(RichText::new(self.message.clone()).color(Color32::RED).font(FontId::new(13.0, FontFamily::default())));
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
    }
}

impl ErrorDialog {
    pub fn new(
        title: String,
        message: String,
    ) -> Self {
        ErrorDialog {
            title,
            message: message.to_string(),
            visible: false,
        }
    }

    pub fn open(
        self,
    ) -> eframe::Result {
        let options = eframe::NativeOptions {
            viewport: ViewportBuilder::default().with_inner_size([300.0, 100.0]).with_resizable(false),
            ..Default::default()
        };
        eframe::run_native(
            self.title.clone().as_str(),
            options,
            Box::new(|cc| Ok(Box::new(self))),
        )
    }
}