// Neukončený řetězec
const ifj = @import("ifj24.zig");

pub fn main() void {
  const s1 : []u8 = ifj.string("Unterminated string); // ukoncujici uvozovky ovlivnuji implicitni odsazeni vnitrnich radku retezce
  ifj.write(s1);
}

