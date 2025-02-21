use std::any::Any;
use std::collections::BTreeMap;
use std::fmt::Display;
use std::path::PathBuf;
use eframe::{egui, egui_glow, EventLoopBuilderHook, Frame, HardwareAcceleration, NativeOptions, Renderer, Storage, WindowBuilderHook};
use egui::{CentralPanel, Context, Ui, ViewportBuilder};
use egui::ViewportCommand::Close;

/// Example below:
/// ```
/// Window::default()
///     .set_central_panel(move |ctx, ui, frame| {
///         egui::CentralPanel::default().show(ctx, |ui| {
///
///         });
///     })
///     .open().unwrap()
/// ```
pub struct Window {
    title: String, close_window: bool,
    central_panel_response: Option<Box<dyn FnMut(&Context, &mut Ui, &mut Frame, &mut BTreeMap<String, Box<dyn Any>>)>>,
    on_close: Option<Box<dyn FnMut(&BTreeMap<String, Box<dyn Any>>)>>,
    options: NativeOptions,
    values: BTreeMap<String, Box<dyn Any>>
}

pub struct ValueEntry {
    pub key: String,
    pub value: Box<dyn Any>
}

impl ValueEntry {
    pub fn from(key: String, value: Box<dyn Any>) -> Self {
        ValueEntry { key, value }
    }
}

impl eframe::App for Window {
    fn update(&mut self, ctx: &Context, frame: &mut Frame) {
        if self.close_window {
            ctx.send_viewport_cmd(Close);
        }
        CentralPanel::default().show(ctx, |ui| {
            if let Some(ref mut response_ui_func) = self.central_panel_response {
                response_ui_func(ctx, ui, frame, &mut self.values);
            }
        });
    }

    fn on_exit(&mut self, _gl: Option<&eframe::glow::Context>) {
        if let Some(ref mut on_close_func) = self.on_close {
            on_close_func(&self.values);
        }
    }
}

#[allow(dead_code)]
impl Window {
    pub fn viewport(mut self, viewport: ViewportBuilder) -> Self {
        self.options.viewport = viewport;
        self
    }

    pub fn vsync(mut self, vsync: bool) -> Self {
        self.options.vsync = vsync;
        self
    }

    pub fn multisampling(mut self, multisampling: u16) -> Self {
        self.options.multisampling = multisampling;
        self
    }

    pub fn depth_buffer(mut self, depth_buffer: u8) -> Self {
        self.options.depth_buffer = depth_buffer;
        self
    }

    pub fn stencil_buffer(mut self, stencil_buffer: u8) -> Self {
        self.options.stencil_buffer = stencil_buffer;
        self
    }

    pub fn hardware_acceleration(mut self, hardware_acceleration: HardwareAcceleration) -> Self {
        self.options.hardware_acceleration = hardware_acceleration;
        self
    }

    pub fn renderer(mut self, renderer: Renderer) -> Self {
        self.options.renderer = renderer;
        self
    }

    pub fn run_and_return(mut self, run_and_return: bool) -> Self {
        self.options.run_and_return = run_and_return;
        self
    }

    pub fn event_loop_builder(mut self, event_loop_builder: Option<EventLoopBuilderHook>) -> Self {
        self.options.event_loop_builder = event_loop_builder;
        self
    }

    pub fn window_builder(mut self, window_builder: Option<WindowBuilderHook>) -> Self {
        self.options.window_builder = window_builder;
        self
    }

    pub fn shader_version(mut self, shader_version: Option<egui_glow::ShaderVersion>) -> Self {
        self.options.shader_version = shader_version;
        self
    }

    pub fn centered(mut self, centered: bool) -> Self {
        self.options.centered = centered;
        self
    }

    pub fn persist_window(mut self, persist_window: bool) -> Self {
        self.options.persist_window = persist_window;
        self
    }

    pub fn persistence_path(mut self, persistence_path: Option<PathBuf>) -> Self {
        self.options.persistence_path = persistence_path;
        self
    }

    pub fn dithering(mut self, dithering: bool) -> Self {
        self.options.dithering = dithering;
        self
    }

    pub fn title(mut self, title: String) -> Self {
        self.title = title;
        self
    }

    pub fn on_close<F>(mut self, f: F) -> Self
    where
    F: FnMut(&BTreeMap<String, Box<dyn Any>>) + 'static
    {
        self.on_close = Some(Box::new(f));
        self
    }

    pub fn set_central_panel<T>(mut self, central_panel_response: T) -> Self
    where
        T: FnMut(&Context, &mut Ui, &mut Frame, &mut BTreeMap<String, Box<dyn Any>>) + 'static
    {
        self.central_panel_response = Some(Box::new(central_panel_response));
        self
    }

    ///Casting example:
    /// ```
    /// values.get_mut("KEY").unwrap().downcast_mut::<VALUE_TYPE>().unwrap()
    /// ```
    pub fn set_values(mut self, values: Vec<ValueEntry>) -> Self {
        for value in values {
            self.values.insert(value.key, value.value);
        }
        self
    }

    pub fn open(
        self,
    ) -> eframe::Result {
        eframe::run_native(
            self.title.clone().as_str(),
            self.options.clone(),
            Box::new(|cc| Ok(Box::new(self))),
        )
    }

    pub fn values(self) -> BTreeMap<String, Box<dyn Any>> {
        self.values
    }

    pub fn close(mut self) {
        self.close_window = true
    }
}

impl Default for Window {
    fn default() -> Self {
        Self {
            values: BTreeMap::new(),
            on_close: None,
            options: NativeOptions::default(),
            central_panel_response: None,
            close_window: false,
            title: "egui eFrame".to_string()
        }
    }
}