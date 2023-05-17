from srcs.utils.constants import WINDOW_HEIGHT, WINDOW_WIDTH, BOARD_HEIGHT, BOARD_WIDTH


def calculate_block_size() -> int:
    block_height = int(WINDOW_HEIGHT * 0.9 / (BOARD_WIDTH - 1))
    block_width = int(WINDOW_WIDTH * 0.9 / (BOARD_HEIGHT - 1))
    return min(block_width, block_height)


def calculate_renaining_size(block_size) -> tuple[int, int]:
    remaining_height = WINDOW_HEIGHT - block_size * (BOARD_WIDTH - 1)
    remaining_width = WINDOW_WIDTH - block_size * (BOARD_WIDTH - 1)

    return (remaining_width, remaining_height)
