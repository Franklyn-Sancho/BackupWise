use std::{
    ffi::{c_char, CStr, CString},
    fs::{self, File, OpenOptions},
    io::{self, BufRead, BufReader},
    path::{Path, PathBuf},
};

use std::io::Write;

use chrono::{DateTime, Local};

// This function backs up only the modified file
#[no_mangle]
pub extern "C" fn copy_file_to(src_filename: *const c_char, backup_path: *const c_char) -> i32 {
    // Converts C pointers to Rust strings
    let src_filename_str = c_to_rust_string(src_filename);
    let dst_path_str = c_to_rust_string(backup_path);

    if src_filename_str.is_err() || dst_path_str.is_err() {
        return -1; // Error in string conversion
    }

    let src_filename_str = src_filename_str.unwrap();
    let dst_path_str = dst_path_str.unwrap();

    // Defines the source path
    let src_path = Path::new(&src_filename_str);

    // Checks if the source path is valid
    if !src_path.exists() {
        println!("Invalid source path: {:?}", src_path);
        return -3; // Source path not found
    }

    // Defines the destination path (backup folder + filename)
    let dst_file_path = Path::new(&dst_path_str).join(src_path.file_name().unwrap());

    // Copies the file
    if copy_file(&src_path, &dst_file_path).is_err() {
        return -2; // Error copying the file
    }

    // Creates the log
    // Extracts only the filename and not the full path
    let log_file_name = format!(
        "{}_log.txt",
        src_path.file_name().unwrap().to_string_lossy()
    );
    let log_file = Path::new(&dst_path_str)
        .join("historicos")
        .join(log_file_name);

    write_log(&src_path, &log_file.to_string_lossy());

    0 // Success
}

fn c_to_rust_string(filename: *const c_char) -> Result<String, std::str::Utf8Error> {
    let c_str = unsafe { CStr::from_ptr(filename) };
    c_str.to_str().map(|s| s.to_string())
}
/* fn define_paths(filename: &str, backup_path: &str) -> (PathBuf, PathBuf) {
    let src_dir = Path::new("/home/franklyn/Documentos/Códigos/testanto backup");
    let dst_dir = Path::new(backup_path);
    (src_dir.join(filename), dst_dir.join(filename))
} */

fn copy_file(src_path: &Path, dst_path: &Path) -> io::Result<u64> {
    let result = fs::copy(src_path, dst_path);
    if result.is_ok() {
        println!("Item {:?} successfully copied to {:?}", src_path, dst_path);
    }
    result
}

fn write_log(src_path: &Path, log_file: &str) {
    // Checks if the source file exists before trying to open it
    if !src_path.exists() {
        eprintln!("Invalid source path: {:?}", src_path);
        return;
    }

    // Checks if the log file directory exists, if not, creates it
    let log_dir = Path::new(log_file).parent().unwrap();
    if !log_dir.exists() {
        fs::create_dir_all(log_dir).expect("Failed to create log directory");
    }

    let input = File::open(src_path).unwrap(); // Now it should be safe to try to open
    let mut log_output = OpenOptions::new()
        .append(true)
        .create(true)
        .open(log_file)
        .unwrap();

    let now: DateTime<Local> = Local::now();
    writeln!(log_output, "\nChanges made on {}:\n", now).unwrap();

    let reader = BufReader::new(input);
    for line in reader.lines() {
        let line = line.unwrap();
        writeln!(log_output, "{}", line).unwrap();
    }
}

// Função para copiar diretórios
#[no_mangle]
pub extern "C" fn copy_directory(src: *const c_char, dst: *const c_char) -> i32 {
    let src = unsafe { CStr::from_ptr(src).to_string_lossy().into_owned() };
    let dst = unsafe { CStr::from_ptr(dst).to_string_lossy().into_owned() };

    if src == dst {
        println!("Ignoring backup for source path: {} as it is the same as destination path.", src);
        return 0; 
    }

    let src_path = Path::new(&src);
    let dst_path = Path::new(&dst);

    
    match copy_directory_impl(&src_path, &dst_path) {
        Ok(_) => 0,   
        Err(_) => -1,
    }
}


fn copy_directory_impl(src: &Path, dst: &Path) -> io::Result<()> {
    if !src.exists() || !src.is_dir() {
        return Err(io::Error::new(
            io::ErrorKind::NotFound,
            "Source directory not found or is not a directory",
        ));
    }

    if !dst.exists() {
        std::fs::create_dir_all(dst)?;
        println!("Created directory: {:?}", dst);
    }

    for entry in src.read_dir()? {
        let entry = entry?;
        let file_type = entry.file_type()?;
        let src_entry_path = entry.path();
        let dst_entry_path = dst.join(entry.file_name());

        if file_type.is_dir() {
            println!(
                "Copying directory: {:?} to {:?}",
                src_entry_path, dst_entry_path
            );
            copy_directory_impl(&src_entry_path, &dst_entry_path)?; // Chamada recursiva
        } else if file_type.is_file() {
            println!("Copying file: {:?} to {:?}", src_entry_path, dst_entry_path);
            std::fs::copy(&src_entry_path, &dst_entry_path)?;
        }
    }

    Ok(())
}


