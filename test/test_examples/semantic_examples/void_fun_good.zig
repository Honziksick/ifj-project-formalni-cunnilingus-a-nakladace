// Korektní program
const ifj = @import("ifj24.zig");
pub fn main() void {
    foo();
}

pub fn foo(){
    ifj.write("hello");
}