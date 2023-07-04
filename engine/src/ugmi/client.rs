use std::io::{self, Write};
use std::sync::{
    atomic::{AtomicBool, Ordering},
    Arc,
};
use std::time::Duration;

use crate::core::{board::Board, types::Square};
use crate::engine::search::{self, Params};

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
                "go" => self.new_search(tokens, &board)?,
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
            token_str.parse::<u64>().ok().map(Duration::from_millis)
        } else {
            None
        }
    }

    fn new_search<'a, I>(&self, mut tokens: I, board: &Board) -> io::Result<()>
    where
        I: Iterator<Item = &'a str>,
    {
        let mut params = Params::new();

        match tokens.next() {
            Some("wtime") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_wtime(value);
                }
            }
            Some("btime") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_btime(value);
                }
            }
            Some("winc") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_winc(value);
                }
            }
            Some("binc") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_binc(value);
                }
            }
            Some("movetime") => {
                if let Some(value) = self.parse_millis(tokens.next()) {
                    params.set_movetime(value);
                }
            }
            Some("depth") => {
                if let Ok(value) = tokens.next().unwrap_or("").parse::<u16>() {
                    params.set_depth(value);
                }
            }
            Some("nodes") => {
                if let Ok(value) = tokens.next().unwrap_or("").parse::<u64>() {
                    params.set_nodes(value);
                }
            }
            Some("mate") => {
                if let Ok(value) = tokens.next().unwrap_or("").parse::<u16>() {
                    params.set_mate(value);
                }
            }
            Some("searchmoves") => {
                let mut moves = Vec::new();
                for token in tokens {
                    if let Ok(value) = token.parse::<Square>() {
                        moves.push(value);
                    }
                }
                params.set_searchmoves(moves.as_slice());
            }
            _ => (),
        }
        search::run_search(board, &params)
    }

    fn stop_search(&self) {}
}

impl Default for Client {
    fn default() -> Self {
        Self::new()
    }
}
