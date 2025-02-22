use crate::launcher::game::ats::ATS;
use crate::launcher::game::ets2::ETS2;
use crate::launcher::game::game::{Game, Games};
use crate::launcher::os::system::OperatingSystems;
use crate::launcher::steam::get_steam_path;
use crate::launcher::utils::{download, launch_game, verify};
use crate::launcher::vdf::get_library_folders;
use crate::public_values::{OPERATING_SYSTEM_VAR};
use crate::ui::error_dialog::ErrorDialog;
use egui::os::OperatingSystem;
use keyvalues_serde::parser::Vdf;
use serde_json::error::Category::Syntax;
use std::any::{Any, TypeId};
use std::collections::BTreeMap;
use std::ops::{Deref, RangeInclusive};
use std::{panic, thread};
use std::path::{Path, PathBuf};
use std::process::exit;
use std::sync::{Arc, Mutex};
use egui::Slider;
use egui::WidgetType::Button;
use crate::ui::main::MainWindow;

pub mod launcher;
pub mod public_values;
pub mod ui;
pub mod downloader;
pub mod api;
mod files;

pub trait PathBufExt {
    fn append<P: AsRef<Path>>(self, path: P) -> Self;
}

impl PathBufExt for PathBuf {
    fn append<P: AsRef<Path>>(self, path: P) -> Self {
        let mut new_path = self;
        if !new_path.to_str().unwrap_or("").is_empty() {
            let path = path.as_ref();
            let path = if path.is_absolute(){
                path.strip_prefix("C:\\").unwrap_or(path).strip_prefix("/").unwrap_or(path)
            } else {
                path
            };
            new_path.push(path);
        } else {
            new_path.push(path);
        }
        new_path
    }
}

fn main() {
    let mut operating_system = OPERATING_SYSTEM_VAR.lock().unwrap();
    if cfg!(windows) {
        *operating_system = OperatingSystems::WINDOWS
    }else if cfg!(unix) {
        *operating_system = OperatingSystems::LINUX
    }else if cfg!(target_os = "macos") {
        *operating_system = OperatingSystems::MACOS
    } else {
        ErrorDialog::new(
            "Fatal error".to_string(),
            "Unknown operating system".to_string()
        ).open().expect("Failed to open error dialog");
        eprintln!("Unknown operating system");
        exit(-1);
    }

    let mut games: Vec<Box<dyn Game>> = Vec::new();

    for elem in get_library_folders(operating_system.deref()).libraries.iter() {
        for app in elem.apps.iter() {
            if app.0.eq(&(Games::ETS2.get_id() as u64)) {
                games.push(Box::new(ETS2::new(PathBuf::new()
                    .append(elem.path.clone())
                    .append("steamapps"))))
            }
            if app.0.eq(&(Games::ATS.get_id() as u64)) {
                games.push(Box::new(ATS::new(PathBuf::new()
                    .append(elem.path.clone())
                    .append("steamapps"))))
            }
        }
    }

    drop(operating_system);

    panic::set_hook(Box::new(|panic_info| {
        eprintln!("{}", panic_info);
    }));

    MainWindow::default().set_games(games).open().expect("Failed to open main window");
}
