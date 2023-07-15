#pragma once

#include <concepts>

template <typename T, typename Payload>
struct Event {
    using Type = T;

    Type type;
    Payload payload;
};

template <typename T>
concept EventType = requires(T event) {
    { event.type } -> std::same_as<typename T::Type>;
    { event.payload };
};
