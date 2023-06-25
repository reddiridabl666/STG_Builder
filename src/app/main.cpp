import Window;

int main() {
    WindowSFML window("STG_Builder", 640, 480);

    while (window.is_open()) {
        window.render_ui();
        window.process_events();
        window.display();
    }

    return 0;
}
