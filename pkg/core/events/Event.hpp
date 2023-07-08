#pragma once

#include <concepts>

template <typename T, typename U>
struct Event {
    using Type = U;

    Type type;
    T payload;
};

template <typename T>
concept EventType = requires(T event) {
                        { event.type } -> std::same_as<typename T::Type>;
                        { event.payload };
                    };
