#include "Mediator.hpp"

Mediator& Mediator::get() {
    static Mediator instance;
    return instance;
}

void Mediator::on(Event::Type event, const Callback& cb) {
    cbs_[event].push_back(cb);
}

void Mediator::off(Event::Type event, const Callback& cb) {}

void Mediator::emit(Event event) {
    for (const auto& cb : cbs_[event.type]) {
        cb(event);
    }
}
