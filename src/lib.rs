use std::{path::Path, fs};

#[no_mangle]
pub extern "C" fn copy_dir_to() -> std::io::Result<()> {
    // Definindo os diretórios de origem e destino
    let src_dir = Path::new("");
    let dst_dir = Path::new("");

    // Iterando sobre cada entrada no diretório de origem
    for entry_result in src_dir.read_dir()? {
        let entry = entry_result?;
        let file_type = entry.file_type()?;
        let src_path = entry.path();
        let dst_path = dst_dir.join(entry.file_name());

        // Copiando arquivos e diretórios para o diretório de destino
        if file_type.is_file() || file_type.is_dir() {
            fs::copy(&src_path, &dst_path)?;
            println!("Item {:?} copiado com sucesso para {:?}", &src_path, &dst_path);
        }
    }

    Ok(())
}