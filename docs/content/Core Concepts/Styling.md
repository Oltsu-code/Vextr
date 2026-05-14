# Styling

Every widget has a `Style` that controls its appearance. Styles use C++23
designated initializers - you only set the fields you care about, everything
else stays at its default.

## The Style struct

```cpp
widget->setStyle(Style{
    .fg      = {255, 255, 255},          // foreground (text) color, RGB
    .bg      = {30, 30, 46},             // background color, RGB
    .text    = TextDecoration{
        .bold          = true,
        .italic        = false,
        .underline     = false,
        .strikethrough = false,
    },
    .border  = Border{
        .style = BorderStyle::Rounded,
        .color = {100, 100, 255},
    },
    .padding      = PaddingSpec{ .top = 1, .left = 2 },
    .innerAlignX  = Align::Center,
    .innerAlignY  = Align::Center,
});
```

Every field is optional. A plain `Style{}` means transparent background,
white text, no border, no decoration.

## Colors

Colors are RGB - three values from 0 to 255.

```cpp
Style{ .fg = {255, 100, 0}  }  // orange text
Style{ .bg = {20, 20, 20}   }  // dark background
Style{ .fg = {0, 0, 0}, .bg = {255, 255, 255} }  // black on white
```

## Text decoration

```cpp
Style{ .text = TextDecoration{ .bold = true } }
Style{ .text = TextDecoration{ .italic = true, .underline = true } }
Style{ .text = TextDecoration{ .strikethrough = true } }
```

All four flags default to `false`. Combine as many as you like.

## Borders

Border takes a `BorderStyle` and an optional color and title/footer labels.

```cpp
Style{ .border = Border{ .style = BorderStyle::Single  } }  // ─┐
Style{ .border = Border{ .style = BorderStyle::Double  } }  // ═╗
Style{ .border = Border{ .style = BorderStyle::Rounded } }  // ─╮
Style{ .border = Border{ .style = BorderStyle::Thick   } }  // ━┓
Style{ .border = Border{ .style = BorderStyle::Dashed  } }  // ╌┐
```

You can also set a border color and title:

```cpp
Style{
    .border = Border{
        .style = BorderStyle::Rounded,
        .color = {100, 200, 255},
        .title = { BorderLabel{ "My Widget" } },
    }
}
```

## Alignment

`innerAlignX` and `innerAlignY` control how content is positioned inside
the widget's own bounds.

```cpp
Style{ .innerAlignX = Align::Center, .innerAlignY = Align::Center }
```

Available values: `Align::Start`, `Align::Center`, `Align::End`,
`Align::Stretch`.

## Padding

Padding adds space between the widget's border and its content.

```cpp
Style{
    .padding = PaddingSpec{ .top = 1, .bottom = 1, .left = 2, .right = 2 }
}
```

## Focus styles

Every widget can have a separate style for when it has keyboard focus.
The framework switches between them automatically - you never check focus
state manually.

```cpp
button->setStyle(       Style{ .bg = {60, 60, 60}  });
button->setFocusedStyle(Style{ .bg = {0, 120, 255} });
```

A common pattern is to keep the focused style identical to the normal one
except for background or border color, so the user can clearly see which
widget is active.

---

→ Next: [Events](events.md) - handling keyboard input and shortcuts.