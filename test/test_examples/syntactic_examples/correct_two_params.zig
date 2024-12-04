const ifj = @import("ifj24.zig");

pub fn main() void {
    
}

pub fn foo (x : i32, y : []u8) void {
    ifj.write(x);
    ifj.write(y);
}