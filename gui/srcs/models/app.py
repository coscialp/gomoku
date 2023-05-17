""" This module contains the class app"""

import threading
from typing import Callable, Optional
import pygame
import pygame_menu

from srcs.utils.events import EventFlags
from srcs.utils.math import calculate_block_size, calculate_renaining_size
from srcs.models.game import Game, GameAlreadySetError
from srcs.builders.menu_builder import MenuBuilder

from srcs.utils.constants import (
    BLACK,
    BOARD_HEIGHT,
    BOARD_WIDTH,
    WHITE,
    WINDOW_HEIGHT,
    WINDOW_WIDTH,
)
from srcs.models import Coordinate, Menu

event_flags = EventFlags()
event_flags.add_event(pygame.QUIT)
event_flags.add_event(pygame.MOUSEBUTTONDOWN)
event_flags.add_event(pygame.KEYDOWN)


class App:
    """Class App is the main class of the app"""

    is_running: bool

    __screen: pygame.Surface
    __windows_size: Coordinate
    __has_menu: bool
    __menu_builder: MenuBuilder
    __menu: Menu
    __theme: pygame_menu.themes.Theme
    __game: Optional[Game]

    def __init__(self) -> None:
        pygame.init()
        self.is_running = False
        self.__windows_size = Coordinate(x=WINDOW_WIDTH, y=WINDOW_HEIGHT)
        self.__menu_builder = MenuBuilder()
        self.__has_menu = False
        self.__theme = pygame_menu.themes.THEME_DEFAULT
        self.__game = None

    def set_windows_size(self, width: int, height: int) -> None:
        """Set the windows_size attribute"""
        self.__windows_size = Coordinate(x=width, y=height)

    def get_windows_width(self) -> int:
        """Get the windows width"""
        return self.__windows_size.x

    def get_windows_height(self) -> int:
        """Get the windows height"""
        return self.__windows_size.y

    def get_windows_size(self) -> tuple[int, int]:
        """Get the windows size"""
        return self.__windows_size.to_tuple()

    def init_menu(self) -> None:
        """Initialize the menu if does not exist, else do nothing"""
        if self.__has_menu is False:
            self.__has_menu = True

    def set_menu_title(self, title: str) -> None:
        """Set the menu_title attribute"""
        self.__menu_builder.set_title(title)

    def set_dark_theme(self) -> None:
        """Set the menu_dark_theme attribute"""
        self.__theme = pygame_menu.themes.THEME_DARK
        self.__menu_builder.set_dark_theme()

    def set_light_theme(self) -> None:
        """Set the menu_light_theme attribute"""
        self.__theme = pygame_menu.themes.THEME_DEFAULT
        self.__menu_builder.set_light_theme()

    def set_game(self, game: Game) -> None:
        """Set the game attribute"""
        if self.__game:
            raise GameAlreadySetError("Game is already set")
        self.__game = game

    def add_menu_button(self, name: str, callback: Callable) -> None:
        """Add a button to the menu"""
        self.__menu_builder.add_button(name=name, callback=callback)

    def add_menu_quit_button(self) -> None:
        """Add a quit button to the menu"""
        self.__menu_builder.add_quit_button()

    def __draw_grid(self) -> None:
        block_size = calculate_block_size()
        remaining_width, remaining_height = calculate_renaining_size(block_size)

        self.__screen.fill(self.__theme.background_color)
        large_rect: pygame.Rect = pygame.Rect(
            remaining_width / 2 - 1,
            remaining_height / 2 - 1,
            block_size * (BOARD_WIDTH - 1) + 2,
            block_size * (BOARD_HEIGHT - 1) + 2,
        )
        for x in range(BOARD_WIDTH - 1):
            for y in range(BOARD_HEIGHT - 1):
                rect = pygame.Rect(
                    x * block_size + remaining_width / 2,
                    y * block_size + remaining_height / 2,
                    block_size,
                    block_size,
                )
                pygame.draw.rect(self.__screen, self.__theme.widget_font_color, rect, 1)
        pygame.draw.rect(self.__screen, self.__theme.widget_font_color, large_rect, 1)

    def __draw_stones(self) -> None:
        block_size = calculate_block_size()
        remaining_width, remaining_height = calculate_renaining_size(block_size)

        for x in range(BOARD_WIDTH):
            for y in range(BOARD_HEIGHT):
                if self.__game.get_cell(x, y) != 0:
                    coordinate: Coordinate = Coordinate(
                        x=x * block_size + remaining_width / 2,
                        y=y * block_size + remaining_height / 2,
                    )
                    pygame.draw.circle(
                        self.__screen,
                        BLACK if self.__game.get_cell(x, y) == 1 else WHITE,
                        coordinate.to_tuple(),
                        block_size / 2,
                    )

    def __render(self) -> None:
        """Render the application"""
        if self.__menu.is_enabled():
            self.__menu.render()

        if self.__game:
            self.__draw_grid()
            self.__draw_stones()
        pygame.display.update()

    def run(self) -> None:
        """Run the application"""
        self.__screen = pygame.display.set_mode(size=self.get_windows_size())
        clock: pygame.time.Clock = pygame.time.Clock()
        dt: float = 0

        if self.__has_menu:
            self.__menu_builder.set_size(self.__windows_size.x, self.__windows_size.y)
            self.__menu_builder.set_surface(self.__screen)
            self.__menu = self.__menu_builder.build(self)

        self.is_running = True
        while self.is_running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    event_flags.update_event(pygame.QUIT, True)
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    event_flags.update_event(pygame.MOUSEBUTTONDOWN, True)
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    event_flags.update_event(pygame.KEYDOWN, True)

            self.__render()

            if self.__game:
                try:
                    self.__game.launch_turn()
                except Exception:
                    self.__game = None
                    self.__menu.enable()

            dt = clock.tick(60) / 1000
        pygame.quit()
