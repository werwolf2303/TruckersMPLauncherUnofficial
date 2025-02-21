/*use reqwest::{Client, Error};
use std::fs::{OpenOptions};
use std::io::{BufWriter, Write};
use std::sync::{Arc};
use futures::stream::StreamExt;
use std::time::{Instant};

pub struct Downloader {
    client: Client,
    files_to_download: Vec<(String, String)>,
    downloads_finished_callback: Option<Arc<dyn Fn(bool) + Send + Sync>>,
    download_item_finished_callback: Option<Arc<dyn Fn(u64, u64) + Send + Sync>>,
    progress_callback: Option<Arc<dyn Fn(f64, u64, u64) + Send + Sync>>
}

impl Downloader {
    pub fn new() -> Self {
        Downloader {
            client: Client::new(),
            files_to_download: Vec::new(),
            downloads_finished_callback: None,
            download_item_finished_callback: None,
            progress_callback: None
        }
    }

    pub fn download(&mut self, url: &str, output_path: &str) {
        self.files_to_download.push((url.to_string(), output_path.to_string()));
    }

    pub fn set_downloads_finished_callback<F>(&mut self, callback: F)
    where
        F: Fn(bool) + Send + 'static + Sync,
    {
        self.downloads_finished_callback = Some(Arc::new(callback));
    }

    pub fn set_download_item_finished_callback<F>(&mut self, callback: F)
    where
        F: Fn(u64, u64) + Send + 'static + Sync,
    {
        self.download_item_finished_callback = Some(Arc::new(callback));
    }

    pub fn set_progress_callback<F>(&mut self, callback: F)
    where
        F: Fn(f64, u64, u64) + Send + 'static + Sync,
    {
        self.progress_callback = Some(Arc::new(callback));
    }

    pub async fn start(self) -> Result<(), Error> {
        let mut all_successful = true;
        let total_files = self.files_to_download.len() as u64;
        let mut current_mut = 0;
        let downloads_finished_callback = self.downloads_finished_callback;
        let download_item_finished_callback = self.download_item_finished_callback;
        let progress_callback = self.progress_callback;
        let download_tasks = self.files_to_download.into_iter().map(|(url, output_path)| {
            let client = &self.client;
            let url = url;
            current_mut += 1;
            let current = current_mut;
            let output_path = output_path;
            let download_item_finished_callback = &download_item_finished_callback;
            let progress_callback = &progress_callback;
            async move {
                let result = Self::download_file(&client.clone(), &url, &output_path, &progress_callback).await;
                if let Some(callback) = &download_item_finished_callback {
                    callback(current, total_files);
                }
                result.is_ok()
            }
        }).collect::<Vec<_>>();
        let stream = futures::stream::iter(download_tasks).buffer_unordered(50);
        let results = stream.collect::<Vec<_>>().await;
        for success in results {
            if !success {
                all_successful = false;
                break;
            }
        }
        println!("Downloaded total files: {}", total_files);
        if let Some(finished_callback) = downloads_finished_callback {
            finished_callback(all_successful);
        }
        Ok(())
    }

    async fn download_file(client: &Client, url: &str, output_path: &str, progress_callback: &Option<Arc<dyn Fn(f64, u64, u64) + Send + Sync>>) -> Result<(), Error> {
        let response = client.get(url).send().await?;
        let total_size = response.content_length().unwrap_or(0);
        let file = OpenOptions::new()
            .create(true)
            .write(true)
            .open(output_path).unwrap();
        let mut buf_writer = BufWriter::new(file);
        let mut stream = response.bytes_stream();
        let mut downloaded_bytes: u64 = 0;
        let start_time = Instant::now();
        let mut last_report_time = Instant::now();
        let mut last_reported_bytes = 0;
        while let Some(chunk) = stream.next().await {
            let bytes = chunk?;
            let chunk_len = bytes.len() as u64;
            buf_writer.write_all(&bytes).unwrap();
            downloaded_bytes += chunk_len;
            let now = Instant::now();
            let elapsed_since_last_report = now - last_report_time;
            if elapsed_since_last_report.as_millis() >= 500 || downloaded_bytes - last_reported_bytes >= 1_000_000 {
                let elapsed_since_start = now - start_time;
                let elapsed_since_start_secs = elapsed_since_start.as_secs_f64();
                let avg_speed = if elapsed_since_start_secs > 0.0 {
                    (downloaded_bytes as f64) / elapsed_since_start_secs / (1024.0 * 1024.0)
                } else {
                    0.0
                };
                if let Some(callback) = progress_callback {
                    callback(avg_speed, downloaded_bytes, total_size);
                }
                last_report_time = now;
                last_reported_bytes = downloaded_bytes;
            }
        }
        Ok(())
    }
}*/

use futures::stream::StreamExt;
use reqwest::{Client, Error};
use std::fs::OpenOptions;
use std::io::Write;
use std::time::Instant;

pub struct Downloader {
    client: Client,
    files_to_download: Vec<(String, String)>,
    downloads_finished_callback: Box<dyn Fn(bool) + Send + Sync>,
    download_item_finished_callback: Box<dyn Fn(u64, u64) + Send + Sync>,
    progress_callback: Box<dyn Fn(f64, u64, u64) + Send + Sync>,
}

impl Downloader {
    pub fn new<F, G, T>(callback_finished: F, callback_item: G, callback_progress: T) -> Self
    where
        F: Fn(bool) + Send + Sync + 'static,
        G: Fn(u64, u64) + Send + Sync + 'static,
        T: Fn(f64, u64, u64) + Send + Sync + 'static
    {
        Downloader {
            client: Client::new(),
            files_to_download: Vec::new(),
            downloads_finished_callback: Box::new(callback_finished),
            download_item_finished_callback: Box::new(callback_item),
            progress_callback: Box::new(callback_progress),
        }
    }

    pub fn download(&mut self, url: &str, output_path: &str) {
        self.files_to_download.push((url.to_string(), output_path.to_string()));
    }

    pub async fn start(self) -> Result<(), Error> {
        let mut all_successful = true;
        let total_files = self.files_to_download.len() as u64;
        let mut current_mut = 0;
        let downloads_finished_callback = self.downloads_finished_callback;
        let download_item_finished_callback = self.download_item_finished_callback;
        let progress_callback = self.progress_callback;

        for (url, output_path) in self.files_to_download {
            current_mut += 1;
            let current = current_mut;
            let result = Self::download_file(
                &self.client,
                url,
                output_path,
                &progress_callback,
            ).await;

            download_item_finished_callback(current, total_files);
            if result.is_err() {
                all_successful = false;
            }
        }

        downloads_finished_callback(all_successful);
        Ok(())
    }

    async fn download_file(
        client: &Client,
        url: String,
        output_path: String,
        progress_callback: &Box<dyn Fn(f64, u64, u64) + Send + Sync>,
    ) -> Result<(), Error> {
        let response = client.get(url).send().await?;
        let total_size = response.content_length().unwrap_or(0);
        let mut file = OpenOptions::new()
            .create(true)
            .write(true)
            .open(output_path)
            .unwrap();
        let mut stream = response.bytes_stream();
        let mut downloaded_bytes: u64 = 0;
        let start_time = Instant::now();
        let mut last_report_time = Instant::now();
        let mut last_reported_bytes = 0;

        while let Some(chunk) = stream.next().await {
            let bytes = chunk?;
            let chunk_len = bytes.len() as u64;
            file.write_all(&bytes).unwrap();
            downloaded_bytes += chunk_len;

            let now = Instant::now();
            let elapsed_since_last_report = now - last_report_time;

            if elapsed_since_last_report.as_millis() >= 100 || downloaded_bytes - last_reported_bytes >= 100_000 {
                let elapsed_since_start = now - start_time;
                let elapsed_since_start_secs = elapsed_since_start.as_secs_f64();
                let avg_speed = if elapsed_since_start_secs > 0.0 {
                    (downloaded_bytes as f64) / elapsed_since_start_secs / (1024.0 * 1024.0)
                } else {
                    0.0
                };
                progress_callback(avg_speed, downloaded_bytes, total_size);
                last_report_time = now;
                last_reported_bytes = downloaded_bytes;
            }
        }

        Ok(())
    }
}
