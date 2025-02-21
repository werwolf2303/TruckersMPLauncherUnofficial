use crate::launcher::utils::get_url_as_string;
use crate::public_values::{files_endpoint, DOWNLOAD_URL};
use crate::ui::error_dialog::ErrorDialog;
use serde_json::Error;
use std::process::exit;

#[derive(serde::Deserialize, Debug)]
pub struct File {
    #[serde(rename = "FilePath")]
    pub file_path: String,
    #[serde(rename = "Type")]
    pub file_type: String,
    #[serde(rename = "Md5")]
    pub md5: String
}

#[derive(serde::Deserialize, Debug)]
pub struct Files {
    #[serde(rename = "Files")]
    pub files: Vec<File>
}

pub async fn get_files() -> Result<Vec<File>, Error> {
    let result = get_url_as_string(files_endpoint().to_string()).await;
    if result.is_err() {
        let error = result.unwrap_err();
        ErrorDialog::new(
            "Fatal error".to_string(),
            format!("Error: {}", error.to_string())
        ).open().expect("Failed to show error dialog");
        eprintln!("Failed to get files: {:?}", error);
        exit(-1)
    }
    let deserialize: Result<Files, Error> = serde_json::from_str::<Files>(result.unwrap().as_str());
    if deserialize.is_err() {
        return Err(deserialize.unwrap_err())
    }
    Ok(deserialize?.files)
}

pub fn get_file_download_url(file: &File) -> String {
    format!("{}/files{}", DOWNLOAD_URL, file.file_path)
}