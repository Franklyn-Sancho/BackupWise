use std::{
    ffi::{c_char, CStr},
    fs::{self, File, OpenOptions},
    io::{self, BufRead, BufReader},
    path::{Path, PathBuf},
};

use std::io::Write;

use chrono::{DateTime, Local};

//this function backups only the modified file
#[no_mangle]
pub extern "C" fn copy_file_to(filename: *const c_char) -> i32 {
    let filename_str = c_to_rust_string(filename);
    if filename_str.is_err() {
        return -1;
    }
    let filename_str = filename_str.unwrap();

    let (src_path, dst_path) = define_paths(&filename_str);
    if copy_file(&src_path, &dst_path).is_err() {
        return -2;
    }

    let log_file = format!(
        "/media/franklyn/Seagate Backup Plus Drive/rust/historicos/{}_log.txt",
        filename_str
    );
    write_log(&src_path, &log_file);

    0
}

fn c_to_rust_string(filename: *const c_char) -> Result<String, std::str::Utf8Error> {
    let c_str = unsafe { CStr::from_ptr(filename) };
    c_str.to_str().map(|s| s.to_string())
}

fn define_paths(filename: &str) -> (PathBuf, PathBuf) {
    let src_dir = Path::new("/home/franklyn/Documentos/Códigos/testanto backup");
    let dst_dir = Path::new("/media/franklyn/Seagate Backup Plus Drive/rust");
    (src_dir.join(filename), dst_dir.join(filename))
}

fn copy_file(src_path: &Path, dst_path: &Path) -> io::Result<u64> {
    let result = fs::copy(src_path, dst_path);
    if result.is_ok() {
        println!(
            "Item {:?} copiado com sucesso para {:?}",
            src_path, dst_path
        );
    }
    result
}

fn write_log(src_path: &Path, log_file: &str) {
    let input = File::open(src_path).unwrap();
    let mut log_output = OpenOptions::new()
        .append(true)
        .create(true)
        .open(log_file)
        .unwrap();

    let now: DateTime<Local> = Local::now();
    writeln!(log_output, "\nAlterações feitas em {}:\n", now).unwrap();

    let reader = BufReader::new(input);
    for line in reader.lines() {
        let line = line.unwrap();
        writeln!(log_output, "{}", line).unwrap();
    }
}

//This function backups all files in the dir
#[no_mangle]
pub extern "C" fn copy_dir_to() -> std::io::Result<()> {
    // Definindo os diretórios de origem e destino
    let src_dir = Path::new("/home/franklyn/Documentos/Códigos/testanto backup");
    let dst_dir = Path::new("/media/franklyn/Seagate Backup Plus Drive/rust");

    // Iterando sobre cada entrada no diretório de origem
    for entry_result in src_dir.read_dir()? {
        let entry = entry_result?;
        let file_type = entry.file_type()?;
        let src_path = entry.path();
        let dst_path = dst_dir.join(entry.file_name());

        // Copiando arquivos e diretórios para o diretório de destino
        if file_type.is_file() || file_type.is_dir() {
            fs::copy(&src_path, &dst_path)?;
            println!(
                "Item {:?} copiado com sucesso para {:?}",
                &src_path, &dst_path
            );
        }
    }

    Ok(())
}
