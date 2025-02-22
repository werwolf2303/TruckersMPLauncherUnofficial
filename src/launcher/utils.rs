use crate::api::file::{get_file_download_url, get_files, File};
use crate::downloader::Downloader;
use crate::files::truckersmp_cli::get_truckersmp_cli;
use crate::launcher::os::system::OperatingSystems;
use crate::launcher::steam::get_steam_path;
use crate::public_values::{OPERATING_SYSTEM_VAR};
use crate::ui::error_dialog::ErrorDialog;
use crate::PathBufExt;
use dirs::home_dir;
use md5::Digest;
use regex::Regex;
use serde_json::{Map, Value};
use std::fs;
use std::fs::OpenOptions;
use std::io::{BufReader, BufWriter, Read, Write};
use std::ops::Deref;
use std::path::{Path, PathBuf};
use std::process::{exit, Command, Stdio};
use crate::launcher::game::game::Game;

fn extract_proton_version(path: &PathBuf) -> Option<u32> {
    let re = Regex::new(r"Proton (\d+)").unwrap();
    path.file_name()
        .and_then(|name| name.to_str())
        .and_then(|s| re.captures(s))
        .and_then(|caps| caps.get(1))
        .and_then(|m| m.as_str().parse::<u32>().ok())
}


pub fn launch_game(
    game_path: PathBuf,
    truckersmp_path: PathBuf,
    launcher_path: PathBuf,
    prefix_path: PathBuf,
) {
    let proton_matching_regex = Regex::new(r"Proton [0-9]+").unwrap();
    let mut proton_versions: Vec<PathBuf> = Vec::new();
    for folder in fs::read_dir(PathBuf::new()
        .append(get_steam_path(OPERATING_SYSTEM_VAR.lock().unwrap().deref())
            .append("steamapps/common"))).unwrap() {
        let folder = folder.unwrap();
        if folder.path().is_dir() {
            if proton_matching_regex.is_match(folder.path().to_str().unwrap()) {
                proton_versions.push(folder.path().to_path_buf());
            }
        }
    }
    proton_versions.sort_by_key(|path| extract_proton_version(path).unwrap_or(0));
    proton_versions.reverse();

    if !launcher_path.exists() {
        let file = OpenOptions::new()
            .create(true)
            .write(true)
            .open(&launcher_path)
            .unwrap();
        let mut buffer_writer = BufWriter::new(file);
        buffer_writer.write(get_truckersmp_cli().deref()).unwrap();
        buffer_writer.flush().unwrap();
    }

    let full_command_linux = format!(
        "\"{}\" run \"{}\" \"{}\" \"{}\"",
        PathBuf::new()
            .append(proton_versions.first().unwrap())
            .append("proton").to_str().unwrap(), launcher_path.to_str().unwrap(), game_path.to_str().unwrap(), truckersmp_path.to_str().unwrap()
    );

    let full_command_windows = format!(
        "\"{}\" \"{}\" \"{}\"",
        launcher_path.to_str().unwrap(), game_path.to_str().unwrap(), truckersmp_path.to_str().unwrap()
    );


    let mut command = match OPERATING_SYSTEM_VAR.lock().unwrap().deref() {
        OperatingSystems::WINDOWS => {
            let mut cmd = Command::new("cmd");
            cmd.arg("/C").arg(full_command_windows);
            cmd
        },
        _ => {
            let mut bash = Command::new("/usr/bin/bash");
            bash.arg("-c").arg(full_command_linux);
            bash
        },
    };

    let steam_path = get_steam_path(OPERATING_SYSTEM_VAR.lock().unwrap().deref());

    command.env("STEAM_COMPAT_DATA_PATH", prefix_path.to_str().unwrap());
    command.env("STEAM_COMPAT_CLIENT_INSTALL_PATH", steam_path.to_str().unwrap());

    println!("Launching TruckersMP with:");
    for (key, value) in command.get_envs() {
        if let Some(value) = value {
            println!("{}={}", key.to_string_lossy(), value.to_string_lossy());
        }
    }

    // Start the process
    match command.stdout(Stdio::inherit()).stderr(Stdio::inherit()).spawn() {
        Ok(mut child) => {
            let status = child.wait();
            match status {
                Ok(code) if code.success() => println!("TruckersMP launched successfully!"),
                Ok(code) => eprintln!("TruckersMP exited with status: {:?}", code),
                Err(err) => {
                    eprintln!("Failed to launch TruckersMP: {}", err);
                    exit(-1);
                }
            }
        }
        Err(err) => {
            eprintln!("Failed injecting: {}", err);
            exit(-1);
        }
    }
}

pub async fn download<F, G, T>(
    truckersmp_path: &PathBuf,
    filter: &str,
    callback_finished: F,
    callback_item: G,
    callback_progress: T,
    files_to_download: Vec<PathBuf>
)
where
    F: FnMut(bool) + 'static,
    G: FnMut(u64, u64) + 'static,
    T: FnMut(f64, u64, u64) + 'static,
{
    let mut download: Downloader = Downloader::new(
        callback_finished,
        callback_item,
        callback_progress
    );
    let online_files = get_files().await.unwrap();
    if files_to_download.is_empty() {
        for file in online_files.iter() {
            if !file.file_type.eq_ignore_ascii_case("system") && !file.file_type.eq_ignore_ascii_case(filter) {
                continue;
            }
            let file_path = PathBuf::new()
                .append(truckersmp_path.clone())
                .append(&file.file_path);
            if !fs::exists(&file_path.parent().unwrap()).unwrap() {
                let result = fs::create_dir_all(&file_path.parent().unwrap());
                if result.is_err() {
                    let error = result.unwrap_err();
                    ErrorDialog::new(
                        "Fatal error".to_string(),
                        format!("Error: {}", error.to_string())
                    ).open().expect("Failed to open error dialog");
                    eprintln!("Failed to create directory: {:?}", error);
                    exit(-1)
                }
            }
            download.download(&get_file_download_url(file), file_path.to_str().unwrap())
        }
    }else{
        for entry in files_to_download.iter() {
            for online_file in online_files.iter() {
                if !online_file.file_type.eq_ignore_ascii_case("system") && !online_file.file_type.eq_ignore_ascii_case(filter) {
                    continue;
                }
                let file_path = PathBuf::new()
                    .append(truckersmp_path.clone())
                    .append(&online_file.file_path);
                if file_path.eq(entry) {
                    download.download(&get_file_download_url(online_file), file_path.to_str().unwrap());
                    break;
                }
            }
        }
    }
    let state = download.start().await;
    if state.is_err() {
        let error = state.unwrap_err();
        ErrorDialog::new(
            "Fatal error".to_string(),
            format!("Error: {}", error.to_string())
        ).open().expect("Failed to open error dialog");
        eprintln!("Failed to download files: {:?}", error);
        exit(-1)
    }
    let mut map = Map::new();
    for file in online_files.iter() {
        if file.file_type.eq_ignore_ascii_case("system") || file.file_type.eq_ignore_ascii_case(filter) {
            map.insert(file.file_path.strip_prefix("/").unwrap().to_string(), Value::String(file.md5.to_string()));
        }
    }
    let file = OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(PathBuf::new()
            .append(truckersmp_path.clone())
            .append("md5.json")
        )
        .unwrap();
    let mut writer = BufWriter::new(file);
    serde_json::to_writer(&mut writer, &map).unwrap();
    writer.flush().unwrap();
}

pub fn verify(
    truckersmp_path: &PathBuf
) -> (Vec<PathBuf>, bool) {
    if !truckersmp_path.exists() {
        let ret = std::fs::create_dir(truckersmp_path.clone());
        if ret.is_err() {
            let error = ret.unwrap_err();
            ErrorDialog::new(
                "Fatal error".to_string(),
                format!("Error: {}", error.to_string())
            ).open().expect("Failed to open error dialog");
            eprintln!("Failed to create truckersmp directory: {:?}", error);
            exit(-1)
        }
        return (Vec::new(), false)
    }
    let md5_file_output = fs::read_to_string(
        PathBuf::new()
            .append(truckersmp_path.clone())
            .append("md5.json")
    );
    if md5_file_output.is_err() {
        return (Vec::new(), false);
    }
    let md5_file_output_unwrapped = md5_file_output.unwrap();
    let json_value: Value = serde_json::from_str::<Value>(md5_file_output_unwrapped.as_str()).expect("Invalid JSON");
    let mut files_that_need_update: Vec<PathBuf> = Vec::new();
    if let Value::Object(map) = json_value {
        for (key, value) in map {
            let file_path = PathBuf::new()
                .append(truckersmp_path.clone())
                .append(key);
            if !file_path.exists() {
                files_that_need_update.push(file_path.clone());
                continue;
            }
            let md5_for_file = calculate_md5(file_path.clone());
            if md5_for_file.unwrap() != value {
                files_that_need_update.push(file_path);
            }
        }
    }
    (files_that_need_update.clone(), files_that_need_update.is_empty())
}

pub async fn get_url_as_string(url: String) -> Result<String, reqwest::Error> {
    let result = reqwest::get(url).await;
    if result.is_err() {
        return Err(result.unwrap_err())
    }
    Ok(result?.text().await?)
}

pub fn extract_parent_from_file(file: &File) -> PathBuf {
    Path::new(file.file_path.as_str()).parent().map(|p| p.to_path_buf()).unwrap()
}

pub fn get_home_directory() -> PathBuf {
    let home = home_dir();
    if home.is_none() {
        ErrorDialog::new(
            "Fatal error".to_string(),
            "Couldn't find home directory".to_string()
        ).open().expect("Failed to open error dialog");
        eprintln!("Couldn't find home directory");
        exit(0);
    }
    home.unwrap()
}

pub fn calculate_md5(file_path: PathBuf) -> Result<String, std::io::Error> {
    let file = OpenOptions::new()
        .create(false)
        .write(false)
        .read(true)
        .open(file_path)
        .unwrap();
    let mut reader = BufReader::new(file);
    let mut hasher = md5::Md5::new();
    let mut buffer = [0; 1024];
    loop {
        let bytes_read = reader.read(&mut buffer)?;
        if bytes_read == 0 {
            break;
        }
        hasher.update(&buffer[..bytes_read]);
    }
    let result = hasher.finalize();
    Ok(base16ct::lower::encode_string(&*result).to_string())
}

pub fn check_game(
    game_to_check: &Box<dyn Game>
) -> bool {
    game_to_check.get_truckersmp_directory().parent().unwrap().exists()
}