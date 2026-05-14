# Your First App

You'll build a counter app. The app will have two buttons to increment and decrement a number. By the end you'll have touched layout, styling, and events - the three things every Vextr app uses.

## Finished code

```cpp
#include <Vextr/Vextr.hpp>

using namespace vextr::widgets;
using namespace vextr::layout;
using namespace vextr::core;

int main() {
    int count = 0;

    App app;

    auto root = std::make_shared<Container>(
        std::make_shared<StackLayout>(Axis::Vertical, 1)
    );

    auto countLabel = std::make_shared<Label>("0");
    countLabel->setStyle(Style{ .fg = {255, 255, 255}, .text = TextDecoration{ .bold = true } });

    auto incrementBtn = std::make_shared<Button>("+");
    incrementBtn->setStyle(Style{ .bg = {31, 189, 0} });
    incrementBtn->setFocusedStyle(Style{ .bg = {0, 255, 68} });
    incrementBtn->setOnClick([countLabel, &count]() {
        count++;
        countLabel->setText(std::to_string(count));
    });

    auto decrementBtn = std::make_shared<Button>("-");
    decrementBtn->setStyle(Style{ .bg = {133, 0, 11} });
    decrementBtn->setFocusedStyle(Style{ .bg = {255, 0, 21} });
    decrementBtn->setOnClick([countLabel, &count]() {
        count--;
        countLabel->setText(std::to_string(count));
    });

    root->addChild(countLabel);
    root->addChild(incrementBtn);
    root->addChild(decrementBtn);

    app.setRoot(root);
    app.run();

    return 0;
}
```

:::note
This example uses `using namespace` for brevity. In real projects, prefer
fully qualified names like `vextr::widgets::Button` to avoid name collisions.
:::

## Breaking it down

### 1. Include the library

```cpp
#include <Vextr/Vextr.hpp>
```

This single header includes everything you need - Button, Label, App, layouts, and more.
For larger projects you'd include only what you need, but for getting started
this is fine.

### 2. Create the app and root container

```cpp
App app;

auto root = std::make_shared<Container>(
    std::make_shared<StackLayout>(Axis::Vertical, 1)
);
```

Every Vextr app has an `App` and a root `Container`. `App` owns the main loop -
it polls input, ticks the render cycle, and exits when you call `app.quit()`.
The root `Container` is the top of the widget tree. All other widgets live
inside it, and any event that nothing else handles will bubble up to it - useful
for app-wide shortcuts like Ctrl+S.

`StackLayout(Axis::Vertical, 1)` stacks children vertically with a 1-cell gap
between them. Swap `Axis::Vertical` for `Axis::Horizontal` to stack side by
side.

### 3. Add a label

```cpp
auto countLabel = std::make_shared<Label>("0");
countLabel->setStyle(Style{ .fg = {255, 255, 255}, .text = TextDecoration{ .bold = true } });
```

`Style` uses C++23 designated initializers - you only set the fields you care
about, everything else stays at its default. `.fg` is the foreground (text)
color as RGB. `.text` holds decoration flags like bold, italic, and underline.

### 4. Wire up the buttons

```cpp
auto incrementBtn = std::make_shared<Button>("+");
incrementBtn->setStyle(Style{ .bg = {31, 189, 0} });
incrementBtn->setFocusedStyle(Style{ .bg = {0, 255, 68} });
incrementBtn->setOnClick([countLabel, &count]() {
    count++;
    countLabel->setText(std::to_string(count));
});
```

`setStyle` sets the button's normal appearance. `setFocusedStyle` sets how it
looks when the user tabs to it - the framework switches between them
automatically, you never check focus state manually.

`setOnClick` takes a lambda. Capture `countLabel` by value (shared_ptr, so
it's cheap) and `count` by reference so the lambda reads and writes the same
integer the rest of `main` uses.

The decrement button follows the same pattern with `count--`.

### 5. Add everything to root and run

```cpp
root->addChild(countLabel);
root->addChild(incrementBtn);
root->addChild(decrementBtn);

app.setRoot(root);
app.run();
```

`addChild` adds a widget to the container in order - top to bottom for a
vertical stack. `setRoot` tells the app which widget is the top of the tree.
`run()` starts the main loop and blocks until `app.quit()` is called or the
user exits.



