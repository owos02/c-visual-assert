# c-visual-assert (commented-visual-assert)
> [!CAUTION]
> This library is in development. Problems may arise.

A lightweight header-only library for displaying asserts.

<!-- TOC start -->

# Table of Contents
- [Settings](#settings)
   * [Quiet Mode](#quiet-mode)
   * [Collection Mode](#collection-mode)
   * [Bordered Messages](#bordered-messages)
      + [Colors](#colors)
      + [Formatting](#formatting)
- [Usage](#usage)
- [Documentation](#documentation)

<!-- TOC end -->

# Settings

The library offers a selection of settings which can be tweaked.

Color settings are available under `c_visual_assert_::ANSI_Colors::*`

The other settings can be accessed under the following `c_visual_assert_::Settings::*`

Those include...

## Quiet Mode

Disables the visual output of the asserts.   
Default: `false`

## Collection Mode

Collects all asserts within a list which can be displayed.   
Default: `false`

## Bordered Messages

Enable/Disable bordered messages on the list displayed asserts.   
Default: `false`

### Colors

It is possible to change the colors. Due to the nature of terminals you are limited to the ![ANSI escape characters[]()](https://en.wikipedia.org/wiki/ANSI_escape_code#Select_Graphic_Rendition_parameters)

### Formatting

Left/Right Offset are used for changing spacing between fillers.
fillerSymbol is used for the filling character.

Changing colors works as follows:
> `c_visual_assert_::ANSI_Colors::FAILURE = "35"; // Changes the color to Magenta`

# Usage

1. Get a copy of the header file.
2. Include in your project.

That's pretty much it.

# Documentation

> [!IMPORTANT]
> For this library to work you will need to define `DEBUG` (case-sensitive).

Currently has the following macros that can be used.

---

`c_visual_assert(condition, message)`

Makes an assert and outputs the assert and result (colored) onto the default output stream.

> **Parameters**   
> `condition`: must be a expression which returns a `boolean`.   
> `message`:   expects a const char* (e.g. "x should be 2")

---

`c_visual_assert_quiet(condition, message)`

Asserts the condition but shows no output.

> **Parameters**   
> `condition`: must be a expression which returns a `boolean`.   
> `message`:   expects a const char* (e.g. "x should be 2")

---

`c_visual_headline_print(message)`

Prints the message bold and underlined.

---

> [!IMPORTANT]
> To properly use the following macros you will need to define `NDEBUG` inside of your code, also `collectionMode` must be set to `true`.   
> This will disable the asserts but the library will still evaluate the expressions.
> 

---

`c_visual_assert_list_all`

Lists all called asserts (from **this** library) with colored output.

---

`c_visual_assert_list_stats`

Shows some statistics about the asserts (from **this** library) with colored output.
