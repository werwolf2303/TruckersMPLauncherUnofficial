use std::path::PathBuf;
use std::str::FromStr;
use crate::launcher::game::game::Games::{ATS, ETS2};

pub trait Game: Sync + Send {
    fn get_game_directory(&self) -> PathBuf;
    fn get_truckersmp_directory(&self) -> PathBuf;
    fn get_prefix_directory(&self) -> PathBuf;
    fn get_game_name(&self) -> String;
    fn get_game_id(&self) -> String;
    fn new(steam_library_path: PathBuf) -> Self
    where
        Self: Sized;
}

#[derive(PartialEq, Debug)]
pub enum Games {
    ETS2,
    ATS,
}

impl FromStr for Games {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "ETS2" => Ok(ETS2),
            "ATS" => Ok(ATS),
            _ => Err(()),
        }
    }
}

impl Games {
    pub(crate) const fn get_id(self) -> u32 {
        match self {
            Games::ETS2 => 227300,
            Games::ATS => 270880,
        }
    }
}