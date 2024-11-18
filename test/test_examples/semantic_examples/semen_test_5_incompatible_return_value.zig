// Program: Vraceni f64 do promenne typu i32
//ERR4

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("?xes ydk ...ezkaT\n");

    const a: i32 = decrement(10.5, 5.2);  //Vkladani f64 do var i32

    ifj.write(a);
}

pub fn decrement(n: f64, m: f64) f64 {
    return n - m;
}