# see https://dassur.ma/things/c-to-webassembly/
clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--allow-undefined,--export-all -o app.wasm app.c
