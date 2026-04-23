[![CI](https://github.com/Oltsu-code/Vextr/actions/workflows/ci.yml/badge.svg)](https://github.com/Oltsu-code/Vextr/actions/workflows/ci.yml)

# Vextr

Vextr is a flexible **widget-based terminal UI framework for modern C++**.

---

## Features

- Cross-platform: Windows and Unix terminals supported
- Flexible layout system: stack, grid, absolute positioning
- Rich styling: RGB colors, bold/underline, padding/margin
- Common widgets: labels, buttons, inputs, scroll views
- Simple API for building complex UIs
- Flexibility oriented
- Events

---

## Installation

### CMake FetchContent
```cmake
include(FetchContent)

FetchContent_Declare(
    vextr
    GIT_REPOSITORY https://github.com/Oltsu-code/Vextr.git
    GIT_TAG main
)

FetchContent_MakeAvailable(vextr)

target_link_libraries(my_app PRIVATE vextr)
```

---

## Quick Start

1. Include the header:
   ```c++
   #include "vextr.h"
   ```

2. Create a root Container:
   ```c++
   auto root = std::make_shared<vextr::core::Container>(
      std::make_shared<vextr::core::StackLayout>(vextr::core::Axis::Vertical, 1)); 
   ```

3. Create a Label widget:
   ```c++
   auto label = std::make_shared<vextr::widgets::Label>("Hello, Vextr!");
   ```

4. create a Style for the Label and apply it:
   ```c++
   vextr::core::Style style = {
      .fg = {54, 9, 217},
      .bold = true
   };
   label->set_style(style);
   ```

5. Add the label to the root container:
   ```c++
   root->add_child(label);
   ```
   
6. Create the application, set the root container, and run:
   ```c++
   vextr::App app;
   app.setRoot(root);
   app.run();
   ```
   
---

## Building from source

1. Clone the repository:
   ```bash
   git clone https://github.com/Oltsu-code/Vextr.git
   cd Vextr
   ```

2. Build with CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. Have fun!

---

## Documentation

You can find the online documentation [here](https:/vextr.oltsu.dev/).
All guides should also be available in the `docs/content/` folder.
The code also has doxygen comments.

This repository uses [Moxygen](https://github.com/sourcey/moxygen) to generate the API reference documentation from Doxygen XML.

**Note:** The documentation is currently a work in progress.

### Building the docs site

1. Install [Moxygen](https://github.com/sourcey/moxygen)
```bash
npm install -g moxygen
```

2. Install dependencies
```bash
cd docs/site
npm instll
```

3. Generate the API References and format the content for astro
```bash
npm run generate
```

4. Start the development server
```bash
npm run dev
```

or build

```bash 
npm run build
```

## Weather App Integration (Ruby)

A full weather app example is available at `examples/weather_example/`.
It uses:

- Vextr (C++) for the terminal UI
- Ruby for weather API integration and parsing
- Open-Meteo API for geocoding + weather data

### Requirements

- Ruby installed and available in `PATH`
- Internet connection

### Build and run

```bash
cmake -S . -B build
cmake --build build --target weather_example
./build/bin/weather_example
```

On Windows use:

```powershell
build\bin\weather_example.exe
```

Inside the app:

- Enter a city name (for example `Helsinki`)
- Press Enter in the input field or activate `Fetch Weather`
- View current conditions and the 5-day forecast

## Roadmap

---

Check the currently planned roadmap [here](ROADMAP.md).

## Contributing

---

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## License

Mozilla Public License Version 2.0. See [LICENSE](LICENSE) for details.


   
