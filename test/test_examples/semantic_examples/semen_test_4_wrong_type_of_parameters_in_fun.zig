// Program: Spatny typ parametru
//ERR4

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("?xes ydk ...ezkaT\n");

    const a: i32 = decrement(10, "Kys");  //[]u8 namísto i32

    ifj.write(a);
}

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}