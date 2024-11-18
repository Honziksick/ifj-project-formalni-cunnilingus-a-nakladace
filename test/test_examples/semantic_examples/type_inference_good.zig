// Korektni program
const ifj = @import("ifj24.zig");
pub fn main() void {
    const x = foo();
    const y : f64 = ifj.i2f(x);
    ifj.write(y);
}

pub fn foo()i32{
    return 5;
}