use rust_embed::Embed;
use std::ops::Deref;

#[derive(Embed)]
#[folder = "Files"]
struct Files;

pub fn get_truckersmp_cli() -> Vec<u8> {
    Files::get("truckersmp-cli.exe").unwrap().data.deref().to_vec()
}