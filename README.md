[简体中文](https://github.com/ic005k/PlistEDPlus/blob/main/README-en.md) | [English](https://github.com/ic005k/PlistEDPlus/blob/main/README.md)
# PlistEDPlus - open source cross-platform lightweight Plist file editor

| [Release][release-link]|[Download][download-link]|[Issues][issues-link]|[Discourse][discourse-link]|
|-----------------|-----------------|-----------------|-----------------|
|[![release-badge](https://img.shields.io/github/release/ic005k/PlistEDPlus.svg?style=flat-square "Release status")](https://github.com/ic005k/PlistEDPlus/releases "Release status") | [![download-badge](https://img.shields.io/github/downloads/ic005k/PlistEDPlus/total.svg?style=flat-square "Download status")](https://github.com/ic005k/PlistEDPlus/releases/latest "Download status")|[![issues-badge](https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues")](https://github.com/ic005k/PlistEDPlus/issues "Issues")|[![discourse](https://img.shields.io/badge/forum-discourse-orange.svg)](https://www.insanelymac.com/forum/topic/345512-open-source-cross-platform-plist-file-editor-plistedplus/)|

[download-link]: https://github.com/ic005k/PlistEDPlus/releases/latest "Download status"
[download-badge]: https://img.shields.io/github/downloads/ic005k/PlistEDPlus/total.svg?style=flat-square "Download status"

[release-link]: https://github.com/ic005k/PlistEDPlus/releases "Release status"
[release-badge]: https://img.shields.io/github/release/ic005k/PlistEDPlus.svg?style=flat-square "Release status"

[issues-link]: https://github.com/ic005k/PlistEDPlus/issues "Issues"
[issues-badge]: https://img.shields.io/badge/github-issues-red.svg?maxAge=60 "Issues"

[discourse-link]: https://www.insanelymac.com/forum/topic/345512-open-source-cross-platform-plist-file-editor-plistedplus/

Note on file names：

PlistEDPlus_macos-10.15_5.9.9.dmg    For OSX 10.12 and below

PlistEDPlus_macos-10.15_5.15.2.dmg   For OSX 10.13 and above



Write in front: The prototype of this software is from: https://github.com/alpex92/PlistED , thanks to this prototype, not to let me start coding from zero, thanks to @alpex92!

When writing this software, actually some hesitation, similar software seems to be quite a lot, plus the relationship of time, has not paid substantial action. But the idea of writing a Plist editor has been lingering, so it finally led to PlistEDPlus...

## New: Add aml, kext, efi files for OpenCore. The operation is very simple, take ACPI as an example: select Add under ACPI, drag and drop one or more aml files to the app window.

## Features include but are not limited to.

* Lightweight and high performance

* Automatic detection of OpenCore configuration files with corresponding operation prompts

* Support multi-select operations (including copy, cut, delete, etc.)

* Open source, cross-platform (currently supports win, mac, linux)

* Humanized search function (a. Keyboard initial character search for each column of key, data type, and value within the expanded node range; b. Search box global search, one-time display of search results)

* Fully comply with and conform to the technical specifications of plist files, data types are displayed exactly the same as plist text, without any escaping

* Multi-tab view or edit files

* Multi-language support (currently supports Chinese, Russian, English)

* Drag and drop files to the software interface to open files, double-click to open files, file open history display, automatically load the last opened file

* Add, remove, paste, cut, edit entries, etc., all included in Undo, Redo policy group, support undo and restore

* Expand and collapse entries right-click menu, in different columns, with different effects

* For values of data type, the hover window automatically displays the corresponding ASCII and Base64 values

* When changing the data type, the value is automatically converted with it

* Free to select the children under a node for A->Z sorting (currently no sorting of arrays)

* Drag and drop to adjust the order of sibling items, up or down button to adjust the order of sibling items

* Rich shortcut keys

* Built-in update checking

* If the currently opened file is modified by other programs, it automatically prompts whether to reload

* Software UI adapts to various resolutions, supports winpe, linux livecd, etc.

...

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p1.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p2.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p3.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p4.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p5.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p6.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p7.png)

![Screenshot](https://github.com/ic005k/PlistEDPlus/blob/main/p8.png)

## Discussion forum

[insanelymac](https://www.insanelymac.com/forum/topic/345512-open-source-cross-platform-plist-file-editor-plistedplus/)

### Note: This project started in October 2020
