use crate::launcher::os::system::OperatingSystems;
use crate::launcher::utils::get_home_directory;
use crate::ui::error_dialog::ErrorDialog;
use crate::PathBufExt;
use std::path::PathBuf;
use std::process::exit;

pub fn get_steam_path(operating_system: &OperatingSystems, ) -> PathBuf {
    let home = get_home_directory();
    match operating_system {
        OperatingSystems::WINDOWS => PathBuf::from(r"C:\Program Files (x86)\Steam"),
        OperatingSystems::LINUX => PathBuf::new().append(home).append(".local/share/Steam"),
        _ => {
            ErrorDialog::new(
                "Fatal error".to_string(),
                "Unsupported operating system".to_string()
            ).open().expect("Failed to open error dialog");
            eprintln!("Unsupported operating system");
            exit(-1)
        }
    }
}