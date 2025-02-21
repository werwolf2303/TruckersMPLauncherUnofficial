use crate::launcher::game::game::Game;
use crate::launcher::os::system::OperatingSystems;
use once_cell::sync::Lazy;
use std::sync::Mutex;

pub static OPERATING_SYSTEM_VAR: Lazy<Mutex<OperatingSystems>> = Lazy::new(|| Mutex::new(
    OperatingSystems::LINUX
));

pub static DOWNLOAD_URL: &str = "https://download-new.ets2mp.com";
pub static API_URL: &str = "https://api.truckersmp.com";
pub static UPDATE_URL: &str = "https://update.ets2mp.com";

pub fn files_endpoint() -> String {
    format!("{}{}", UPDATE_URL, "/files.json")
}

pub fn servers_endpoint() -> String {
    format!("{}{}", API_URL, "/servers")
}