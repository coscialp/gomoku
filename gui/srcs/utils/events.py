import pygame


class EventFlags:
    def __init__(self):
        self.events = {}

    def add_event(self, event_type):
        self.events[event_type] = False

    def update_event(self, event_type, state):
        self.events[event_type] = state

    def get_event_state(self, event_type):
        return self.events[event_type]
