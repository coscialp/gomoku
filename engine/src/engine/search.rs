use crate::core::{types::Square, board::Board};
use std::time::{Duration, Instant};
use std::sync::{
    atomic::AtomicBool,
    Arc,
};

lazy_static! {
    pub static ref STOP: Arc<AtomicBool> = Arc::new(AtomicBool::new(true));
}

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

    pub fn set_searchmoves(&mut self, searchmoves: &[Square]) {
        self.searchmoves = Some(Vec::from(searchmoves));
    }

    pub fn set_wtime(&mut self, wtime: Duration) {
        self.wtime = Some(wtime);
    }

    pub fn set_btime(&mut self, btime: Duration) {
        self.btime = Some(btime);
    }

    pub fn set_winc(&mut self, winc: Duration) {
        self.winc = Some(winc);
    }

    pub fn set_binc(&mut self, binc: Duration) {
        self.binc = Some(binc);
    }

    pub fn set_depth(&mut self, depth: u16) {
        self.depth = Some(depth);
    }

    pub fn set_nodes(&mut self, nodes: u64) {
        self.nodes = Some(nodes);
    }

    pub fn set_mate(&mut self, mate: u16) {
        self.mate = Some(mate);
    }

    pub fn set_movetime(&mut self, movetime: Duration) {
        self.movetime = Some(movetime);
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