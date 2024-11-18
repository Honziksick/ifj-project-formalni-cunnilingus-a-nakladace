// Err 7/8
const ifj = @import("ifj24.zig");
pub fn main() void {
    const x = foo();
}

pub fn foo(){
    ifj.write("hello");
}