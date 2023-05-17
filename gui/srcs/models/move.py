from typing import Optional
from pydantic import BaseModel, validator

from srcs.utils.constants import BOARD_HEIGHT, BOARD_WIDTH
from srcs.models.coordinate import Coordinate


class Move(BaseModel):
    last: Coordinate
    stone_captured: Optional[tuple[Coordinate, Coordinate]]

    @validator("last")
    def validate_last(cls, value: Coordinate) -> Coordinate:
        """Check if the last coordinate is positive."""
        if value.x > BOARD_WIDTH or value.y > BOARD_HEIGHT:
            raise ValueError("Coordinate must be in the board")
        return value

    @validator("stone_captured")
    def validate_stone_captured(
        cls, value: tuple[Coordinate, Coordinate]
    ) -> tuple[Coordinate, Coordinate]:
        """Check if the stone captured is positive."""
        if value:
            for coordinate in value:
                if coordinate.x > BOARD_WIDTH or coordinate.y > BOARD_HEIGHT:
                    raise ValueError("Coordinate must be in the board")
        return value
