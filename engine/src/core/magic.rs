use std::str::FromStr;

use super::types::Stone;

#[derive(Clone, Copy, Debug)]
pub enum Alignment {
    NoAlign,
    Three,
    OpenThree,
    Four,
    OpenFour,
    Five,
}

#[derive(Clone, Copy, Debug)]
pub struct OwnedAlignment {
    align: Alignment,
    owned: bool,
}

#[derive(Clone, Copy)]
pub enum CrossAlignment {
    NoAlign,
    Three,
    OpenThree,
    Four,
    OpenFour,
    DoubleThree,
    DoubleOpenThree,
    FourThree,
    OpenFourThree,
    FourFour,
    Five,
}

#[derive(Clone, Copy)]
pub struct SquareAlignment {
    cross_align: CrossAlignment,
    owned: bool,
}

struct StonePattern(Vec<Stone>);

impl StonePattern {
    fn into_inner(self) -> Vec<Stone> {
        self.0
    }
}

#[derive(Debug)]
pub struct PatternError(());

impl FromStr for StonePattern {
    type Err = PatternError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        Ok(StonePattern(
            s.as_bytes()
                .iter()
                .map(|b| match b {
                    46 => Ok(Stone::Empty),
                    120 => Ok(Stone::Black),
                    111 => Ok(Stone::White),
                    _ => Err(PatternError(())),
                })
                .collect::<Result<Vec<_>, PatternError>>()?,
        ))
    }
}

lazy_static! {
    pub static ref MAGIC_STRUCT: [OwnedAlignment; 0x10000] =
        build_magic().expect("Failed to generate magic struct");
}

pub fn build_magic() -> Result<[OwnedAlignment; 0x10000], PatternError> {
    let mut magic = [OwnedAlignment {
        align: Alignment::NoAlign,
        owned: false,
    }; 0x10000];

    let mut count = 0;

    count += magic_init_pattern(
        &mut magic,
        &"xxxxx".parse::<StonePattern>()?.into_inner(),
        Alignment::Five,
    );

    count += magic_init_pattern(
        &mut magic,
        &".xxxx.".parse::<StonePattern>()?.into_inner(),
        Alignment::OpenFour,
    );

    count += magic_init_pattern(
        &mut magic,
        &"oxxxx.".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );
    count += magic_init_pattern(
        &mut magic,
        &".xxxxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );
    count += magic_init_pattern(
        &mut magic,
        &"oxxxxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );

    count += magic_init_pattern(
        &mut magic,
        &".xxx.".parse::<StonePattern>()?.into_inner(),
        Alignment::OpenThree,
    );

    count += magic_init_pattern(
        &mut magic,
        &"oxxx.".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );
    count += magic_init_pattern(
        &mut magic,
        &".xxxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );
    count += magic_init_pattern(
        &mut magic,
        &"oxxxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );

    count += magic_init_pattern(
        &mut magic,
        &".x.xx.".parse::<StonePattern>()?.into_inner(),
        Alignment::OpenThree,
    );

    count += magic_init_pattern(
        &mut magic,
        &"ox.xx.".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );
    count += magic_init_pattern(
        &mut magic,
        &".x.xxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );
    count += magic_init_pattern(
        &mut magic,
        &"ox.xxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );

    count += magic_init_pattern(
        &mut magic,
        &".xx.x.".parse::<StonePattern>()?.into_inner(),
        Alignment::OpenThree,
    );

    count += magic_init_pattern(
        &mut magic,
        &"oxx.x.".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );
    count += magic_init_pattern(
        &mut magic,
        &".xx.xo".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );
    count += magic_init_pattern(
        &mut magic,
        &"oxx.xo".parse::<StonePattern>()?.into_inner(),
        Alignment::Three,
    );

    count += magic_init_pattern(
        &mut magic,
        &".xx.xx.".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );
    count += magic_init_pattern(
        &mut magic,
        &"oxx.xx.".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );
    count += magic_init_pattern(
        &mut magic,
        &".xx.xxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );
    count += magic_init_pattern(
        &mut magic,
        &"oxx.xxo".parse::<StonePattern>()?.into_inner(),
        Alignment::Four,
    );

    println!("Initialized {}/65536 patterns", count);

    Ok(magic)
}

pub fn magic_init_pattern(
    magic: &mut [OwnedAlignment; 0x10000],
    pattern: &Vec<Stone>,
    align: Alignment,
) -> u16 {
    let mut base = [Stone::Empty; 9];
    let mut count = 0;

    base[9 - pattern.len()..].copy_from_slice(pattern.as_slice());

    let range = 3i32.pow(9 - pattern.len() as u32);

    for c in 0..range {
        let mut ci = c;

        for value in base.iter_mut().take(9 - pattern.len()) {
            *value = match ci % 3 {
                0 => Stone::Empty,
                1 => Stone::Black,
                _ => Stone::White,
            };

            ci /= 3;
        }

        let mut shift = base;
        let mut p = 0;

        while shift[4] != Stone::Black {
            let left = shift[0];

            for r in 0..8 {
                shift[r] = shift[r + 1];
            }

            shift[8] = left;
            p += 1;
        }

        magic[stones_to_mask(&shift) as usize] = OwnedAlignment { align, owned: true };
        count += 1;

        println!(
            "Init [{}{}{}{}{}{}{}{}{}] as {:?}, owned",
            shift[0],
            shift[1],
            shift[2],
            shift[3],
            shift[4],
            shift[5],
            shift[6],
            shift[7],
            shift[8],
            align
        );

        for _ in p..9 - pattern.len() {
            let left = shift[0];

            for r in 0..8 {
                shift[r] = shift[r + 1];
            }

            shift[8] = left;

            if shift[4] == Stone::White {
                break;
            }

            if shift[4] == Stone::Empty {
                continue;
            }

            magic[stones_to_mask(&shift) as usize] = OwnedAlignment {
                align,
                owned: false,
            };
            count += 1;

            println!(
                "Init [{}{}{}{}{}{}{}{}{}] as {:?}, not owned",
                shift[0],
                shift[1],
                shift[2],
                shift[3],
                shift[4],
                shift[5],
                shift[6],
                shift[7],
                shift[8],
                align
            );
        }
    }

    count
}

pub fn stones_to_mask(stones: &[Stone; 9]) -> u16 {
    let main_stone = stones[4];

    let stone_to_mask = |v| {
        if v == Stone::Empty {
            0
        } else if v == main_stone {
            1
        } else {
            2
        }
    };

    let mut mask = 0;

    for i in 0..4 {
        mask |= stone_to_mask(stones[5 + i]) << (i * 2);
    }

    for i in 4..8 {
        mask |= stone_to_mask(stones[7 - i]) << (i * 2);
    }

    mask
}
