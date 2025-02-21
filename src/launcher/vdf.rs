use crate::launcher::os::system::OperatingSystems;
use crate::launcher::steam::get_steam_path;
use crate::ui::error_dialog::ErrorDialog;
use crate::PathBufExt;
use keyvalues_parser::Vdf;
use keyvalues_serde::from_vdf;
use serde::Deserialize;
use std::process::exit;
use std::{
    borrow::Cow,
    collections::HashMap,
    fs,
    path::PathBuf,
};

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
pub struct LibraryFolders {
    pub libraries: Vec<Library>,
}

#[derive(Deserialize, Debug)]
#[allow(dead_code)]
pub struct Library {
    pub path: PathBuf,
    pub apps: HashMap<u64, u64>,
}

pub fn get_library_folders(operating_system: &OperatingSystems) -> LibraryFolders {
    let vdf_text = fs::read_to_string(PathBuf::new()
        .append(get_steam_path(operating_system))
        .append("steamapps/libraryfolders.vdf"));
    if vdf_text.is_err() {
        let error = vdf_text.unwrap_err();
        ErrorDialog::new(
            "Fatal error".to_string(),
            format!("Error: {}", error.to_string())
        ).open().expect("Failed to show error dialog");
        eprintln!("Error: {}", error.to_string());
        exit(-1);
    }
    let vdf_text_unwrapped = vdf_text.unwrap();
    let vdf = Vdf::parse(vdf_text_unwrapped.as_str());
    if vdf.is_err() {
        let error = vdf.unwrap_err();
        ErrorDialog::new(
            "Fatal error".to_string(),
            format!("Error: {}", error.to_string())
        ).open().expect("Failed to show error dialog");
        eprintln!("Error: {}", error.to_string());
        exit(-1);
    }
    let mut vdf_unwrapped = vdf.unwrap();
    let obj = vdf_unwrapped.value.get_mut_obj();
    if obj.is_none() {
        ErrorDialog::new(
            "Fatal error".to_string(),
            "Unknown error in line 54 in vdf.rs".to_string()
        ).open().expect("Failed to show error dialog");
        eprintln!("Unknown error in line 54 in vdf.rs");
        exit(-1);
    }

    let mut index = 0;
    let obj_unwrapped = obj.unwrap();
    while let Some(mut library) = obj_unwrapped.remove(index.to_string().as_str()) {
        obj_unwrapped.entry(Cow::from("libraries"))
            .or_insert(Vec::new())
            .push(library.pop().unwrap());

        index += 1;
    }

    let deserialized = from_vdf(vdf_unwrapped);

    if deserialized.is_err() {
        let error = deserialized.unwrap_err();
        ErrorDialog::new(
            "Fatal error".to_string(),
            format!("Error: {}", error.to_string())
        ).open().expect("Failed to show error dialog");
        eprintln!("Error: {}", error.to_string());
        exit(-1);
    }

    let deserialized_unwrapped: LibraryFolders = deserialized.unwrap();

    deserialized_unwrapped
}