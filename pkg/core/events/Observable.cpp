#include "Observable.hpp"

void Observable::on(Event::Type event, const Callback& cb) {
    cbs_[event].push_back(cb);
}

void Observable::emit(Event event) {
    for (const auto& cb : cbs_[event.type]) {
        cb(event);
    }
}
