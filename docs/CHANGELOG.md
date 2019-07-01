# Changelog
All notable changes to this project will be documented here.

Design and format of this file are based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).\
Project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] - Upcoming
*Changelog for this version may be still changed*
### Changed
- Commit messages semantics (thanks for idea [@PsychoX](https://github.com/PsychoXIVI)).
- Update "Triangle" example.
### Fix
-  "Setting uniforms not work for glm::vec variables" (thanks for noticing problem and little help templates stuff [@MetGang](https://github.com/metgang)).

## [v0.0.4] - 2019-07-01
### Added
- VAO, VBO and EBO support.
- Loading and applying shaders.
- Setting shader uniforms (support for single variables and vecN types).
- Main loop setup.
- Overloadable callbacks for input, update and render.
- Method for setting custom window size.
- Setting level of anty-aliasing.
- All available OpenGL primitive types for render.
- Error logs support with build-in logger.
- This CHANGELOG.md file.

[Unreleased]: https://github.com/MetRiko/OpenGL-Tests/compare/v0.0.4...HEAD
[v0.0.4]: https://github.com/MetRiko/OpenGL-Tests/releases/tag/v0.0.4