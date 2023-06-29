use std::io::{self, Write};
use std::str::FromStr;
use std::sync::{
    atomic::{AtomicBool, Ordering},
    Arc,
};
use std::time::Duration;

use crate::core::board::Board;
use crate::engine::search::Params;

pub struct Client {}

lazy_static! {
    pub static ref DEBUG: Arc<AtomicBool> = Arc::new(AtomicBool::new(false));
}

impl Client {
    pub fn new() -> Self {
        Self {}
    }

    pub fn run(&self) -> io::Result<()> {
        let mut board = Board::new();
        let stdin = io::stdin();
        let mut buffer = String::new();

        while stdin.read_line(&mut buffer)? != 0 {
            let mut tokens = buffer.split_ascii_whitespace();
            let command = tokens.next().unwrap_or("");

            match command {
                "ugmi" => self.display_ugmi()?,
                "d" => println!("{}", board),
                "debug" => self.select_debug(tokens)?,
                "isready" => self.display_readyok()?,
                "setoption" => (),
                "ugminewgame" => (),
                "position" => self.set_position(&mut board, tokens)?,
                "go" => self.new_search(tokens)?,
                "stop" => self.stop_search(),
                "quit" => break,
                _ => println!("info string Error: unknown command '{}'", command),
            }

            buffer.clear();
        }

        Ok(())
    }

    fn display_ugmi(&self) -> io::Result<()> {
        println!("id name Kinko v{}", env!("CARGO_PKG_VERSION"));
        println!("id author Morgan Houppin");
        println!("ugmiok");
        io::stdout().flush()
    }

    fn display_readyok(&self) -> io::Result<()> {
        println!("readyok");
        io::stdout().flush()
    }

    fn select_debug<'a, I>(&self, mut tokens: I) -> io::Result<()>
    where
        I: Iterator<Item = &'a str>,
    {
        let debug_mode = tokens.next().unwrap_or("");

        match debug_mode {
            "on" => DEBUG.store(true, Ordering::Relaxed),
            "off" => DEBUG.store(false, Ordering::Relaxed),
            _ => {
                println!("info string Error: Unknown debug mode '{}'", debug_mode);
                io::stdout().flush()?;
            }
        }

        Ok(())
    }

    fn set_position<'a, I>(&self, board: &mut Board, mut tokens: I) -> io::Result<()>
    where
        I: Iterator<Item = &'a str>,
    {
        match tokens.next() {
            Some("startpos") => board.reset(),
            Some("board") => {
                board.setup_position(
                    tokens
                        .next()
                        .ok_or_else(|| io::Error::from(io::ErrorKind::InvalidInput))?,
                    tokens
                        .next()
                        .ok_or_else(|| io::Error::from(io::ErrorKind::InvalidInput))?,
                );
            }
            _ => (),
        }

        let _ = tokens.next();

        for sq in tokens {
            board.push(
                sq.parse()
                    .map_err(|_| io::Error::from(io::ErrorKind::InvalidInput))?,
            )
        }

        Ok(())
    }

    fn parse_millis(&self, token: Option<&str>) -> Option<Duration> {
        if let Some(token_str) = token {
            token_str.parse::<u64>().ok().map(|v| Duration::from_millis(v))
        } else {
            None
        }
    }

    fn new_search<'a, I>(&self, mut tokens: I) -> io::Result<()>
    where
        I: Iterator<Item = &'a str>,
    {
        let mut params = Params::new();

        match tokens.next() {
            Some("wtime") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_wtime(value);
                }
            },
            Some("btime") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_btime(value);
                }
            },
            Some("winc") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_winc(value);
                }
            },
            Some("binc") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_binc(value);
                }
            },
            Some("movetime") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_movetime(value);
                }
            },
            _ => (),
        }
        Ok(())
    }

    fn stop_search(&self) {}
}
