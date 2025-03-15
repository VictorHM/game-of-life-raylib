# Changelog

## [Unreleased]

### Added
- Implemented initial game setup and window creation
- Added cell grid initialization and rendering
- Introduced basic game loop with cell state updates
- Add screen transitions and menus for screens.
- Add example config for testing loading configs.
- Add cicle's counter to slow down simulation. Allows to be visible.
- Add functionality that toggle music on/off pressing "M".

### Changed
- Refactored cell update logic for better performance
- Improved rendering efficiency by reducing redundant draw calls
- Change colors to be more pleasent. Colors and sizes are consistent now.

### Fixed
- Corrected issue with incorrect cell state transitions
- Fix initial testing configuration to start with correct "survival" config.
- Fix pause to work correctly. Mutes Music too.
- Fix design and visuals for Options to show correctly 3 options, proper size and distance.

## [0.1.0] - 2023-10-05

### Major Features
- [ x ] Implemented Selection of two set of rules in Options
- [ x ] Deleted red user square. No clear use for now.

### Minor Features

### Bug fixes
- [ x ] Fixed path to load resources to avoid missing sounds and character sets.

### Added
- Initial release of Game of Life with basic functionality
- Implemented user controls for starting, stopping, and resetting the simulation

### Changed
- N/A

### Fixed
- N/A