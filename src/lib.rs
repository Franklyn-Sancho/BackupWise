use std::{
    ffi::{c_char, CStr},
    fs,
    path::Path,
};

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

//this function backups only the modified file
#[no_mangle]
pub extern "C" fn copy_file_to(filename: *const c_char) -> i32 {
    // Convertendo o argumento C para uma string Rust
    let c_str = unsafe { CStr::from_ptr(filename) };
    let filename_str = match c_str.to_str() {
        Ok(s) => s,
        Err(_) => return -1,
    };

    // Definindo os diretórios de origem e destino
    let src_path =
        Path::new("/home/franklyn/Documentos/Códigos/testanto backup").join(filename_str);
    let dst_path = Path::new("/media/franklyn/Seagate Backup Plus Drive/rust").join(filename_str);

    // Copiando o arquivo para o diretório de destino
    match fs::copy(&src_path, &dst_path) {
        Ok(_) => {
            println!(
                "Item {:?} copiado com sucesso para {:?}",
                &src_path, &dst_path
            );
            0
        }
        Err(_) => -2,
    }
}
