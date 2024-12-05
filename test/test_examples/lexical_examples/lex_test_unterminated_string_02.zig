// Neukončený řetězec
const ifj = @import("ifj24.zig");

pub fn main() void {
  const s1 : []u8 = "Unterminated string;
  ifj.write(s1);
}

