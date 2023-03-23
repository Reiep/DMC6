# DMC6
Alternative firmware for the Disaster Area Design DMC6 Gen 1. For my personal usage, I cannot guarantee it'll work for your setup.

## Features
- 20 presets
- 2 main banks reached by holding the top right button.
- 3 pages by bank reached by pressing the top right button.
  - The first 2 pages (top right LED in color) have 5 presets.
  - The last page allows for 4 CC (1 per button) and 1 PC (bottom right), common for both banks.
- The CCs are limited by design:
  - Hard coded to control a Disaster Area Design SmartLOOP.
  - The status is not carried on between presets: first click activate the matching loop, second one deactivates it, and so on. You need to click twice to deactivate a loop activated in a preset.

## How to use it
- Install the Bounce2 library
- Select the Duomillanove card in Arduino IDE
- Enjoy!
