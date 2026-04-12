# Vextr Roadmap

Status markers: ✅ done · 🔄 in progress · ⬜ planned · 💡 idea

---

## Phase 1 — Core pipeline ✅

The fundamental render loop and widget system.

- ✅ Buffer — 2D cell grid with Color, bold, underline per cell
- ✅ Terminal — cross-platform setup/restore, terminalSize, flush
- ✅ Renderer — traverses widget tree, writes to buffer, flushes to terminal
- ✅ Widget — base class with measure/layout/render pipeline
- ✅ App — main loop, input polling, resize handling (SIGWINCH + Windows polling)
- ✅ Flicker-free rendering — full frame buffered into ostringstream, one write per tick
- ✅ Cross-platform — Windows WINAPI + Unix termios both working

---

## Phase 2 — Layout engine ✅

Pluggable layout system replacing hardcoded direction logic.

- ✅ Layout — abstract base class
- ✅ StackLayout — vertical/horizontal with ratio-based sizing and gap
- ✅ GridLayout — rows/cols with colSpan/rowSpan and gaps
- ✅ AbsoluteLayout — explicit x/y positioning, enables overlapping
- ✅ LayoutSpec — per-child constraints: ratio, fixedW/H%, minW/H, maxW/H, margin, alignment
- ✅ Container — widget that owns children + a layout engine
- ✅ Align enum — Start, Center, End, Stretch on both axes

---

## Phase 3 — Style system ✅

Rich per-widget visual properties.

- ✅ Color — RGB fg/bg per cell
- ✅ Bold, underline per cell
- ✅ Padding — per-side with percent support
- ✅ Margin — handled by parent layout
- ✅ Border — Single, Double, Rounded, Thick, Dashed styles
- ✅ Transparent color — widgets that don't fill their background
- ✅ Dim, italic, blink attributes

---

## Phase 4 — Leaf widgets 🔄

The building blocks users actually put in their UIs.

- ✅ Label — static text rendering
- ✅ Button — focusable, onClick callback, hover/active states
- ✅ Input — single-line text field with cursor, selection
- ⬜ TextView — multi-line read-only text with word wrap
- ⬜ ScrollView — clipping container with vertical/horizontal scroll
- ⬜ Checkbox — toggleable boolean widget
- ⬜ RadioGroup — mutually exclusive options
- ⬜ ProgressBar — horizontal fill indicator
- ⬜ Spinner — animated loading indicator

---

## Phase 5 — Focus and input routing 🔄

Keyboard navigation and event routing.

- ✅ FocusManager — tracks focused widget, lives in Context
- ✅ Focus traversal — Tab/Shift-Tab cycles through focusable widgets in tree order
- 🔄 Navigation groups — containers where arrow keys move focus between children
- 🔄 Event bubbling — unhandled events walk up parent chain
- ⬜ Mouse hit-testing — click sets focus to widget under cursor
- ✅ isFocusable() — widgets declare focus eligibility
- ✅ Focus styles — visual indicator on focused widget

---

## Phase 6 — Navigator and screens ✅

Multi-view navigation and overlay system.

- ✅ Navigator widget — screen stack, push/pop, renderable anywhere in the tree
- ✅ Screen alias — Container used as a full-view screen
- ✅ Overlay rendering — all stack layers rendered bottom to top
- ✅ Dialog — modal overlay with transparent background showing screen beneath
- ✅ Transition hooks — callbacks on push/pop for future animation support

---

## Phase 7 — Navigation components ⬜

Higher-level UI patterns built on the focus and navigator systems.

- ⬜ MenuBar — horizontal top bar with dropdown menus
- ⬜ ContextMenu — floating overlay opened at a position
- ⬜ CommandPalette — full overlay with text input and filtered list (Ctrl+P style)
- ⬜ Tabs — horizontal tab bar switching between child containers
- ⬜ Sidebar nav — vertical list that switches Navigator content

---

## Phase 8 — Advanced layout ⬜

Layout engine improvements and new types.

- ⬜ StackLayout main-axis alignment — Center/End on main axis, not just cross
- ⬜ Explicit fixed sizes in LayoutSpec — fixedCols, fixedRows in terminal cells
- ⬜ WrapLayout — children wrap to next row when they overflow
- ⬜ Lazy layout — skip re-layout of subtrees that haven't changed

---

## Phase 9 — Unicode and rendering quality ✅

Correctness improvements for real-world text.

- ✅ Wide character support — CJK and emoji take 2 columns, Buffer aware
- ✅ UTF-8 iteration in Label/Input — correct codepoint handling
- ⬜ Grapheme cluster support — combining characters treated as one unit
- ⬜ Dirty region tracking — only re-flush cells that changed since last frame

---

## Phase 10 — Testing and examples ⬜

Making the library usable and trustworthy.

- ⬜ Unit tests — Buffer, StackLayout, GridLayout, Label measure/render
- ⬜ Example: todo app — Input, Label, ScrollView, keyboard nav
- ⬜ Example: file browser — Navigator, tree view, keyboard focus
- ⬜ Example: dashboard — Grid layout, ProgressBar, live updating data
- ⬜ Example: form — Input, Checkbox, RadioGroup, Button, validation

---

## Future ideas 💡

Not committed, just worth thinking about.

- 💡 Animation system — frame-timed callbacks for transitions and effects
- 💡 Mouse support — full mouse tracking with hover states
- 💡 Async data binding — widgets that re-render when an observable changes
- 💡 GUI backend — swap Terminal for a windowed renderer, widgets unchanged
- 💡 Hot reload — rebuild widget tree without restarting the process