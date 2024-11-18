// Korektni program
const ifj = @import("ifj24.zig");
pub fn main() void {
    const x : i32 = 5;
    _ = f(x);   // správné využití pseudoproměnné
}

pub fn f (x : i32) i32 {
  return x;
}