from time import sleep
import pygame
from srcs.utils.math import calculate_block_size, calculate_renaining_size
from srcs.utils.constants import (
    WINDOW_HEIGHT,
    WINDOW_WIDTH,
    BOARD_HEIGHT,
    BOARD_WIDTH,
)
from srcs.models.move import Move
from srcs.models.coordinate import Coordinate
from srcs.interfaces import Player


class HumanPlayer(Player):
    def play(self, board: list[list[int]], history: list[Move]) -> Coordinate:
        block_size = calculate_block_size()
        remaining_width, remaining_height = calculate_renaining_size(block_size)
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    quit()
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    raise Exception("Back to menu")
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    x, y = pygame.mouse.get_pos()

                    normalized_x = round((x - remaining_width / 2) / block_size)
                    normalized_y = round((y - remaining_height / 2) / block_size)

                    if normalized_x < 0 or normalized_x >= 19:
                        continue
                    elif normalized_y < 0 or normalized_y >= 19:
                        continue

                    return Coordinate(
                        x=normalized_x,
                        y=normalized_y,
                    )
