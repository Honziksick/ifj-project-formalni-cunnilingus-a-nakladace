const ifj = @import("ifj24.zig");
pub fn main() void {
    const x: i32 = 10;
    const y = foo(x);
    ifj.write(y);
}

pub fn foo(z: i32) ?[]u8 {
    ifj.write(z);
    return null;
}
