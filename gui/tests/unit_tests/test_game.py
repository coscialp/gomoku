"""Module unit tests for Game"""
import pygame
import pytest

from srcs.models.coordinate import Coordinate
from srcs.game import Game


class TestGame:
    """Test class for Game"""

    def test_construct_game_works(self) -> None:
        """Test if the constructor of Game works"""
        game = Game()
        assert game.running == True
        assert isinstance(game.screen, pygame.Surface)
        assert isinstance(game.windows_size, Coordinate)
