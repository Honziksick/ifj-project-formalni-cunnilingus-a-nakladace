// Err 8/7
const ifj = @import("ifj24.zig");
pub fn main() void {
    const x = foo();
    ifj.write(x);
}

pub fn foo() void{
    ifj.write(5);
}