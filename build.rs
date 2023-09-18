use cc;

fn main() {
    cc::Build::new()
        .file("/home/franklyn/Documentos/Códigos/C/inotify.c")
        .compile("main")
}