# Layout

Layout controls how widgets are sized and positioned inside a container.
Vextr has three layout types: Stack, Grid, and Absolute.

## Stack Layout

Children line up vertically or horizontally. This is what you'll use most
of the time.

```cpp
// vertical stack, 1-cell gap between children
auto container = std::make_shared<Container>(
    std::make_shared<StackLayout>(Axis::Vertical, 1)
);

// horizontal stack, no gap
auto toolbar = std::make_shared<Container>(
    std::make_shared<StackLayout>(Axis::Horizontal, 0)
);
```

```
Vertical (Axis::Vertical, gap=1)    Horizontal (Axis::Horizontal, gap=0)

┌─────────────┐                     ┌──────┬──────┬──────┐
│  widget 1   │                     │  w1  │  w2  │  w3  │
├─────────────┤  ← gap              └──────┴──────┴──────┘
│  widget 2   │
├─────────────┤  ← gap
│  widget 3   │
└─────────────┘
```

## Controlling size with LayoutSpec

By default, children share the available space equally via `ratio = 1.0`.
Pass a `LayoutSpec` to `addChild()` to override this.

### Fixed size

Pin a widget to an exact number of rows or columns using `cells()`:

```cpp
root->addChild(header,  { .fixedH = cells(1) });  // always 1 row tall
root->addChild(content);                            // fills remaining space
root->addChild(footer,  { .fixedH = cells(1) });  // always 1 row tall
```

```
┌─────────────┐  ← header  (fixedH = 1)
├─────────────┤
│             │
│   content   │  ← fills everything left over
│             │
├─────────────┤
└─────────────┘  ← footer  (fixedH = 1)
```

This header/content/footer pattern appears in almost every Vextr app.

### Ratio

Share remaining space proportionally between children:

```cpp
container->addChild(sidebar, { .ratio = 1.0f });  // 1/3 of space
container->addChild(main,    { .ratio = 2.0f });  // 2/3 of space
```

### Min and max size

Constrain how small or large a widget can grow:

```cpp
container->addChild(widget, { .minW = cells(20), .maxW = cells(60) });
```

### Alignment within slot

When a widget is smaller than its slot, align it:

```cpp
container->addChild(widget, {
    .sizeW  = cells(20),        // widget is 20 cols wide
    .alignH = Align::Center,    // centered horizontally in its slot
});
```

### LayoutSpec fields at a glance

| Field       | What it does                                      |
|-------------|---------------------------------------------------|
| `fixedW`    | Fixed width in terminal columns                   |
| `fixedH`    | Fixed height in terminal rows                     |
| `ratio`     | Proportional share of remaining space (default 1) |
| `sizeW`     | Widget width inside its slot                      |
| `sizeH`     | Widget height inside its slot                     |
| `minW/minH` | Minimum size                                      |
| `maxW/maxH` | Maximum size                                      |
| `alignH`    | Horizontal alignment within slot                  |
| `alignV`    | Vertical alignment within slot                    |
| `margin`    | Space around the widget                           |

## Grid Layout

Children fill a grid of rows and columns.

```cpp
auto grid = std::make_shared<Container>(
    std::make_shared<GridLayout>(2, 3)  // 2 rows, 3 columns
);
```

Place children at specific cells using `col` and `row`:

```cpp
grid->addChild(widget, { .col = 0, .row = 0 });
grid->addChild(widget, { .col = 1, .row = 0 });
grid->addChild(widget, { .col = 0, .row = 1 });
```

Span multiple cells with `colSpan` and `rowSpan`:

```cpp
grid->addChild(header, { .col = 0, .row = 0, .colSpan = 3 });  // full width
```

```
┌──────┬──────┬──────┐
│    header (span 3) │
├──────┼──────┼──────┤
│  w1  │  w2  │  w3  │
└──────┴──────┴──────┘
```

## Absolute Layout

Children are placed at explicit coordinates. Useful for overlays and
floating elements.

```cpp
auto canvas = std::make_shared<Container>(
    std::make_shared<AbsoluteLayout>()
);

canvas->addChild(widget, { .absX = cells(10), .absY = cells(5) });
```

Children can overlap - later children render on top of earlier ones.
