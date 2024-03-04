use gilrs::{Button, Event, Gilrs};
use std::{thread, time};

static mut ACTIVE_GAMEPAD: Option<gilrs::GamepadId> = None;

pub fn init_gamepad() {
    thread::Builder::new()
        .name("lazap_gamepad".to_string())
        .spawn(move || {
            let mut gilrs = Gilrs::new().unwrap();

            for (_id, gamepad) in gilrs.gamepads() {
                println!("{} is {:?}", gamepad.name(), gamepad.power_info());
            }


            loop {
                while let Some(Event { id, event: _, time: _ }) = gilrs.next_event() {
                    unsafe {
                        ACTIVE_GAMEPAD = Some(id);
                    }
                }
            }
        })
        .expect("Failed to spawn thread.");
}

#[tauri::command]
pub async fn catch_gamepad_keys() -> Result<(), Error> {
    let gilrs = Gilrs::new().unwrap();

    thread::sleep(time::Duration::from_millis(1000));

    unsafe {
        loop {
            if let Some(gamepad) = ACTIVE_GAMEPAD.map(|id| gilrs.gamepad(id)) {
                if gamepad.is_pressed(Button::DPadDown) {
                    println!("Down");
                } else if gamepad.is_pressed(Button::DPadUp) {
                    println!("Up");
                } else if gamepad.is_pressed(Button::DPadLeft) {
                    println!("Left");
                } else if gamepad.is_pressed(Button::DPadRight) {
                    println!("Right");
                }
            }
        }
    }
}

#[derive(Debug, thiserror::Error)]
pub enum Error {
    #[error(transparent)]
    Io(#[from] html_parser::Error),
}

impl serde::Serialize for Error {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: serde::ser::Serializer,
    {
        serializer.serialize_str(self.to_string().as_ref())
    }
}
