use crate::core::{types::Square, board::Board};
use std::time::{Duration, Instant};

pub struct Params {
    searchmoves: Option<Vec<Square>>,
    wtime: Option<Duration>,
    btime: Option<Duration>,
    winc: Option<Duration>,
    binc: Option<Duration>,
    depth: Option<u16>,
    nodes: Option<u64>,
    mate: Option<u16>,
    movetime: Option<Duration>,
}

impl Params {
    pub fn new() -> Self {
        Self {
            searchmoves:  None,
            wtime: None,
            btime: None,
            winc: None,
            binc: None,
            depth: None, 
            nodes: None,
            mate: None,
            movetime: None,
        }
    }
}

pub struct SearchData {
    start: Instant,
    node_count: u64,
    iter_depth: u16,
    seldepth: u16,
}

pub fn run_search(board: &Board, params: &Params) {
}