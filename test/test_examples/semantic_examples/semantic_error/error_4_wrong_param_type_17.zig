// Semantic error: Wrong parameter type of function "ifj.string()" on line 14.1

const ifj = @import("ifj24.zig"); // 1

pub fn f (x : f64) i32    // seznam parametru // 2
{ // 3
  // deklarace funkce; v IFJ24 nejsou blokove komentare // 4
  if(x<10) { // 5
    return x-1; // 6
  } // 7
  else { // 8
    const y = x - 1; // cannot redefine x (shadowing is forbidden) // 9
    ifj.write("calling g with "); // 10s
    ifj.write(y); // 11
    ifj.write("\n"); // 12
    const res = g(y); // 13
    var convert = ifj.string(10); // 14.1
    convert = convert + 1.14; // 14.2
    return res; // 14.2
  } // 15
} // 16

pub fn g(x:i32) i32 { // 17
  if (x > 0) { // 18
    ifj.write("calling f with "); // 19
    ifj.write(x); // 20
    ifj.write("\n"); // 21
    const y = f(x); // inicializace konstanty volanim funkce // 22
    return y; // 23
  } // 24
  else { // 25
    return 200; // 26
  } // 27
} // 28

pub fn main() void { // 29
  const res = g(10); // 30
  ifj.write("res: "); // 31
  ifj.write(res); // 32
  ifj.write("\n"); // 33
} // 34
