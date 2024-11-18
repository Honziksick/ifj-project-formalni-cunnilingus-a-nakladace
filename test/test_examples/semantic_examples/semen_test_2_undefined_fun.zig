// Program: Nedefinovana funkce
//ERR3

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("ned ynkep ijerp\n");

    const a: i32 = decrement(10, 5);
    const b: i32 = increment(20, 30); //Volani nedefinovane funkce 'increment'

    ifj.write(a);
    ifj.write(b);
}

pub fn decrement(n: i32, m: i32) i32 {
    return n - m;
}