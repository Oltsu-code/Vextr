/// @file Vextr.h
/// @brief Main header file for the Vextr TUI framework.
///
/// This file includes all public API headers. Users can include this single header
/// to get access to the entire framework.
///
/// **Library Structure:**
/// - **App**: Main application class for event loop and rendering
/// - **Core**: Fundamental types (Widget, Event, Style, Geometry, Alignment)
/// - **Widgets**: UI components (Button, Label, Input, Container, Navigator,
///   ScrollView)
/// - **Layouts**: Layout engines (StackLayout, GridLayout)

#pragma once

#include <Vextr/App.hpp>

#include <Vextr/core/Widget.hpp>
#include <Vextr/core/Event.hpp>
#include <Vextr/core/Style.hpp>
#include <Vextr/core/Geometry.hpp>

#include <Vextr/widgets/Button.hpp>
#include <Vextr/widgets/Label.hpp>
#include <Vextr/widgets/Input.hpp>
#include <Vextr/widgets/TextView.hpp>
#include <Vextr/widgets/TextArea.hpp>
#include <Vextr/widgets/Container.hpp>
#include <Vextr/widgets/Navigator.hpp>
#include <Vextr/widgets/ScrollView.hpp>

#include <Vextr/layout/StackLayout.hpp>
#include <Vextr/layout/GridLayout.hpp>