import pygame

from srcs.models.coordinate import Coordinate


class Game:
    screen: pygame.Surface
    running: bool
    windows_size: Coordinate

    def __init__(self, windows_size: Coordinate = Coordinate(x=1080, y=720)) -> None:
        pygame.init()
        self.screen = pygame.display.set_mode(size=windows_size.to_tuple())
        self.running = True

    def run(self) -> None:
        while self.running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
        pygame.quit()
