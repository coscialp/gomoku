import pygame
import pygame_menu

from srcs.utils.constants import (
    BOARD_HEIGHT,
    BOARD_WIDTH,
)
from srcs.utils.exceptions import PlayerAlreadySetException
from srcs.interfaces import Player
from srcs.models import Move


class GameAlreadySetError(Exception):
    def __init__(self, message: str = "Game already set") -> None:
        super().__init__(message)


class Game:
    """This class represents a game."""

    __players: tuple[Player, Player] | None
    __current_player: Player
    __is_finished: bool
    __board: list[list[int]]
    __histories: list[Move]
    __turn_count: int
    __turn_time: int
    __game_time: int

    def __init__(self) -> None:
        self.__histories = []
        self.__board = [[0 for _ in range(BOARD_WIDTH)] for _ in range(BOARD_HEIGHT)]
        self.__players = None
        self.__is_finished = False
        self.__turn_count = 0
        self.__turn_time = 1000 * 60
        self.__game_time = 1000 * 60 * 10

    def set_turn_time(self, turn_time: int) -> None:
        self.__turn_time = turn_time

    def set_game_time(self, game_time: int) -> None:
        self.__game_time = game_time

    def set_players(self, player1: Player, player2: Player) -> None:
        if self.__players:
            raise PlayerAlreadySetException()
        self.__players = (player1, player2)
        self.__current_player = player1

    def get_cell(self, x: int, y: int) -> int:
        return self.__board[x][y]

    def launch_turn(self) -> None:
        if self.__is_finished is False:
            self.__current_player = self.__players[self.__turn_count % 2]
            self.__turn_count += 1
            stone_coordinate = None
            while stone_coordinate is None:
                stone_coordinate = self.__current_player.play(
                    self.__board, self.__histories
                )

                if self.__board[stone_coordinate.x][stone_coordinate.y] != 0:
                    stone_coordinate = None

            self.__board[stone_coordinate.x][stone_coordinate.y] = (
                1 if self.__turn_count % 2 else 2
            )
            self.__histories.append(Move(last=stone_coordinate))
