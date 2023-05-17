from abc import ABC, abstractmethod
from srcs.models.coordinate import Coordinate
from srcs.models.move import Move


class Player(ABC):
    @classmethod
    def __init_subclass__(cls):
        """
        check that provider_name has been implemented on class initialization
        """
        if not issubclass(cls, Player):
            raise NotImplementedError()

    @abstractmethod
    def play(self, board: list[list[int]], history: list[Move]) -> Coordinate:
        raise NotImplementedError()
