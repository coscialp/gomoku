"""GUI commands"""
import click
import pygame_menu

from srcs.builders.game_builder import GameBuilder
from srcs.utils.constants import WINDOW_HEIGHT, WINDOW_WIDTH
from srcs.builders import AppBuilder
from srcs.models import App, Game


@click.group()
def gui() -> None:
    """GUI commands"""


def build_game_only_players(menu: pygame_menu.Menu, app: App) -> None:
    """Build a game with only players"""
    print("Run Player vs Player")
    game_builder: GameBuilder = GameBuilder()
    game: Game = (
        game_builder.set_game_time(1000 * 60 * 10)
        .set_turn_time(1000 * 60)
        .set_players_only_humans()
        .build()
    )

    app.set_game(game)
    menu.disable()


def build_game_only_ai(menu: pygame_menu.Menu, app: App) -> None:
    """Build a game with only AI"""
    print("Run AI vs AI")
    game_builder: GameBuilder = GameBuilder()
    game: Game = (
        game_builder.set_game_time(1000 * 60 * 10)
        .set_turn_time(1000 * 60)
        .set_players_only_ai()
        .build()
    )

    app.set_game(game)
    menu.disable()


def build_game_both_ai_player(menu: pygame_menu.Menu, app: App) -> None:
    """Build a game with both AI and player"""
    print("Run AI vs Player")
    game_builder: GameBuilder = GameBuilder()
    game: Game = (
        game_builder.set_game_time(1000 * 60 * 10)
        .set_turn_time(1000 * 60)
        .set_players_both_ai_human()
        .build()
    )

    app.set_game(game)
    menu.disable()


@gui.command()
def run() -> None:
    """Run the GUI"""
    app_builder: AppBuilder = AppBuilder()

    app: App = (
        app_builder.set_windows_size(WINDOW_WIDTH, WINDOW_HEIGHT)
        .set_menu_title("Gomoku")
        .set_dark_theme()
        .add_menu_button("1 vs 1", build_game_only_players)
        .add_menu_button("1 vs AI", build_game_both_ai_player)
        .add_menu_button("AI vs AI", build_game_only_ai)
        .add_menu_quit_button()
        .build()
    )

    app.run()


if __name__ == "__main__":
    gui()
