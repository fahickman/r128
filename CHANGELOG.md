# Change Log

## 1.4.4 - 2020/9/12
### Fixed
* Gcc/clang compile warnings
* MINGW compile errors

## 1.4.3 - 2019/7/10
### Fixed
* Fix MSVC ARM64 compile errors
### Changed
* Use MSVC 2019 intrinsics for long division. Produces slightly bettter code.

## 1.4.2 - 2019/5/24
### Fixed
* Fix incorrect numeric_limits::digits10 value.

## 1.4.1 - 2018/5/30
### Fixed
* Fix rare assertion in division.

## 1.4.0 - 2018/2/22
### Added
* R128_STDC_ONLY optional define to exclude Intel intrinsics and inline asm.
### Fixed
* Removed MSVC workaround in r128Sub.

## 1.3.0 - 2017/7/22
### Fixed/Added
* Fix C++ constructor overload resolution issues by adding a new constructor.
* Warning fixes.

## 1.2.2 - 2017/7/4
### Fixed
* Warnings on non-intel platforms
* Another division bug

## 1.2.1 - 2017/7/2
### Fixed
* Return value of r128ToString functions correctly matches snprintf

## 1.2.0 - 2017/6/30
### Added
* R128_last global variable which shows the result of the last operation as a
  string. Enable with #define R128_DEBUG_VIS

### Fixed
* Minor documentation errors in r128ToString functions.

## 1.1.0 - 2017/6/25
### Added
* Square root and reciprocal square root

### Fixed
* Division bugs
