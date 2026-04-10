#include <Vextr/utils/InputParser.hpp>
#include <Vextr/utils/Input.hpp>

#include "Vextr/utils/Debug.hpp"

namespace vextr::utils {

void InputParser::emit(int key, bool shift, bool ctrl, bool alt) {
    VEXTR_LOG(
    "key=" + std::to_string(key) + "\n"
    " ctrl=" + std::to_string(ctrl) + "\n"
    " alt=" + std::to_string(alt) + "\n"
    " shift=" + std::to_string(shift) + "\n"
    );

    core::Event e;
    e.type  = core::EventType::Key;
    e.key   = key;
    e.shift = shift;
    e.ctrl  = ctrl;
    e.alt   = alt;
    events.push(e);
    buf.clear();
}

void InputParser::flushEscape() {
    emit(Key::Escape);
}

void InputParser::feed(unsigned char byte) {
    buf.push_back(byte);
    if (buf[0] == 13) buf[0] = 10; // this bc windows....
    tryParse();
}

void InputParser::tryParse() {
    if (buf.empty()) return;

    // plain character — not an escape sequence
    if (buf[0] != 27) {
        // ctrl+key detection: ctrl+a = 1, ctrl+z = 26
        bool ctrl = (buf[0] > 0 && buf[0] < 27 && buf[0] != Key::Tab
                                                && buf[0] != Key::Enter);
        int key = ctrl ? (buf[0] + 'a' - 1) : (int)buf[0];
        emit(key, false, ctrl, false);
        return;
    }

    // lone escape — caller handles timeout via pendingEscape()
    if (buf.size() == 1) return;

    // esc + [ — csi sequence
    if (buf[1] == '[') {
        if (buf.size() == 2) return; // wait for more

        // esc [ x — simple sequences
        if (buf.size() == 3) {
            switch (buf[2]) {
                case 'A': emit(Key::Up);       return;
                case 'B': emit(Key::Down);     return;
                case 'C': emit(Key::Right);    return;
                case 'D': emit(Key::Left);     return;
                case 'H': emit(Key::Home);     return;
                case 'F': emit(Key::End);      return;
                case 'Z': emit(Key::ShiftTab, true); return;
                default: break;
            }
            // wait for more
            if (buf[2] >= '0' && buf[2] <= '9') return;
            // unknown 3 byte — drop
            buf.clear();
            return;
        }

        // esc [ n ~ — extended sequences
        if (buf.size() == 4 && buf[3] == '~') {
            switch (buf[2]) {
                case '1': emit(Key::Home);     return;
                case '2': emit(Key::Insert);   return;
                case '3': emit(Key::Delete);   return;
                case '4': emit(Key::End);      return;
                case '5': emit(Key::PageUp);   return;
                case '6': emit(Key::PageDown); return;
                default:  buf.clear();         return;
            }
        }

        // esc [ 1 ; 2 x — modified sequences (shift/ctrl/alt + arrow)
        if (buf.size() == 5 && buf[2] == '1' && buf[3] == ';') {
            bool shift = (buf[4] == '2' || buf[4] == '4' ||
                          buf[4] == '6' || buf[4] == '8');
            bool alt   = (buf[4] == '3' || buf[4] == '4' ||
                          buf[4] == '7' || buf[4] == '8');
            bool ctrl  = (buf[4] == '5' || buf[4] == '6' ||
                          buf[4] == '7' || buf[4] == '8');
            if (buf.size() < 6) return; // wait for final byte
        }
        if (buf.size() == 6 && buf[2] == '1' && buf[3] == ';') {
            bool shift = (buf[4] == '2');
            bool ctrl  = (buf[4] == '5');
            bool alt   = (buf[4] == '3');
            switch (buf[5]) {
                case 'A': emit(Key::Up,    shift, ctrl, alt); return;
                case 'B': emit(Key::Down,  shift, ctrl, alt); return;
                case 'C': emit(Key::Right, shift, ctrl, alt); return;
                case 'D': emit(Key::Left,  shift, ctrl, alt); return;
                default:  buf.clear(); return;
            }
        }

        // f1-f4: esc [ [ a-d (linux console)
        if (buf.size() == 4 && buf[2] == '[') {
            switch (buf[3]) {
                case 'A': emit(Key::F1); return;
                case 'B': emit(Key::F2); return;
                case 'C': emit(Key::F3); return;
                case 'D': emit(Key::F4); return;
                case 'E': emit(Key::F5); return;
                default:  buf.clear();   return;
            }
        }

        // still accumulating — wait if buffer is short
        if (buf.size() < 6) return;

        // give up on unknown long sequence
        buf.clear();
        return;
    }

    // esc O — ss3 sequences (f1-f4 on some terminals)
    if (buf[1] == 'O') {
        if (buf.size() == 2) return; // wait
        switch (buf[2]) {
            case 'P': emit(Key::F1); return;
            case 'Q': emit(Key::F2); return;
            case 'R': emit(Key::F3); return;
            case 'S': emit(Key::F4); return;
            default:  buf.clear();   return;
        }
    }

    // esc + anything else — alt+key
    if (buf.size() == 2) {
        emit((int)buf[1], false, false, true);
        return;
    }

    buf.clear();
}

core::Event InputParser::nextEvent() {
    core::Event e = events.front();
    events.pop();
    return e;
}

} // vextr::utils