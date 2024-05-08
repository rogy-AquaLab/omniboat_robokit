#ifndef OMNIBOAT_ROBOKIT_TRACE_HPP
#define OMNIBOAT_ROBOKIT_TRACE_HPP

namespace trace {
enum class LedId : uint8_t {
    First,
    Second,
    Third
};

void init();
void toggle(LedId);
void on(LedId);
void off(LedId);
}  // namespace trace

#endif  // OMNIBOAT_ROBOKIT_TRACE_HPP
