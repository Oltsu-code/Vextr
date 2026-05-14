# Events

Vextr routes input events through the widget tree. The focused widget gets
each event first. If it doesn't handle it, the event bubbles up to its
parent, then its grandparent, all the way to the root.

## How bubbling works

```
User presses Ctrl+S
        │
        ▼
  TextArea (focused) ── returns false ──▶ not handled
        │
        ▼
  Container ── has addEventListener ──▶ handles it, returns true
        │
       stop
```

This means you can put app-wide shortcuts on the root container and they'll
fire whenever nothing else claims the event first.

## App-wide shortcuts with addEventListener

```cpp
root->addEventListener([&](const events::Event& e) -> bool {
    if (auto* k = std::get_if<events::KeyEvent>(&e)) {
        if (k->modifiers.ctrl && k->key == 's') {
            save();
            return true;   // handled - stop bubbling
        }
    }
    return false;  // not handled - keep bubbling
});
```

Return `true` to consume the event. Return `false` to let it continue up
the tree. You can register multiple listeners on the same widget - they run
in registration order.

## Event types

### KeyEvent

Fired on every key press.

```cpp
if (auto* k = std::get_if<events::KeyEvent>(&e)) {
    k->key;              // key code - ASCII or Key:: constant
    k->modifiers.ctrl;   // Ctrl held
    k->modifiers.shift;  // Shift held
    k->modifiers.alt;    // Alt held
}
```

For printable characters, `key` is the ASCII code - `'a'`, `'1'`, `' '`,
etc. For special keys, use the constants from `vextr::utils::Key`:

| Constant             | Key           |
|----------------------|---------------|
| `Key::Enter`         | Enter         |
| `Key::Escape`        | Escape        |
| `Key::Backspace`     | Backspace     |
| `Key::Tab`           | Tab           |
| `Key::ShiftTab`      | Shift+Tab     |
| `Key::Up`            | Arrow Up      |
| `Key::Down`          | Arrow Down    |
| `Key::Left`          | Arrow Left    |
| `Key::Right`         | Arrow Right   |
| `Key::PageUp`        | Page Up       |
| `Key::PageDown`      | Page Down     |
| `Key::Home`          | Home          |
| `Key::End`           | End           |
| `Key::Delete`        | Delete        |
| `Key::Insert`        | Insert        |
| `Key::F1`–`Key::F12` | Function keys |

### ResizeEvent

Fired when the terminal is resized. Vextr re-layouts automatically, but
you can listen to this if you need to react manually.

```cpp
if (auto* r = std::get_if<events::ResizeEvent>(&e)) {
    r->width;   // new terminal width in columns
    r->height;  // new terminal height in rows
}
```

### Mouse events

:::note
Mouse events are defined but not yet fully implemented. They will be wired
up in a future release.
:::

`MousePressEvent`, `MouseReleaseEvent`, and `MouseMoveEvent` are available
in the event variant but not yet dispatched by the framework.

## Handling events in custom widgets

If you're building a custom widget, override `onEvent()` directly instead
of using `addEventListener`. Always call `Widget::onEvent(event)` first so
any listeners attached to your widget still fire.

```cpp
bool MyWidget::onEvent(const events::Event& event) {
    if (Widget::onEvent(event)) return true;  // listeners first

    if (auto* k = std::get_if<events::KeyEvent>(&event)) {
        if (k->key == Key::Enter) {
            activate();
            return true;
        }
        if (k->key == Key::Escape) {
            cancel();
            return true;
        }
    }

    return false;  // not handled
}
```

## Using std::visit for exhaustive matching

If you want to handle multiple event types without chaining `get_if`:

```cpp
root->addEventListener([](const events::Event& e) -> bool {
    return std::visit(events::overloaded{
        [](const events::KeyEvent& k) -> bool {
            // handle key
            return false;
        },
        [](const events::ResizeEvent& r) -> bool {
            // handle resize
            return false;
        },
        [](const auto&) -> bool {
            return false;  // catch-all
        }
    }, e);
});
```


