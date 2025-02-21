use crate::launcher::game::game::Game;
use crate::launcher::os::system::OperatingSystems;
use crate::launcher::steam::get_steam_path;
use crate::launcher::utils::get_home_directory;
use crate::public_values::OPERATING_SYSTEM_VAR;
use crate::ui::error_dialog::ErrorDialog;
use crate::PathBufExt;
use std::ops::Deref;
use std::path::PathBuf;
use std::process::exit;

pub struct ETS2 {
    pub steam_library_path: PathBuf,
    pub game_id: String,
    pub game_name: String,
}

impl Game for ETS2 {
    fn new(steam_library_path: PathBuf) -> Self {
        ETS2 {
            steam_library_path,
            game_id: "227300".to_string(),
            game_name: "ETS2".to_string(),
        }
    }

    fn get_game_directory(&self) -> PathBuf {
        PathBuf::new()
            .append(&self.steam_library_path)
            .append("common/Euro Truck Simulator 2")
    }
    fn get_truckersmp_directory(&self) -> PathBuf {
        let operating_system_raw = OPERATING_SYSTEM_VAR.lock().unwrap();
        let operating_system = operating_system_raw.deref();
        match operating_system {
            OperatingSystems::WINDOWS => PathBuf::new()
                    .append(get_home_directory())
                    .append("AppData/Local/TruckersMP"),
            OperatingSystems::LINUX => PathBuf::new()
                    .append(get_steam_path(operating_system))
                    .append("steamapps/compatdata/227300/pfx/drive_c/users/steamuser/AppData/Local/TruckersMP"),
            _ => {
                ErrorDialog::new(
                    "Fatal error".to_string(),
                    "Unsupported operating system".to_string()
                ).open().expect("Failed to show error dialog");
                eprintln!("Unsupported operating system");
                exit(-1)
            },
        }
    }

    fn get_game_name(&self) -> String {
        self.game_name.to_string()
    }

    fn get_game_id(&self) -> String {
        self.game_id.to_string()
    }

    fn get_prefix_directory(&self) -> PathBuf {
        PathBuf::new()
            .append(&self.steam_library_path)
            .append("compatdata/").append(self.game_id.as_str())
    }
}


