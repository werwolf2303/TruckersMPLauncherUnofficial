use std::any::{Any, TypeId};
use std::ops::{Deref, DerefMut};
use std::path::PathBuf;
use std::str::FromStr;
use egui::ViewportBuilder;
use crate::launcher::game::ets2::ETS2;
use crate::launcher::game::game::{Game, Games};
use crate::launcher::utils::{check_game, download, launch_game, verify};
use crate::ui::error_dialog::ErrorDialog;
use crate::ui::window::{ValueEntry, Window};

pub struct MainWindow;

impl MainWindow {
    pub fn open(games: Vec<Box<dyn Game>>) -> eframe::Result {
        Window::default()
            .set_values(
                vec![
                    ValueEntry::from("selectedgame".to_string(), Box::new(games.get(0).unwrap().get_game_name())),
                ]
            )
            .title("Unofficial TruckersMP Launcher - v0.1".to_string())
            .on_close(move |values| {

            })
            .viewport(ViewportBuilder::default().with_inner_size([400.0, 100.0]).with_resizable(false))
            .set_central_panel(move |ctx, ui, frame, values| {
                egui::CentralPanel::default().show(ctx, |ui| {
                    ui.horizontal(|ui| {
                        let mut selected: String = String::from("");
                        if let Some(any) = values.get("selectedgame") {
                            if let Some(s) = any.downcast_ref::<String>() {
                                selected = s.to_string();
                            }
                        }
                        ui.label("Game");
                        egui::ComboBox::from_id_salt("gamesselector")
                            .selected_text(selected)
                            .show_ui(ui, |ui| {
                                for item in games.iter() {
                                    let selectedgame= values.get_mut("selectedgame").unwrap().downcast_mut::<String>().unwrap();
                                    ui.selectable_value(selectedgame, item.get_game_name().clone(), item.get_game_name());
                                }
                            });
                        if ui.button("Launch Game").clicked() {
                            if let Some(any) = values.get("selectedgame") {
                                if let Some(s) = any.downcast_ref::<String>() {
                                    let mut game: Option<&Box<dyn Game>> = None;
                                    for g in &games {
                                        if g.get_game_name() == *s {
                                            game = Some(g);
                                            break;
                                        }
                                    }
                                    let game_unwrapped = game.unwrap();
                                    if check_game(&game_unwrapped) {
                                        let result = verify(&game_unwrapped.get_truckersmp_directory());
                                        if !result.1 {
                                            let download_finished_callback = |success| {
                                                if success {
                                                    println!("All downloads completed successfully!");
                                                } else {
                                                    println!("Some downloads failed.");
                                                }
                                            };

                                            let download_of_item_finished_callback = |current: u64, total| {
                                                println!("Download finished {}/{}", current, total);
                                            };

                                            let download_progress_callback = |speed, downloaded, total| {
                                                //println!("Speed: {:.2} MB/s, Downloaded: {} bytes, Total: {} bytes", speed, downloaded, total);
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
                                                download_progress_callback,
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
                                        )
                                    } else {
                                        ErrorDialog::new(
                                            "Fatal error".to_string(),
                                            "Please start the game once".to_string()
                                        ).open().expect("Failed to show error dialog");
                                        eprintln!("Please start the game once")
                                    }
                                }
                            }
                        }
                    });
                });
            }).open()
    }
}