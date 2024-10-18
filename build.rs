use cc;

fn main() {
    cc::Build::new()
        .cpp(true)
        .file("./cpp_src/event_checker.cpp")
        .file("./cpp_src/event_handler.cpp")
        .compile("main");
}
