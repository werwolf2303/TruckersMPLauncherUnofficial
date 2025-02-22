use crate::launcher::game::game::Game;
use crate::launcher::utils::{check_game, download, launch_game, verify};
use crate::ui::error_dialog::ErrorDialog;
use eframe::Frame;
use egui::{Spinner, ViewportBuilder};
use once_cell::sync::Lazy;
use std::ops::Deref;
use std::path::PathBuf;
use std::sync::{Arc, Mutex};
use std::sync::mpsc::{Receiver, Sender};
use std::thread;
use crate::launcher::game::ets2::ETS2;

pub struct MainWindow {
    pub games: Arc<Vec<Box<dyn Game>>>,
    pub selected_game: String,
}

impl Default for MainWindow {
    fn default() -> Self {
        MainWindow {
            games: Arc::new(vec![]),
            selected_game: "".to_string()
        }
    }
}

impl eframe::App for MainWindow {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.vertical(|ui| {
                ui.horizontal(|ui| {
                    ui.label("Game");
                    egui::ComboBox::from_id_salt("gamesselector")
                        .selected_text(self.selected_game.clone())
                        .show_ui(ui, |ui| {
                            for item in self.games.iter() {
                                ui.selectable_value(
                                    &mut self.selected_game,
                                    item.get_game_name().clone(),
                                    item.get_game_name(),
                                );
                            }
                        });
                    if ui.button("Launch Game").clicked() {
                        let mut game: Option<&Box<dyn Game>> = None;
                        for g in self.games.iter() {
                            if g.get_game_name() == self.selected_game {
                                game = Some(g);
                                break;
                            }
                        }
                        let game_unwrapped = game.unwrap();
                        if check_game(&game_unwrapped) {
                            let result = verify(&game_unwrapped.get_truckersmp_directory());
                            if !result.1 {
                                let download_finished_callback = |success| {};
                                let download_of_item_finished_callback = |current: u64, total| {
                                    println!("Downloading {}/{}", current, total);
                                };

                                let rt = tokio::runtime::Builder::new_current_thread()
                                    .enable_all()
                                    .build()
                                    .unwrap();
                                rt.block_on(async { download(
                                    &game_unwrapped.get_truckersmp_directory(),
                                    &game_unwrapped.get_game_name().as_str(),
                                    download_finished_callback,
                                    download_of_item_finished_callback,
                                    |first, second, third| {},
                                    result.0
                                ).await });
                            }
                            launch_game(
                                game_unwrapped.get_game_directory(),
                                game_unwrapped.get_truckersmp_directory(),
                                PathBuf::new()
                                    .join(game_unwrapped.get_truckersmp_directory())
                                    .join("truckersmp-cli.exe"),
                                game_unwrapped.get_prefix_directory(),
                            );
                        } else {
                            ErrorDialog::new(
                                "Fatal error".to_string(),
                                "Please start the game once".to_string(),
                            )
                                .open()
                                .expect("Failed to open error dialog");
                            eprintln!("Please start the game once");
                        }
                    };
                });
            });
        });
    }
}

impl MainWindow {
    pub fn set_games(mut self, games: Vec<Box<dyn Game>>) -> Self {
        self.games = Arc::new(games);
        self
    }

    pub fn open(self) -> eframe::Result {
        let options = eframe::NativeOptions {
            viewport: ViewportBuilder::default()
                .with_inner_size([400.0, 100.0])
                .with_resizable(false),
            ..Default::default()
        };
        eframe::run_native(
            "Unofficial TruckersMP Launcher - v0.1",
            options,
            Box::new(|cc| Ok(Box::new(self))),
        )
    }
}
