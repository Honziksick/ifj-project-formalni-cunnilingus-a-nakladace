// Program: Nevyuziti promenne
//ERR9

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("lliw syawla I dnA\n");
    
    const a: i32 = 12;

    ifj.write(a);
}

pub fn decrement(n: i32, m: i32) i32 {
    const b: i32 = 16;    //Nevyuzita promenna
    return n - m;
}